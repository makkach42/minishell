/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/05/14 16:13:52 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_arr_fill_helper(t_tree **tree, char **str)
{
	t_list	*head;
	char	**arr;
	t_list	*tmp;
	int		i;
	int		list_size;

	i = 0;
	head = list_init(*str);
	list_size = lst_size(&head);
	arr = malloc(sizeof(char *) * (list_size + 1));
	tmp = head;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->data);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	free_list(&head);
	(*tree)->command_arr = arr;
}

void	command_arr_fill(t_tree **tree)
{
	char	*str;
	char	*tmp;
	char	**arr;

	if ((*tree) && (*tree)->left)
		command_arr_fill(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		command_arr_fill(&(*tree)->right);
	if ((tree) && (*tree) && (*tree)->command && !(*tree)->command_arr)
	{
		str = ft_strdup((*tree)->command);
		if (!str || !*str)
		{
			arr = malloc(sizeof(char *));
			if (arr)
				arr[0] = NULL;
			(*tree)->command_arr = arr;
			free(str);
			return ;
		}
		tmp = str;
		str = ft_strtrim(str, " ");
		free(tmp);
		command_arr_fill_helper(tree, &str);
	}
}

void	env_fill_helper(t_env **node, int *i, int *j, char **argev)
{
	*node = malloc(sizeof(t_env));
	while (argev[*i] && argev[*i][*j] != '=')
		(*j)++;
	(*node)->key = ft_substr(argev[*i], 0, *j);
	(*node)->value = ft_substr(argev[*i],
			(*j + 1), ft_strlen(argev[*i]) - (*j + 1));
		*j = 0;
}

void	env_fill_empty(t_env **node, int *i, int *j)
{
	// t_env *temp;
	t_env *new_node;

	if (node == NULL)
		exit (1);
	*node = NULL;
	ft_lstadd_back(node, ft_lstnew("OLDPWD", NULL));
	ft_lstadd_back(node, ft_lstnew("PWD", getcwd(NULL, 0)));
	ft_lstadd_back(node, ft_lstnew("SHLVL", "1"));
	ft_lstadd_back(node, ft_lstnew("PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
}

t_env	*env_fill(char **argev)
{
	int		i;
	int		j;
	t_env	*head;
	t_env	*new_node;
	t_env	*tmp;

	i = 0;
	j = 0;
	if (!argev || !*argev)
		env_fill_empty(&head, &i, &j);
	else
	{
		env_fill_helper(&head, &i, &j, argev);
		head->next = NULL;
		tmp = head;
		i++;
		while (argev[i])
		{
			env_fill_helper(&new_node, &i, &j, argev);
			new_node->next = NULL;
			tmp->next = new_node;
			tmp = new_node;
			i++;
		}
		tmp = head;
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, "OLDPWD"))
				(free(tmp->value), tmp->value = NULL);
			tmp = tmp->next;
		}
	}
	return (head);
}

void	ft_exec(t_tree *tree, t_env *h, char **e)
{
	char	*temp;
	t_env *path;
	char *env;
	// t_pip *value;

	path = ft_check(h, "PATH");
	if (path == NULL || path->value == NULL || check_empty(path->value) == 1)
		env = NULL;
	else
		env = path->value; 
	temp = tree->command_arr[0];
	tree->command_arr[0] =  ft_cmd_check(env, tree->command_arr[0]);
	if (tree->command_arr[0] == NULL && env == NULL)
	{
		tree->command_arr[0] = temp;
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, tree->command_arr[0]);
		ft_putstr_fd(2, ": No such file or directory\n");
	}
	else if (tree->command_arr[0] == NULL)
	{
		tree->command_arr[0] = temp;
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, tree->command_arr[0]);
		ft_putstr_fd(2, ": command not found\n");
		exit (1);
	}
	execve(tree->command_arr[0], tree->command_arr, e);
	exit (1);
}

int	ft_execute(t_tree *tree, t_env **h, char **e);

int	ft_pip(t_tree *tree, t_env **h, char **e)
{
	int	fd[2];
	int id1 = 0;
	int id2 = 0;
	int	status;
	
	status = 0;
	if (pipe(fd) == -1)
		perror("minishell: pipe: ");
	id1 = fork();
	if (id1 == -1)
	{
		perror("minishell: fork: ");
		exit (1);
	}
	if (id1 == 0)
	{
		dup2(fd[1], 1);
		close (fd[1]);
		close (fd[0]);
		exit (ft_execute(tree->left, h, e));
	}
	id2 = fork();
	if (id2 == -1)
	{
		perror("minishell: fork: ");
		exit (1);
	}
	if (id2 == 0)
	{
		dup2(fd[0], 0);
		close (fd[0]);
		close (fd[1]);
		exit(ft_execute(tree->right, h, e));
	}
	// waitpid(id1, NULL, 0);
	close (fd[0]);
	close (fd[1]);
	waitpid(id2, &status, 0);
	while (wait(NULL) != -1) ;
	return (WEXITSTATUS(status));
}

int	ft_cmd_exec(t_tree *tree, t_env **h)
{
	int status;

	status = 0;
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		status = ft_cd(tree->command_arr, *h);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		status = ft_echo(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		status = ft_env(*h);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		ft_exit(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		status = ft_export(tree->command_arr, *h, tree);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		status = ft_pwd();
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
	{
		ft_unset(h, tree->command_arr);
	}
	return (status);
}

int	ft_redir_check(char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (1);
	else if (ft_strcmp(str, ">") == 0)
		return (2);
	else if (ft_strcmp(str, "<<") == 0)
		return (3);
	else if (ft_strcmp(str, ">>") == 0)
		return (4);
	return (-1);
}

void	ft_hdoc_free(char **str, char **limiter, int fd)
{
	free (*str);
	free (*limiter);
	close (fd);
}

char	*ft_name_check(char *name)
{
	int	i;
	char *add;
	char *tmp;

	i = 0;
	add = NULL;
	tmp = NULL;
	if (name == NULL)
		return (NULL);
	while (access(name, F_OK) == 0)
	{
		tmp = name;
		add = ft_itoa(i);
		if (!add)
			return (NULL);
		name = ft_strjoin(name, add);
		if (!name)
			return (NULL);
		free (tmp);
		i++;
	}
	return (name);
}

int	ft_hdoc(char *limiter, char *name)
{
	char	*str;
	int		fd;
	char	*tmp;
	int		fd1;

	unlink(name);
	name = ft_name_check(name);
	fd = open(name, O_RDWR | O_CREAT, 0777);
	if (fd == -1)
		return (1);
	fd1 = open(name, O_RDONLY, 0777);
	if (fd1 == -1)
		return (1);
	unlink(name);
	str = readline("> ");
	while (str != NULL && ft_strcmp(limiter, str) != 0)
	{
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		tmp = str;
		str = readline("> ");
		free(tmp);
	}
	ft_hdoc_free(&str, &limiter, fd);
	//i was unlinking the name in return but changed it
	return (fd1);
}

int	ft_exec_redir(t_tree *tree, t_env **h, char **env)
{
	while (tree->fd_list != NULL && tree->ambiguous == 0)
	{
		if (ft_redir_check(tree->fd_list->redir) == 3)
		{
			dup2(tree->fd_list->fd, 0);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 2)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return (-1);
			}
			dup2(tree->fd_list->fd, 1);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 1)
		{
			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return (-1);
			}
			dup2(tree->fd_list->fd, 0);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 4)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
			if (tree->fd_list ->fd == -1)
			{
				// printf("minishell: %s: %s\n", tree->fd_list->name, strerror(errno));
				perror("minishell: ");
				return (-1);
			}
			dup2(tree->fd_list->fd, 1);
		}
		tree->fd_list = tree->fd_list->next;
	}
	if (tree->ambiguous == 1)
		return (1);
	if (tree->command_arr[0])
		ft_exec(tree, *h, env);
}

int ft_cmd_redir(t_tree *tree, t_env **h)
{
	int	org_stdout;
	int	org_stdin;
	int	status;

	org_stdout = dup(1);
	org_stdin = dup(0);
	status = 0;
	if (check_empty(tree->fd_list->name) == 1)
		return (1);//must be checked
	while (tree->fd_list != NULL && tree->ambiguous == 0)
	{
		if (ft_redir_check(tree->fd_list->redir) == 1)
		{
			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
			if (tree->fd_list ->fd == -1)
			{
				ft_putstr_fd(2, "minishell: ");
				ft_putstr_fd(2, tree->fd_list->name);
				ft_putstr_fd(2, ": No such file or directory");
				return (-1);
			}
			dup2(tree->fd_list->fd, 0);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 2)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return (-1);
			}
			dup2(tree->fd_list->fd, 1);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 4)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
			if (tree->fd_list ->fd == -1)
			{
				// printf("minishell: %s: %s\n", tree->fd_list->name, strerror(errno));
				perror("minishell: ");
				return (-1);
			}
			dup2(tree->fd_list->fd, 1);
		}
		tree->fd_list = tree->fd_list->next;
	}
	if (tree->ambiguous == 1)
		return (1);
	status = ft_cmd_exec(tree, h);
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	return (status);
}

int	ft_variable(t_tree *tree)
{
	ft_putstr_fd(2, "minishell: ");
	ft_putstr_fd(2, tree->command_arr[0]);
	ft_putstr_fd(2, ": command not found\n");
}

int	check_amb(t_tree *tree)
{
	int	amb_check;
	int	left_check;
	int	right_check;

	amb_check = 0;
	left_check = 0;
	right_check = 0;
	if (tree->ambiguous == 1)
		amb_check = 1;
	if (tree->left)
		left_check = check_amb(tree->left);
	if (tree->right)
		right_check = check_amb(tree->right);
	return (amb_check || left_check || right_check);
}

int ft_para_redir(t_tree *tree, t_env **h)
{
	// int	org_stdout;
	// int	org_stdin;
	int	status;

	// org_stdout = dup(1);
	// org_stdin = dup(0);
	status = 0;
	if (check_amb(tree) == 1)
		return (1);//return value must be checked
	while (tree->fd_list != NULL && check_empty(tree->fd_list->name) != 1)
	{
		dprintf(2, "entered in here\n");
		if (ft_redir_check(tree->fd_list->redir) == 1)
		{
			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
			if (tree->fd_list ->fd == -1)
			{
				ft_putstr_fd(2, "minishell: ");
				ft_putstr_fd(2, tree->fd_list->name);
				ft_putstr_fd(2, ": No such file or directory");
				return (-1);
			}
			dup2(tree->fd_list->fd, 0);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 2)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return (-1);
			}
			dup2(tree->fd_list->fd, 1);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 4)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
			if (tree->fd_list ->fd == -1)
			{
				// printf("minishell: %s: %s\n", tree->fd_list->name, strerror(errno));
				perror("minishell: ");
				return (-1);
			}
			dup2(tree->fd_list->fd, 1);
		}
		tree->fd_list = tree->fd_list->next;
	}
	// status = ft_cmd_exec(tree, h);
	// dup2(org_stdout, 1);
	// dup2(org_stdin, 0);
	return (status);
}

int	ft_parenthasis(t_tree *tree, t_env **h, char **e)
{
	int	fd[2];
	int id1 = 0;
	int id2 = 0;
	int	org_stdout;
	int	org_stdin;
	int	check;
	
	check = 0;
	org_stdout = dup(1);
	org_stdin = dup(0);
	if (!tree)
		return (-1);
	check = ft_para_redir(tree, h);
	if (check)
		return (1);
	if (tree->left)
		ft_execute(tree->left, h, e);
	if (tree->right)
		ft_execute(tree->right, h, e);
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	// if (tree->redirections != NULL)
	// {
	// 	tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
	// 	if (tree->fd_list ->fd == -1)
	// 	{
	// 		perror("minishell: ");
	// 		return (-1);
	// 	}
	// 	// dup2(tree->fd_list->fd, 1);
	// }
	return (0);
}




int	ft_execute(t_tree *tree, t_env **h, char **e)
{
	int id;
	int	status;
	int	check;
	int	redir;

	status = 0;
	check = 0;
	if (!tree)
		return (1);
	if (ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections == NULL)
	{
		if (variable_search(&tree) == 1) //TO EXPAND WITH IN EXECUTION THIS SEARCHES FOR VARIABLES AND THE NEXT ONE EXPANDS THEM
			variable_expantion(&tree, h);
		status = ft_cmd_exec(tree, h);
	}
	if (ft_strcmp("PARENTHASIS", tree->type) == 0)
	{
		if (variable_search_inlnkedlst(&tree) == 1)
		{
			// dprintf(2, "this is variable serach:%d\n", variable_search_inlnkedlst(&tree));
			variable_expantion_para(&tree, h);
			// printf("***************************\n");
			// print_tree_visual(tree, 1, 1);
			// printf("***************************\n");
		}
		status = ft_parenthasis(tree, h, e);
	}
	if (ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections != NULL)
	{
		if (variable_search_inlnkedlst(&tree) == 1)
			variable_expantion_inlnkedlst(&tree, h);
		status = ft_cmd_redir(tree, h);
	}
	if (ft_strcmp("OPERATION_&&", tree->type) == 0)
	{
		status = ft_execute(tree->left, h, e);
		if (status == 0)
			status = ft_execute(tree->right, h, e);
	}
	if (ft_strcmp("OPERATION_||", tree->type) == 0)
	{
		status = ft_execute(tree->left, h, e);
		if (status != 0)
			status = ft_execute(tree->right, h, e);
	}
	if (ft_strcmp("VARIABLE", tree->type) == 0)
	{
		status = ft_variable(tree);
	}
	if (tree->redirections == NULL && ft_strcmp("WORD", tree->type) == 0)
	{
		if (variable_search(&tree) == 1) //TO EXPAND WITH IN EXECUTION THIS SEARCHES FOR VARIABLES AND THE NEXT ONE EXPANDS THEM
			variable_expantion(&tree, h);
		id = fork();
		if (id == 0)
			ft_exec(tree, *h, e);
		else if (id < 0)
			perror("fork");
		waitpid(id, &status, 0);
		status = WEXITSTATUS(status);
	}
	if ((tree->redirections != NULL && ft_strcmp("WORD", tree->type) == 0) || ft_strcmp("REDIRECTION", tree->type) == 0)
	{
		id = fork();
		if (id == 0)
		{
			if (variable_search_inlnkedlst(&tree) == 1)
				variable_expantion_inlnkedlst(&tree, h);
			ft_exec_redir(tree, h, e);
		}
		waitpid(id, &status, 0);
		status = WEXITSTATUS(status);
	}
	if (ft_strcmp("PIPE", tree->type) == 0)
	{
		status = ft_pip(tree, h, e);
		// printf("done piping\n");
	}
	return (status);
}

// void	f(void)
// {
// 	system("leaks -q minishell");
// }

void	ft_hdoc_check(t_tree *tree)
{
	t_list_fd *tmp;

	if (!tree)
		return ;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3)
		{
			tmp->fd = ft_hdoc(ft_strdup(tmp->name), tmp->name);
			if (tmp ->fd == -1)
			{
				perror("minishell: ");
				return ;
			}
			// dup2(tmp->fd, 0);
		}
		tmp = tmp->next;
	}
}

void	ft_hdoc_handle(t_tree *tree)
{
	if (!tree)
		return ;
	// dprintf(2, "got out here\n");
	if (tree->left)
		ft_hdoc_handle(tree->left);
	if (tree->right)
		ft_hdoc_handle(tree->right);
	ft_hdoc_check(tree);
}

int	ft_node_lent(t_env *h)
{
	int	i;
	int count;
	
	i = 0;
	count = 0;
	while (h && h->key && h->key[i])
		i++;
	count += i;
	i = 0;
	while (h && h->value && h->value[i])
		i++;
	// dprintf(2, "entered here\n");
	i += 2;
	count += i;
	return (i);
}

char	*ft_node_joint(t_env *h)
{
	char *str;
	int node_lent;
	char *temp;
	
	if (h == NULL)
		dprintf(2, "entered here\n");
	node_lent = ft_node_lent(h);
	str = malloc(node_lent);
	if (!str)
		return (NULL);
	str = ft_strjoin(h->key, "=");
	if (!str)
		return (NULL);
	temp = str;
	str = ft_strjoin(str, h->value);
	if (!str)
		return (NULL);
	free (temp);
	return (str);
}

char **ft_env_str(t_env *h)
{
	char	**p;
	int		i;
	int		lst_lent;

	i = 0;
	if (h == NULL)
		return (NULL);
	lst_lent = ft_lstsize(h);
	p = malloc(sizeof(char *) * (lst_lent + 1));
	if (p == NULL)
		exit (1);
	while (i < lst_lent && h)
	{
		p[i] = ft_node_joint(h);
		h = h->next;
		i++;
	}
	p[i] = NULL;
	return (p);
}

int	main(int argc, char **argv, char **argev)
{
	char		*str;
	t_env		*env;
	t_tree		*tree;
	char 		**e;

	// atexit(f);
	((void)argc, (void)argv, inits_main(&env, &tree, argev));
	// signal(SIGINT, handle_signal);
	// signal(SIGQUIT, handle_signal);
	while (1)
	{
		str = readline("minishell$> ");
		if (!str)
			break ;
		else if (!*str || check_empty(str))
		{
			free(str);
			continue ;
		}
		add_history(str);
		quote_parse(&str);
		lexer_to_tree(str, &tree);
		tree_to_rediropen(tree);
		redirections_list_maker(&tree);
		// print_tree_visual(tree, 1, 1);
		// dprintf(2, "this is limiter: %s\n", tree->fd_list->name);
		quote_remove_lst(&tree);
		// if (variable_search(&tree) == 1) //TO EXPAND WITH IN EXECUTION THIS SEARCHES FOR VARIABLES AND THE NEXT ONE EXPANDS THEM
		// 	variable_expantion(&tree, &env);
		// if (variable_search_inlnkedlst(&tree) == 1)
		// 	variable_expantion_inlnkedlst(&tree, &env);
		if (has_wild_cards_comarr(&tree) == 1)
			handle_wildcards_in_cmdarr(&tree);
		if (has_wild_cards_fdlst(&tree) == 1)
			handle_wildcards_in_fdlst(&tree);
		// ambiguous_set(&tree);
		if (ambiguous_syntax_error(&tree) == 1)
			(write(2, "ambiguous redirect\n", 19));
		if (ambiguous_syntax_error(&tree) == 2)
			(write(2, "No such file or directory\n", 26));
		tree_empty_error(&tree);
		// printf("+++++++++++++++++++++++++++\n");
		// print_tree_visual(tree, 1, 1);
		// printf("+++++++++++++++++++++++++++\n");
		e = ft_env_str(env);
		ft_hdoc_handle(tree);
		ft_execute(tree, &env, e);
		// printf("***************************\n");
		// print_tree_visual(tree, 1, 1);
		// printf("***************************\n");
		lasfree(&tree);
		// free (e);
		// e = NULL;
	}
	free_env(&env);
}
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt
//((ls > file) && echo | la > file2) > file3

//((ls)>file2) > file
// "((ls)>file2) > file"
// >file4(>file5 ls>file>file2>file3 -la>file6)>file7>file8
//> file4 (>file5 ls>file>file2>file3 -la>file6)>file7>file8make
