/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/06/04 10:25:26 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	global_status;

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
	t_env *tmp;

	if (node == NULL)
		exit (1);
	*node = NULL;
	ft_lstadd_back(node, ft_lstnew("OLDPWD", NULL));
	ft_lstadd_back(node, ft_lstnew("PWD", getcwd(NULL, 0)));
	ft_lstadd_back(node, ft_lstnew("SHLVL", "1"));
	ft_lstadd_back(node, ft_lstnew("PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
	ft_lstadd_back(node, ft_lstnew("1PWD", NULL));
	tmp = ft_check(*node, "OLDPWD");
	if (tmp)
		tmp->active = 1;
	tmp = ft_check(*node, "1PWD");
	if (tmp)
	{
		tmp->active = 1;
		tmp->h = 1;
	}
	tmp = *node;
	while (tmp->next)
	{
		tmp->h = 0;
		tmp = tmp->next; 
	}
	// tmp->h = 1;
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
		head->h = 0;
		head->active = 0;
		tmp = head;
		i++;
		while (argev[i])
		{
			env_fill_helper(&new_node, &i, &j, argev);
			new_node->next = NULL;
			tmp->next = new_node;
			tmp = new_node;
			tmp->active = 0;
			tmp->h = 0;
			i++;
		}
		tmp = ft_check(head, "OLDPWD");
		if (tmp)
		{
			tmp->active = 1;
			(free(tmp->value), tmp->value = NULL);
		}
		ft_lstadd_back(&head, ft_lstnew("1PWD", NULL));
		tmp = ft_check(head, "1PWD");
		if (tmp)
		{
			tmp->active = 1;
			tmp->h = 1;
		}
	}
	return (head);
}

void	free_array(char **s)
{
	int i;
	
	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		free (s[i]);
		i++;
	}
	free (s);
}

void	ft_exec(t_tree *tree, t_env *h, char **e)
{
	char	*temp;
	t_env *path;
	char *env;
	int	flag;
	// t_pip *value;

	flag = 0;
	path = ft_check(h, "PATH");
	if (path == NULL || path->value == NULL || check_empty(path->value) == 1)
		env = NULL;
	else
		env = path->value; 
	temp = tree->command_arr[0];
	tree->command_arr[0] =  ft_cmd_check(env, tree->command_arr[0]);
	// free (temp);
	if (tree->command_arr[0] == NULL || (tree->var && tree->command_arr[1]))
	{
		tree->command_arr[0] = temp;
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, tree->command_arr[0]);
		ft_putstr_fd(2, ": command not found\n");
		free_array(tree->command_arr);
		exit (127);
	}
	// dprintf(2, "this is command array: %s\n", tree->command_arr[0]);
	// ft_sig();
	execve(tree->command_arr[0], tree->command_arr, e);
	exit (1);
}

void	ft_execute(t_tree *tree, t_env **h, char **e, int *check);

int	ft_pip(t_tree *tree, t_env **h, char **e, int *check)
{
	int	fd[2];
	int id1 = 0;
	int id2 = 0;
	int	status;
	int	status1;
	
	status = 0;
	status1 = 0;
	if (pipe(fd) == -1)
		perror("minishell: pipe: ");
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	id1 = fork();
	if (id1 == -1)
	{
		perror("minishell: fork: ");
		exit (1);
	}
	if (id1 == 0)
	{
		signal(SIGINT, SIG_DFL);
		*check = 1;
		dup2(fd[1], 1);
		close (fd[1]);
		close (fd[0]);
		ft_execute(tree->left, h, e, check);
		exit (tree->status);
	}
	// display_terminal_control_chars();
	id2 = fork();
	if (id2 == -1)
	{
		perror("minishell: fork: ");
		exit (1);
	}
	if (id2 == 0)
	{
		signal(SIGINT, SIG_DFL);
		*check = 1;
		dup2(fd[0], 0);
		close (fd[0]);
		close (fd[1]);
		ft_execute(tree->right, h, e, check);
		exit(tree->status);
	}
	// signal(SIGINT, SIG_IGN);
	// signal(SIGQUIT, SIG_IGN);
	// waitpid(id1, NULL, 0);
	close (fd[0]);
	close (fd[1]);
	waitpid(id1, &status1, 0);
	waitpid(id2, &(status), 0);
	if (WIFEXITED(status) && WIFEXITED(status1))
	{
		// dprintf(2, "this is it\n");
		status = WEXITSTATUS(status);
		// dprintf(2, "this is status\n");
	}
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		if (status == 2)
		{
			global_status = SIGINT;
		}
		else if (status == 3)
		{
			global_status = SIGQUIT;
			// ft_putstr_fd(1, "Quit: 3\n");
		}
		status += 128;
	}
	if (WIFSIGNALED(status1))
	{
		// dprintf(2, "enterd herepip\n");
		status1 = WTERMSIG(status1);
		if (status1 == 2)
		{
			global_status = SIGINT;
		}
		else if (status1 == 3)
		{
			global_status = SIGQUIT;
			// ft_putstr_fd(1, "Quit: 3\n");
		}
	}
	// dprintf(2, "this is the status: %d\n", WEXITSTATUS(status));
	return (status);
}

int	cmd_check(t_tree *tree)
{
	if (!tree->command_arr || !tree->command_arr[0])
		return (1);
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		return (0);
	return (1);
}

int	ft_cmd_exec(t_tree *tree, t_env **h)
{
	int status;

	status = 0;
	// flag = 0;
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		status = ft_cd(tree->command_arr, *h);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		status = ft_echo(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		status = ft_env(*h);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		ft_exit(tree->command_arr, h);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		status = ft_export(tree->command_arr, *h, tree);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		status = ft_pwd(*h);
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		ft_unset(h, tree->command_arr);
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

void	ft_hdoc_signal(int sig)
{
	(void)sig;

	// break ;
}

int	ft_hdoc(char *limiter, char *name, int fd)
{
	char	*str;
	// int		fd;
	char	*tmp;
	// int		fd1;

	// unlink(name);
	// name = ft_name_check(name);
	// fd = open(name, O_RDWR | O_CREAT, 0777);
	// if (fd == -1)
	// 	return (1);
	// fd1 = open(name, O_RDONLY, 0777);
	// if (fd1 == -1)
	// 	return (1);
	// unlink(name);
	// signal(SIGINT, ft_hdoc_signal);
	// signal(SIGQUIT, SIG_IGN);
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
	// dprintf(2, "this is fd: %d\n", fd1);
	// return (fd1);
}

int	ft_exec_redir(t_tree *tree, t_env **h, char **env)
{
	while (tree->fd_list != NULL && tree->ambiguous == 0)
	{
		if (ft_redir_check(tree->fd_list->redir) == 3)
		{
			dup2(tree->fd_list->fd1, 0);
			close (tree->fd_list->fd1);
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
			close (tree->fd_list->fd);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 1)
		{
			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return (-1);//must change not the same exit status
			}
			dup2(tree->fd_list->fd, 0);
			close (tree->fd_list->fd);
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
			close (tree->fd_list->fd);
		}
		tree->fd_list = tree->fd_list->next;
	}
	if (tree->ambiguous == 1)
		return (1);
	if (tree->command_arr[0] && check_empty(tree->command_arr[0]) == 0)
	{
		// dprintf(2, "entered here\n");
		ft_exec(tree, *h, env);
	}
	exit (0);
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
			close(tree->fd_list->fd);
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
			close(tree->fd_list->fd);
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
			close(tree->fd_list->fd);
		}
		tree->fd_list = tree->fd_list->next;
	}
	if (tree->ambiguous == 1)
		return (1);
	status = ft_cmd_exec(tree, h);
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	close (org_stdin);
	close (org_stdout);
	return (status);
}

void	ft_word_handle(t_tree *tree, t_env **h, char **e, int *check);

// int	ft_variable(t_tree *tree, t_env **h, char **e)
// {
// 	if (cmd_check(tree) == 1)
// 		ft_word_handle(tree, h, e);
// 	else if (cmd_check(tree) == 0)
// 		tree->status = ft_cmd_exec(tree, h);
// 	else
// 	{
// 		ft_putstr_fd(2, "minishell: ");
// 		ft_putstr_fd(2, tree->command_arr[0]);
// 		ft_putstr_fd(2, ": command not found\n");
// 	}
// 	return (tree->status);
// }

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
		// dprintf(2, "entered in here\n");
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
	// int	fd[2];
	// int id1 = 0;
	// int id2 = 0;
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
		ft_execute(tree->left, h, e, check);
	if (tree->right)
		ft_execute(tree->right, h, e, check);
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	return (0);
}

void	ft_is_dir(char *s)
{
	// int	i;
	struct stat buf;

	// i = 0;
	// while (tree->command_arr[i])
	// {
		// dprintf(2, "this is entered here\n");
		stat(s, &buf);
		if (S_ISDIR(buf.st_mode) != 0)
		{
			ft_putstr_fd(2, "minishell: ");
			ft_putstr_fd(2, s);
			ft_putstr_fd(2, ": is a directory\n");
			exit (126);
		}
	// 	i++;
	// }
	// return (-1);
}

void	ft_new_handler(int sig)
{
	(void)sig;
	printf("\n");
	// hide_terminal_control_chars();
	// rl_on_new_line();
	// rl_replace_line("", 0);
}

void	ft_new_handler_pip(int sig)
{
	(void)sig;
	// printf("\n");
	hide_terminal_control_chars();
	rl_on_new_line();
	rl_replace_line("", 0);
}
void	ft_word_handle(t_tree *tree, t_env **h, char **e, int *check)
{
	int id;

	display_terminal_control_chars();
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		// ft_sig();
		ft_exec(tree, *h, e);
	}
	else if (id < 0)
		perror("fork");
	if (*check == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	waitpid(id, &(tree->status), 0);
	if (WIFEXITED(tree->status))
		tree->status = WEXITSTATUS(tree->status);
	else if (WIFSIGNALED(tree->status))
	{
		tree->status = WTERMSIG(tree->status);
		if (tree->status == 2)
		{
			global_status = SIGINT;
		}
		else if (tree->status == 3)
		{
			global_status = SIGQUIT;
			// ft_putstr_fd(1, "Quit: 3\n");
		}
		tree->status += 128;
	}
	// signal(SIGINT, ft_new_handler);
}

void	ft_execute(t_tree *tree, t_env **h, char **e, int *check)
{
	int id;
	// int	check;
	// int	stdout;
	
	// stdout = 0;
	// if (isatty(1) || isatty(0))
	// 	stdout = dup(1);
	// check = 0;
	// dprintf(2, "this is check: %d\n", *check);
	if (!tree)
	{
		free_env(h);
		exit (1);
	}
	// if (!(*h))
	// {
	// 	dprintf(2, "env is NULL\n");
	// }
	// print_tree_visual(tree, 1, 1);
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections == NULL) || (ft_strcmp("WORD", tree->type) == 0 && cmd_check(tree) == 0))
	{
		reset_vars(&tree, h);
		// dprintf(2, "this is the OLDPWD EXPAnd: %s\n", tree->command_arr[1]);
		tree->status = ft_cmd_exec(tree, h);
	}
	if (ft_strcmp("PARENTHASIS", tree->type) == 0)
		tree->status = ft_parenthasis(tree, h, e);
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections != NULL) || (cmd_check(tree) == 0 && ft_strcmp("REDIRECTION", tree->type) == 0))
	{
		if (variable_search_inlnkedlst(&tree) == 1)
			variable_expantion_inlnkedlst(&tree, h);
		ambiguous_set(&tree);
		quote_remove_lst(&tree);
		if (ambiguous_syntax_error(&tree, h) == 1)
			(write(2, "ambiguous redirect\n", 19));
		if (ambiguous_syntax_error(&tree, h) == 2)
			(write(2, "No such file or directory\n", 26));
		tree->status = ft_cmd_redir(tree, h);
	}
	if (ft_strcmp("OPERATION_&&", tree->type) == 0)
	{
		ft_execute(tree->left, h, e, check);
		if (tree->status == 0)
			ft_execute(tree->right, h, e, check);
	}
	if (ft_strcmp("OPERATION_||", tree->type) == 0)
	{
		ft_execute(tree->left, h, e, check);
		if (tree->status != 0)
			ft_execute(tree->right, h, e, check);
	}
	if (ft_strcmp("VARIABLE", tree->type) == 0)
	{
		reset_vars(&tree, h);
		// print_tree_visual(tree, 1, 1);
		// tree->status = ft_variable(tree, h, e);
	}
	if (tree->redirections == NULL && ft_strcmp("WORD", tree->type) == 0 && cmd_check(tree) == 1)
	{
		reset_vars(&tree, h);
		ft_word_handle(tree, h, e, check);
	}
	if ((tree->redirections != NULL && ft_strcmp("WORD", tree->type) == 0) || (ft_strcmp("REDIRECTION", tree->type) == 0 && cmd_check(tree) == 1))
	{
		display_terminal_control_chars();
		id = fork();
		if (id == 0)
		{
			if (variable_search_inlnkedlst(&tree) == 1)
				variable_expantion_inlnkedlst(&tree, h);
			ambiguous_set(&tree);
			quote_remove_lst(&tree);
			if (ambiguous_syntax_error(&tree, h) == 1)
				(write(2, "ambiguous redirect\n", 19));
			if (ambiguous_syntax_error(&tree, h) == 2)
				(write(2, "No such file or directory\n", 26));
			ft_exec_redir(tree, h, e);
		}
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(id, &(tree->status), 0);
		if (WIFEXITED(tree->status))
			tree->status = WEXITSTATUS(tree->status);
		else if (WIFSIGNALED(tree->status))
		{
			tree->status = WTERMSIG(tree->status);
			if (tree->status == 2)
				ft_putstr_fd(1, "\n");
			else if (tree->status == 3)
				ft_putstr_fd(1, "Quit: 3\n");
			tree->status += 128;
		}
		// tree->status = WEXITSTATUS(tree->status);
	}
	if (ft_strcmp("PIPE", tree->type) == 0)
	{
		// display_terminal_control_chars();
		// signal(SIGINT, ft_new_handler);
		// signal(SIGQUIT, ft_new_handler);
		tree->status = ft_pip(tree, h, e, check);
	}
}

int	ft_hdoc_first(t_tree *tree)
{
	int		fd;
	char	*temp;
	int		fd1;
	t_list_fd *tmp;

	tmp = tree->fd_list;
	unlink(tmp->name);
	tmp->name = ft_name_check(tmp->name);
	tmp->fd = open(tmp->name, O_RDWR | O_CREAT, 0777);
	if (tmp->fd == -1)
		return (1);
	tmp->fd1 = open(tmp->name, O_RDONLY, 0777);
	if (fd1 == -1)
		return (1);
	unlink(tmp->name);
	// return (fd1);
	return (0);
}

void	 ft_hdoc_check(t_tree *tree, int *sig_flag)
{
	t_list_fd *tmp;
	int		id;

	if (!tree)
		return ;
	tmp = tree->fd_list;
	// if (!tmp)
	// 	return;
	// tmp->sig_flag = 1;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3 && (*sig_flag) == 1)
		{
			if (!ft_hdoc_first(tree))
			{
				id = fork();
				if (id == 0)
				{
					signal(SIGINT, SIG_DFL);
					ft_hdoc(ft_strdup(tmp->name), tmp->name, tmp->fd);
				}
				else if (id < 0)
					perror("fork");
				signal(SIGINT, SIG_IGN); 
				waitpid(id, &(tree->status), 0);
				// tmp->fd = tmp->fd1;
				if (tmp ->fd1 == -1)
				{
					perror("minishell: ");
					return ;
				}
				if (WIFSIGNALED(tree->status))
				{
					tree->status = WTERMSIG(tree->status);
					if (tree->status == 2)
						ft_putstr_fd(1, "\n");
					// tree->status = 1;
					*sig_flag = 0;
					// puts("it entered here");
				}
				
			}
			// dup2(tmp->fd1, 0);
		}
		tmp = tmp->next;
	}
	// return (tree->sig_flag);
}

void	ft_hdoc_handle(t_tree *tree, int *sig_flag)
{
	if (!tree)
		return ;
	// dprintf(2, "got out here\n");
	if (tree->left)
		ft_hdoc_handle(tree->left, sig_flag);
	if (tree->right)
		ft_hdoc_handle(tree->right, sig_flag);
	ft_hdoc_check(tree, sig_flag);
}

// int	ft_node_lent(t_env *h)
// {
// 	int	i;
// 	int count;
	
// 	i = 0;
// 	count = 0;
// 	while (h && h->key && h->key[i])
// 		i++;
// 	count += i;
// 	i = 0;
// 	while (h && h->value && h->value[i])
// 		i++;
// 	// dprintf(2, "entered here\n");
// 	i += 2;
// 	count += i;
// 	return (i);
// }

char	*ft_node_joint(t_env *h)
{
	char *str;
	// int node_lent;
	char *temp;
	
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
	{
		free_env(&h);
		exit (1);
	}
	while (i < lst_lent && h)
	{
		p[i] = ft_node_joint(h);
		h = h->next;
		i++;
	}
	p[i] = NULL;
	return (p);
}

void	var_set(t_tree **tree)
{
	int	i;

	if ((*tree) && (*tree)->left)
		var_set(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		var_set(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		i = 0;
		while ((*tree)->command_arr[i])
		{
			if (variable_search_instr((*tree)->command_arr[i]))
				(*tree)->var = 1;
			i++;
		}
	}
}

int	hidenword(char *str, char *str2)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	if (!str || !str2)
		return (0);
	while (str2[i])
	{
		while (str[j])
		{
			if (str2[i] == str[j])
				break ;
			j++;
		}
		if (!str[j])
			break ;
		if (str[j] == str2[i])
			count++;
		i++;
	}
	if (count == (int)ft_strlen(str2))
		return (1);
	return (0);
}

int	isnt_valid(char *str, int flag)
{
	int	i;

	i = 0;
	if (flag == 0)
	{
		while (str[i])
		{
			if (str[i] == '%' || str[i] == '$' || str[i] == '*' || (str[i] == '+' && str[i + 1]))
				return (1);
			i++;
		}
	}
	else if (flag == 1)
	{
		while (str[i])
		{
			if ((str[i] == '%') || (
					str[i] == '"') || (
					str[i] == '\'') || (
					str[i] == '$') || (
					str[i] == '*') || (
					str[i] == '+' && str[i + 1]) || (
					str[0] >= '0' && str[0] <= '9'))
				return (1);
			i++;
		}
	}
	return (0);
}

void	check_full_quotes(char *str, int *is_fully_quoted, char *quote_char)
{
	int	len;

	*is_fully_quoted = 0;
	*quote_char = 0;
	len = ft_strlen(str);
	if (len >= 2 && (str[0] == '"' || str[0] == '\'') && str[len - 1] == str[0])
	{
		*is_fully_quoted = 1;
		*quote_char = str[0];
	}
}

// void	reset_command_arr(t_tree **tree)
// {
// 	int		i;
// 	int		j;
// 	int		count;
// 	int		flag;
// 	int		in_quotes;
// 	int		size_lst;
// 	int		is_fully_quoted;
// 	char	quote_type;
// 	char	*old_cmd;
// 	char	*prev_command_arr;
// 	char	**new_cmd_arr;
// 	char	quote_char;
// 	t_list	*head;
// 	t_list	*new_node;
// 	t_list	*tmp;

// 	if ((*tree) && (*tree)->left)
// 		reset_command_arr(&(*tree)->left);
// 	if ((*tree) && (*tree)->right)
// 		reset_command_arr(&(*tree)->right);
// 	if ((*tree) && (*tree)->command_arr)
// 	{
// 		if ((!ft_strcmp((*tree)->command_arr[0], "export"
// 				) || variable_search_instr((*tree
// 					)->command_arr[0])) || hidenword((
// 					*tree)->command_arr[0], "export"))
// 		{
// 			i = 0;
// 			while ((*tree)->command_arr[i])
// 				i++;
// 			(*tree)->command_arr_expanded = malloc(sizeof(char **) * (i + 1));
// 			if (!(*tree)->command_arr_expanded)
// 				return ;
// 			i = 0;
// 			while ((*tree)->command_arr[i])
// 			{
// 				check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
				
// 				if (is_fully_quoted)
// 				{
// 					new_cmd_arr = malloc(sizeof(char *) * 2);
// 					if (!new_cmd_arr)
// 						return ;
// 					new_cmd_arr[0] = ft_strdup((*tree)->command_arr[i]);
// 					new_cmd_arr[1] = NULL;
// 					(*tree)->command_arr_expanded[i] = new_cmd_arr;
// 					i++;
// 					continue ;
// 				}
				
// 				old_cmd = ft_strdup((*tree)->command_arr[i]);
// 				if (!old_cmd)
// 				{
// 					while (i > 0)
// 					{
// 						i--;
// 						if ((*tree)->command_arr_expanded[i])
// 						{
// 							j = 0;
// 							while ((*tree)->command_arr_expanded[i][j])
// 							{
// 								free((*tree)->command_arr_expanded[i][j]);
// 								j++;
// 							}
// 							free((*tree)->command_arr_expanded[i]);
// 						}
// 					}
// 					free((*tree)->command_arr_expanded);
// 					return ;
// 				}
// 				head = NULL;
// 				if (ft_strchr(old_cmd, '='))
// 				{
// 					j = 0;
// 					while (old_cmd[j] && old_cmd[j] != '=')
// 						j++;
// 					if (old_cmd[j] == '=')
// 					{
// 						head = malloc(sizeof(t_list));
// 						if (!head)
// 						{
// 							free(old_cmd);
// 							return ;
// 						}
// 						head->data = ft_substr(old_cmd, 0, j);
// 						head->token = NULL;
// 						head->next = NULL;
// 						new_node = malloc(sizeof(t_list));
// 						if (!new_node)
// 						{
// 							free(old_cmd);
// 							free_list(&head);
// 							return ;
// 						}
// 						new_node->data = ft_strdup("=");
// 						new_node->token = NULL;
// 						new_node->next = NULL;
// 						head->next = new_node;
// 						tmp = new_node;
// 						new_node = malloc(sizeof(t_list));
// 						if (!new_node)
// 						{
// 							free(old_cmd);
// 							free_list(&head);
// 							return ;
// 						}
// 						new_node->data = ft_substr(
// 								old_cmd, j + 1, ft_strlen(old_cmd) - (j + 1));
// 						new_node->token = NULL;
// 						new_node->next = NULL;
// 						tmp->next = new_node;
// 						free(old_cmd);
// 						old_cmd = NULL;
// 					}
// 				}
// 				if (head)
// 				{
// 					tmp = head;
// 					while (tmp)
// 					{
// 						printf("data = %s\n", tmp->data);
// 						printf("ll\n");
// 						tmp = tmp->next;
// 					}
// 					tmp = head;
// 					char *tmp_char;
// 					while (tmp)
// 					{
// 						i = 0;
// 						in_quotes = 0;
// 						while (tmp->data[i])
// 						{
// 							if ((tmp->data[i] == '\'' || tmp->data[i] == '\"') && !in_quotes)
// 							{
// 								in_quotes = 1;
// 								quote_type = tmp->data[i];
// 							}
// 							else if (in_quotes && tmp->data[i] == quote_type)
// 								in_quotes = 0;
// 							i++;
// 						}
// 						if (in_quotes)
// 						{
// 							if (tmp->next && ft_strchr(tmp->next->data, '='))
// 							{
// 								t_list *tmp2;
// 								tmp_char = tmp->data;
// 								if (tmp->next->next && tmp->next->next->data)
// 								{
// 									tmp->data = ft_strjoin_three(tmp->data, tmp->next->data, tmp->next->next->data);
// 									free(tmp_char);
// 									tmp2 = tmp->next;
// 									t_list *tmp3 = tmp->next->next;
// 									if (tmp->next->next->next)
// 									{
// 										tmp->next = tmp->next->next->next;
// 										if (tmp->next)
// 											tmp->next->prev = tmp;
// 									}
// 									else
// 									{
// 										tmp->next = NULL;
// 									}
// 									free(tmp2->data);
// 									free(tmp3->data);
// 									free(tmp2);
// 									free(tmp3);
// 								}
// 								else if (tmp->next)
// 								{
// 									char *joined = ft_strjoin(tmp->data, tmp->next->data);
// 									free(tmp_char);
// 									tmp->data = joined;
// 									tmp2 = tmp->next;
// 									tmp->next = tmp->next->next;
// 									if (tmp->next)
// 										tmp->next->prev = tmp;
// 									free(tmp2->data);
// 									free(tmp2);
// 								}
// 							}
// 						}
// 						tmp = tmp->next;
// 					}
// 					i = 0;
// 					in_quotes = 0;
// 					quote_type = 0;
// 					tmp = head;
// 					while (tmp)
// 					{
// 						printf("data = %s\n", tmp->data);
// 						tmp = tmp->next;
// 					}
// 					size_lst = lst_size(&head);
// 					new_cmd_arr = malloc(sizeof(char *) * (size_lst + 1));
// 					if (!new_cmd_arr)
// 					{
// 						free_list(&head);
// 						if (old_cmd)
// 							free(old_cmd);
// 						return ;
// 					}
// 					j = 0;
// 					tmp = head;
// 					while (tmp)
// 					{
// 						new_cmd_arr[j] = ft_strdup(tmp->data);
// 						j++;
// 						tmp = tmp->next;
// 					}
// 					new_cmd_arr[j] = NULL;
// 					free_list(&head);
// 					(*tree)->command_arr_expanded[i] = new_cmd_arr;
// 				}
// 				else
// 				{
// 					new_cmd_arr = malloc(sizeof(char *) * 2);
// 					if (!new_cmd_arr)
// 					{
// 						if (old_cmd)
// 							free(old_cmd);
// 						return ;
// 					}
// 					new_cmd_arr[0] = old_cmd;
// 					new_cmd_arr[1] = NULL;
// 					(*tree)->command_arr_expanded[i] = new_cmd_arr;
// 					old_cmd = NULL;
// 				}
// 				if (old_cmd)
// 					free(old_cmd);
// 				i++;
// 			}
// 			(*tree)->command_arr_expanded[i] = NULL;
// 			i = 0;
// 			j = 0;
// 			count = 0;
// 			while ((*tree)->command_arr_expanded[i])
// 			{
// 				j = 0;
// 				while ((*tree)->command_arr_expanded[i][j])
// 				{
// 					count++;
// 					j++;
// 				}
// 				i++;
// 			}
// 			new_cmd_arr = malloc(sizeof(char *) * (count + 1));
// 			if (!new_cmd_arr)
// 				return ;
				
// 			i = 0;
// 			j = 0;
// 			count = 0;
// 			while ((*tree)->command_arr_expanded[i])
// 			{
// 				j = 0;
// 				while ((*tree)->command_arr_expanded[i][j])
// 				{
// 					new_cmd_arr[count] = ft_strdup((
// 								*tree)->command_arr_expanded[i][j]);
// 					count++;
// 					j++;
// 				}
// 				i++;
// 			}
// 			new_cmd_arr[count] = NULL;
// 			i = 0;
// 			while ((*tree)->command_arr[i])
// 			{
// 				free((*tree)->command_arr[i]);
// 				i++;
// 			}
// 			free((*tree)->command_arr);
// 			(*tree)->command_arr = new_cmd_arr;
// 			i = 0;
// 			while ((*tree)->command_arr[i])
// 				i++;
// 			(*tree)->split = malloc(sizeof(t_split *) * (i + 1));
// 			i = 0;
// 			while ((*tree)->command_arr[i])
// 			{
// 				(*tree)->split[i] = malloc(sizeof(t_split));
// 				(*tree)->split[i]->arr_index = 0;
// 				(*tree)->split[i]->split_flag = 0;
// 				i++;
// 			}
// 			i = 0;
// 			in_quotes = 0;
// 			flag = 0;
// 			prev_command_arr = NULL;
// 			while ((*tree)->command_arr[i])
// 			{
// 				check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
				
// 				if (is_fully_quoted)
// 				{
// 					(*tree)->split[i]->arr_index = i;
// 					(*tree)->split[i]->split_flag = 0;
// 					prev_command_arr = (*tree)->command_arr[i];
// 					i++;
// 					continue ;
// 				}
				
// 				j = 0;
// 				in_quotes = 0;
// 				flag = 0;
// 				while ((*tree)->command_arr[i][j])
// 				{
// 					if (!in_quotes && ((*tree
// 							)->command_arr[i][j] == '"' || (
// 							*tree)->command_arr[i][j] == '\''))
// 					{
// 						in_quotes = 1;
// 						quote_type = (*tree)->command_arr[i][j];
// 					}
// 					else if (in_quotes && (*tree)->command_arr[i][j] == '$')
// 						flag = 1;
// 					else if (in_quotes && (*tree
// 						)->command_arr[i][j] == quote_type)
// 						in_quotes = 0;
// 					j++;
// 				}
// 				(*tree)->split[i]->arr_index = i;
// 				if ((variable_search_instr((*tree)->command_arr[i]
// 						) && prev_command_arr && !ft_strcmp(
// 							prev_command_arr, "=") && (i - 2 < 0 || (
// 								i - 2 >= 0 && (!variable_search_instr((
// 											*tree)->command_arr[i - 2]
// 									) && !isnt_valid((
// 											*tree)->command_arr[i - 2], 1)
// 								)))) || (
// 						variable_search_instr((*tree)->command_arr[i]
// 						) && flag == 1) || (!ft_strchr((
// 								*tree)->command_arr[i], '$') && !isnt_valid(
// 							(*tree)->command_arr[i], 0)))
// 					(*tree)->split[i]->split_flag = 0;
// 				else
// 					(*tree)->split[i]->split_flag = 1;
// 				prev_command_arr = (*tree)->command_arr[i];
// 				flag = 0;
// 				i++;
// 			}
// 			(*tree)->split[i] = NULL;
// 			prev_command_arr = NULL;
// 			if (!ft_strcmp((*tree)->command_arr[0], "export"
// 				) || variable_search_instr((*tree)->command_arr[0]))
// 			{
// 				i = 1;
// 				in_quotes = 0;
// 				flag = 0;
// 				while ((*tree)->command_arr[i])
// 				{
// 					check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
					
// 					if (is_fully_quoted)
// 					{
// 						(*tree)->split[i]->arr_index = i;
// 						(*tree)->split[i]->split_flag = 0;
// 						prev_command_arr = (*tree)->command_arr[i];
// 						i++;
// 						continue;
// 					}
					
// 					j = 0;
// 					in_quotes = 0;
// 					flag = 0;
// 					while ((*tree)->command_arr[i][j])
// 					{
// 						if (!in_quotes && ((*tree
// 								)->command_arr[i][j] == '"' || (
// 								*tree)->command_arr[i][j] == '\''))
// 						{
// 							in_quotes = 1;
// 							quote_type = (*tree)->command_arr[i][j];
// 						}
// 						else if (in_quotes && (*tree)->command_arr[i][j] == '$')
// 							flag = 1;
// 						else if (in_quotes && (*tree
// 							)->command_arr[i][j] == quote_type)
// 							in_quotes = 0;
// 						j++;
// 					}
// 					(*tree)->split[i]->arr_index = i;
// 					if ((variable_search_instr((*tree)->command_arr[i]
// 							) && prev_command_arr && !ft_strcmp(
// 								prev_command_arr, "=") && (i - 2 < 0 || (
// 									i - 2 >= 0 && (!variable_search_instr((
// 												*tree)->command_arr[i - 2]
// 										) && !isnt_valid((
// 												*tree)->command_arr[i - 2], 1)
// 									)))) || (
// 							variable_search_instr((*tree)->command_arr[i]
// 							) && flag == 1) || (!ft_strchr((
// 									*tree)->command_arr[i], '$') && !isnt_valid(
// 								(*tree)->command_arr[i], 0)))
// 						(*tree)->split[i]->split_flag = 0;
// 					else
// 						(*tree)->split[i]->split_flag = 1;
// 					prev_command_arr = (*tree)->command_arr[i];
// 					flag = 0;
// 					i++;
// 				}
// 				(*tree)->split[i] = NULL;
// 				if (variable_search_instr((*tree)->command_arr[0]))
// 				{
// 					i = 0;
// 					while ((*tree)->split[i])
// 					{
// 						check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
// 						if (!is_fully_quoted)
// 							(*tree)->split[i]->split_flag = 1;
// 						i++;
// 					}
// 				}
// 			}
// 			else if (hidenword((*tree)->command_arr[0], "export"))
// 			{
// 				i = 1;
// 				in_quotes = 0;
// 				flag = 0;
// 				printf("**********%d\n", (*tree)->command_arr[i][0]);
// 				printf("**********%s\n", (*tree)->command_arr[i]);
// 				while ((*tree)->command_arr[i])
// 				{
// 					check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
					
// 					if (is_fully_quoted)
// 					{
// 						(*tree)->split[i]->arr_index = i;
// 						(*tree)->split[i]->split_flag = 0;
// 						i++;
// 						continue ;
// 					}
					
// 					j = 0;
// 					in_quotes = 0;
// 					flag = 0;
// 					while ((*tree)->command_arr[i][j])
// 					{
// 						if (!in_quotes && ((*tree
// 								)->command_arr[i][j] == '"' || (
// 								*tree)->command_arr[i][j] == '\''))
// 						{
// 							in_quotes = 1;
// 							quote_type = (*tree)->command_arr[i][j];
// 						}
// 						else if (in_quotes && (*tree)->command_arr[i][j] == '$')
// 							flag = 1;
// 						else if (in_quotes && (*tree
// 							)->command_arr[i][j] == quote_type)
// 							in_quotes = 0;
// 						j++;
// 					}
// 					(*tree)->split[i]->arr_index = i;
// 					if (((variable_search_instr((*tree)->command_arr[i]
// 								) && flag == 1)))
// 						(*tree)->split[i]->split_flag = 0;
// 					else
// 						(*tree)->split[i]->split_flag = 1;
// 					i++;
// 				}
// 				(*tree)->split[i] = NULL;
// 				i = 1;
// 				while ((*tree)->command_arr[i])
// 				{
// 					check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
// 					if (!is_fully_quoted && !variable_search_instr((*tree)->command_arr[i]))
// 						(*tree)->split[i]->split_flag = 0;
// 					i++;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			i = 0;
// 			while ((*tree)->command_arr[i])
// 				i++;
// 			j = i;
// 			(*tree)->split = malloc(sizeof(t_split *) * (i + 1));
// 			i = 0;
// 			(*tree)->split[j] = NULL;
// 			while ((*tree)->split[i])
// 			{
// 				(*tree)->split[i] = malloc(sizeof(t_split));
// 				(*tree)->split[i]->arr_index = i;
// 				(*tree)->split[i]->split_flag = 1;
// 				i++;
// 			}
// 		}
// 	}
// }

void	reset_command_arr(t_tree **tree)
{
	int		i; 
	int		j;
	int		count;
	int		flag; 
	int		in_quotes; 
	int		size_lst;
	int		is_fully_quoted;
	char	quote_type; 
	char	*old_cmd;
	char	*prev_command_arr;
	char	**new_cmd_arr;
	char	quote_char;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if (!tree || !*tree) 
		return;
	if ((*tree)->left)
		reset_command_arr(&(*tree)->left);
	if ((*tree)->right)
		reset_command_arr(&(*tree)->right);
	if ((*tree)->command_arr)
	{
		if ((!ft_strcmp((*tree)->command_arr[0], "export")) || \
			variable_search_instr((*tree)->command_arr[0]) || \
			hidenword((*tree)->command_arr[0], "export"))
		{
			i = 0;
			while ((*tree)->command_arr[i]) 
				i++;
			(*tree)->command_arr_expanded = malloc(sizeof(char **) * (i + 1));
			if (!(*tree)->command_arr_expanded)
				return ; 

			i = 0; 
			while ((*tree)->command_arr[i])
			{
				check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
				if (is_fully_quoted)
				{
					new_cmd_arr = malloc(sizeof(char *) * 2);
					if (!new_cmd_arr)
						return ; 
					new_cmd_arr[0] = ft_strdup((*tree)->command_arr[i]);
					new_cmd_arr[1] = NULL;
					(*tree)->command_arr_expanded[i] = new_cmd_arr;
					i++;
					continue ;
				}
				old_cmd = ft_strdup((*tree)->command_arr[i]);
				if (!old_cmd)
				{
					
					int k = 0;
					while (k < i)
					{
						if ((*tree)->command_arr_expanded[k])
						{
							int l = 0;
							while((*tree)->command_arr_expanded[k][l])
							{
								free((*tree)->command_arr_expanded[k][l]);
								l++;
							}
							free((*tree)->command_arr_expanded[k]);
						}
						k++;
					}
					free((*tree)->command_arr_expanded);
					(*tree)->command_arr_expanded = NULL; 
					return ;
				}
				head = NULL;
				if (ft_strchr(old_cmd, '='))
				{
					j = 0;
					while (old_cmd[j] && old_cmd[j] != '=')
						j++;
					if (old_cmd[j] == '=')
					{
						head = malloc(sizeof(t_list));
						if (!head) 
						{
							free(old_cmd);
							return ;
						}
						head->data = ft_substr(old_cmd, 0, j);
						head->token = NULL;
						head->next = NULL; 
						new_node = malloc(sizeof(t_list));
						if (!new_node)
						{
							free(old_cmd);
							free_list(&head);
							return ;
						}
						new_node->data = ft_strdup("=");
						new_node->token = NULL; new_node->next = NULL; 
						head->next = new_node;
						tmp = new_node;

						new_node = malloc(sizeof(t_list));
						if (!new_node)
						{
							free(old_cmd);
							free_list(&head);
							return ;
						}
						new_node->data = ft_substr(old_cmd, j + 1, ft_strlen(old_cmd) - (j + 1));
						new_node->token = NULL; new_node->next = NULL; 
						tmp->next = new_node;
						free(old_cmd);
						old_cmd = NULL;
					}
				}
				if (head)
				{
					tmp = head; 
					char *tmp_char_ptr;
					while (tmp)
					{
						int char_idx = 0; 
						in_quotes = 0; 
						if (tmp->data) 
						{
							while (tmp->data[char_idx])
							{
								if ((tmp->data[char_idx] == '\'' || tmp->data[char_idx] == '\"') && !in_quotes)
								{
									in_quotes = 1;
									quote_type = tmp->data[char_idx];
								}
								else if (in_quotes && tmp->data[char_idx] == quote_type)
								{
									in_quotes = 0;
								}
								char_idx++;
							}
						}

						if (in_quotes) 
						{
							if (tmp->next && tmp->next->data && ft_strchr(tmp->next->data, '='))
							{
								t_list *tmp2 = NULL;
								t_list *tmp3 = NULL;
								tmp_char_ptr = tmp->data; 

								if (tmp->next->next && tmp->next->next->data)
								{
									tmp->data = ft_strjoin_three(tmp_char_ptr, tmp->next->data, tmp->next->next->data);
									free(tmp_char_ptr); 

									tmp2 = tmp->next;
									tmp3 = tmp->next->next;
									
									tmp->next = tmp->next->next->next;
									if (tmp->next && tmp->next->prev) 
										tmp->next->prev = tmp;
									
									if(tmp2)
									{
										if(tmp2->data)
											free(tmp2->data);
										free(tmp2);
									}
									if(tmp3)
									{
										if(tmp3->data)
											free(tmp3->data);
										free(tmp3);
									}
								}
								else if (tmp->next) 
								{
									char *joined_str = ft_strjoin(tmp_char_ptr, tmp->next->data);
									free(tmp_char_ptr); 
									tmp->data = joined_str;
									tmp2 = tmp->next;
									tmp->next = tmp->next->next; 
									if (tmp->next && tmp->next->prev) 
										tmp->next->prev = tmp;
									if(tmp2)
									{
										if(tmp2->data)
											free(tmp2->data);
										free(tmp2);
									}
								}
							}
						}
						tmp = tmp->next;
					}
					tmp = head; 
					while (tmp)
					{
						tmp = tmp->next;
					}
					size_lst = lst_size(&head);
					new_cmd_arr = malloc(sizeof(char *) * (size_lst + 1));
					if (!new_cmd_arr)
					{
						free_list(&head);
						if (old_cmd) free(old_cmd);
						
						return ;
					}
					j = 0; 
					tmp = head;
					while (tmp)
					{
						if (tmp->data)
							new_cmd_arr[j] = ft_strdup(tmp->data);
						else
							new_cmd_arr[j] = ft_strdup(""); 
						j++;
						tmp = tmp->next;
					}
					new_cmd_arr[j] = NULL;
					free_list(&head);
					(*tree)->command_arr_expanded[i] = new_cmd_arr; 
				}
				else 
				{
					new_cmd_arr = malloc(sizeof(char *) * 2);
					if (!new_cmd_arr)
					{
						if (old_cmd)
							free(old_cmd);
						return ;
					}
					new_cmd_arr[0] = old_cmd;
					old_cmd = NULL; 
					new_cmd_arr[1] = NULL;

					(*tree)->command_arr_expanded[i] = new_cmd_arr; 
				}
				if (old_cmd) 
					free(old_cmd);
				i++; 
			}
			(*tree)->command_arr_expanded[i] = NULL;
			count = 0;
			int k = 0; 
			while ((*tree)->command_arr_expanded[k])
			{
				j = 0; 
				while ((*tree)->command_arr_expanded[k][j])
				{
					count++;
					j++;
				}
				k++;
			}
			new_cmd_arr = malloc(sizeof(char *) * (count + 1));
			if (!new_cmd_arr)
				return ;
			count = 0; 
			k = 0;
			while ((*tree)->command_arr_expanded[k])
			{
				j = 0;
				while ((*tree)->command_arr_expanded[k][j])
				{
					new_cmd_arr[count] = ft_strdup((*tree)->command_arr_expanded[k][j]);
					count++;
					j++;
				}
				k++;
			}
			new_cmd_arr[count] = NULL;
			k=0;
			while((*tree)->command_arr_expanded[k])
			{
				j = 0;
				while((*tree)->command_arr_expanded[k][j])
				{
					free((*tree)->command_arr_expanded[k][j]);
					j++;
				}
				free((*tree)->command_arr_expanded[k]);
				k++;
			}
			free((*tree)->command_arr_expanded);
			(*tree)->command_arr_expanded = NULL;
			k = 0;
			while ((*tree)->command_arr[k]) 
			{
				free((*tree)->command_arr[k]);
				k++;
			}
			free((*tree)->command_arr); 
			(*tree)->command_arr = new_cmd_arr;
			i = 0; 
			while ((*tree)->command_arr[i])
				i++;
			(*tree)->split = malloc(sizeof(t_split *) * (i + 1));
			if (!(*tree)->split)
				return ;
			k = 0; 
			while(k < i)
			{
				(*tree)->split[k] = malloc(sizeof(t_split));
				if (!(*tree)->split[k])
				{
					/* TODO: cleanup previously allocated splits */
					return ;
				}
				(*tree)->split[k]->arr_index = 0; 
				(*tree)->split[k]->split_flag = 0; 
				k++;
			}
            (*tree)->split[k] = NULL;
			i = 0; 
			in_quotes = 0;
			flag = 0;
			prev_command_arr = NULL;
			while ((*tree)->command_arr[i])
			{
				check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
				(*tree)->split[i]->arr_index = i;
				if (is_fully_quoted)
				{
					(*tree)->split[i]->split_flag = 0;
					prev_command_arr = (*tree)->command_arr[i];
					i++;
					continue ;
				}
				j = 0; 
				in_quotes = 0; 
				flag = 0;      
				if ((*tree)->command_arr[i])
				{
					while ((*tree)->command_arr[i][j])
					{
						if (!in_quotes && ((*tree)->command_arr[i][j] == '"' || (*tree)->command_arr[i][j] == '\''))
						{
							in_quotes = 1;
							quote_type = (*tree)->command_arr[i][j];
						}
						else if (in_quotes && (*tree)->command_arr[i][j] == '$')
							flag = 1; 
						else if (in_quotes && (*tree)->command_arr[i][j] == quote_type)
							in_quotes = 0;
						j++;
					}
				}
				// printf("condition = %d\n", (!ft_strchr((*tree)->command_arr[i], '$') && !isnt_valid((*tree)->command_arr[i], 0)));
				if ((variable_search_instr((*tree)->command_arr[i]) && prev_command_arr && !ft_strcmp(prev_command_arr, "=") && (i - 2 < 0 || (i - 2 >= 0 && (!variable_search_instr((*tree)->command_arr[i - 2]) && !isnt_valid((*tree)->command_arr[i - 2], 1))))) ||((variable_search_instr((*tree)->command_arr[i]) && flag == 1)) || (!ft_strchr((*tree)->command_arr[i], '$') && !isnt_valid((*tree)->command_arr[i], 0)))
				{
					(*tree)->split[i]->split_flag = 0;
				}
				else
				{
					(*tree)->split[i]->split_flag = 1;
				}
				prev_command_arr = (*tree)->command_arr[i];
				i++;
			}
			if (!ft_strcmp((*tree)->command_arr[0], "export") || variable_search_instr((*tree)->command_arr[0]))
			{
				i = 1;
				while ((*tree)->command_arr[i])
				{
					
					check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
					(*tree)->split[i]->arr_index = i;

					if (is_fully_quoted) {
						(*tree)->split[i]->split_flag = 0;
						prev_command_arr = (*tree)->command_arr[i];
						i++;
						continue ;
					}
					j = 0; in_quotes = 0; flag = 0;
					if ((*tree)->command_arr[i])
					{
						while ((*tree)->command_arr[i][j])
						{
							if (!in_quotes && ((*tree)->command_arr[i][j] == '"' || (*tree)->command_arr[i][j] == '\''))
							{
								in_quotes = 1; quote_type = (*tree)->command_arr[i][j];
							}
							else if (in_quotes && (*tree)->command_arr[i][j] == '$')
								flag = 1;
							else if (in_quotes && (*tree)->command_arr[i][j] == quote_type)
								in_quotes = 0;
							j++;
						}
					}
					if ((variable_search_instr((*tree)->command_arr[i]) && prev_command_arr && !ft_strcmp(prev_command_arr, "=") && (i - 2 < 0 || (i - 2 >= 0 && (!variable_search_instr((*tree)->command_arr[i - 2]) && !isnt_valid((*tree)->command_arr[i - 2], 1))))) || (variable_search_instr((*tree)->command_arr[i]) && flag == 1) || (!ft_strchr((*tree)->command_arr[i], '$') && !isnt_valid((*tree)->command_arr[i], 0)))
						(*tree)->split[i]->split_flag = 0;
					else
						(*tree)->split[i]->split_flag = 1;
					
					prev_command_arr = (*tree)->command_arr[i];
					i++;
				}
				char *str_check;
				char *str_check2;
				int	flag0;

				flag0 = 0;
				str_check2 = ft_strdup((*tree)->command_arr[0]);
				str_check = str_extractor(str_check2);
				if (!ft_strcmp(str_check, str_check2) && word_recognizer(str_check))
					flag0 = 1;
				if (str_check2)
					free(str_check2);
				if (str_check)
					free(str_check);
				if (variable_search_instr((*tree)->command_arr[0]) && flag0 == 1)
				{
					i = 0;
					while ((*tree)->split[i] && (*tree)->command_arr[i])
					{ 
						check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
						if (!is_fully_quoted)
							(*tree)->split[i]->split_flag = 1;
						i++;
					}
				}
			}
			else if (hidenword((*tree)->command_arr[0], "export"))
			{
				i = 1; 
				
				while ((*tree)->command_arr[i])
				{
					
					check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
                     (*tree)->split[i]->arr_index = i;

					if (is_fully_quoted) {
						(*tree)->split[i]->split_flag = 0;
						i++;
						continue ;
					}
					j = 0;
					in_quotes = 0;
					flag = 0;
					if ((*tree)->command_arr[i])
					{
						while ((*tree)->command_arr[i][j])
						{
							if (!in_quotes && ((*tree)->command_arr[i][j] == '"' || (*tree)->command_arr[i][j] == '\''))
							{
								in_quotes = 1; quote_type = (*tree)->command_arr[i][j];
							}
							else if (in_quotes && (*tree)->command_arr[i][j] == '$')
								flag = 1;
							else if (in_quotes && (*tree)->command_arr[i][j] == quote_type)
								in_quotes = 0;
							j++;
						}
					}
					if (((variable_search_instr((*tree)->command_arr[i]) && flag == 1)))
						(*tree)->split[i]->split_flag = 0;
					else
						(*tree)->split[i]->split_flag = 1;
					i++;
				}
				i = 1; 
				while ((*tree)->split[i] && (*tree)->command_arr[i])
				{
					check_full_quotes((*tree)->command_arr[i], &is_fully_quoted, &quote_char);
					if (!is_fully_quoted && !variable_search_instr((*tree)->command_arr[i]))
						(*tree)->split[i]->split_flag = 0;
					i++;
				}
			}
		}
		else 
		{
			char *str_check;
			char *str_check2;
			int	flag0;
			// int	flag00;
			flag0 = 0;
			str_check2 = ft_strdup((*tree)->command_arr[0]);
			str_check = str_extractor(str_check2);
			if (!ft_strcmp(str_check, str_check2) && word_recognizer(str_check))
				flag0 = 1;
			if (str_check2)
				free(str_check2);
			if (str_check)
				free(str_check);
			i = 0;
			while ((*tree)->command_arr[i])
				i++;
			(*tree)->split = malloc(sizeof(t_split *) * (i + 1));
			if (!(*tree)->split)
				return ;
			j = 0;
			while (j < i)
			{
				(*tree)->split[j] = malloc(sizeof(t_split));
				if (!(*tree)->split[j])
				{
					/* TODO: cleanup */
					return ;
				}
				(*tree)->split[j]->arr_index = j;
				int has_quotes = 0;
				// printf("word_recognizer = %d\n", word_recognizer((*tree)->command_arr[j]));
				// printf("word = %s\n", (*tree)->command_arr[j]);
				if ((*tree)->command_arr[j] && (ft_strchr((*tree)->command_arr[j], '"') || 
												ft_strchr((*tree)->command_arr[j], '\'')))
					has_quotes = 1;
				if (j == 0 || has_quotes || countwords((*tree)->command_arr[j], 32) <= 1)
					(*tree)->split[j]->split_flag = 0;
				else
					(*tree)->split[j]->split_flag = 1;
				
				j++;
			}
			(*tree)->split[j] = NULL;
		}
	}
}

void	split_adjustments(t_tree **tree)
{
	int	i;
	int cmd_count;

	if ((*tree) && (*tree)->left)
		split_adjustments(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		split_adjustments(&(*tree)->right);
	if ((*tree) && (*tree)->split && (*tree)->command_arr)
	{
		cmd_count = 0;
		while ((*tree)->command_arr[cmd_count])
			cmd_count++;
		i = 0;
		while ((*tree)->split[i] && i < cmd_count)
		{
			int has_single_quotes = 0;
			int j = 0;
			while ((*tree)->command_arr[i][j])
			{
				if ((*tree)->command_arr[i][j] == '\'')
				{
					has_single_quotes = 1;
					break ;
				}
				j++;
			}
			if (has_single_quotes || countwords((*tree)->command_arr[i], 32) <= 1)
				(*tree)->split[i]->split_flag = 0;
			i++;
		}
	}
}

// void	split_adjustments(t_tree **tree)
// {
// 	int	i;

// 	if ((*tree) && (*tree)->left)
// 		split_adjustments(&(*tree)->left);
// 	if ((*tree) && (*tree)->right)
// 		split_adjustments(&(*tree)->right);
// 	if ((*tree) && (*tree)->split)
// 	{
// 		i = 0;
// 		while ((*tree)->split[i])
// 		{
// 			if (countwords((*tree)->command_arr[i], 32) <= 1)
// 				(*tree)->split[i]->split_flag = 0;
// 			i++;
// 		}
// 	}
// }

void	command_arr_readjustments(t_tree **tree)
{
	int		i;
	int		j;
	int		k;
	int		count;
	char	**cmd;
	char	*new_str;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if ((*tree) && (*tree)->left)
		command_arr_readjustments(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		command_arr_readjustments(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		head = NULL;
		if ((*tree)->command_arr_expanded)
		{
			i = -1;
			while ((*tree)->command_arr_expanded[++i])
			{
				j = -1;
				while ((*tree)->command_arr_expanded[i][++j])
				{
					free((*tree)->command_arr_expanded[i][j]);
					(*tree)->command_arr_expanded[i][j] = NULL;
				}
				free((*tree)->command_arr_expanded[i]);
				(*tree)->command_arr_expanded[i] = NULL;
			}
			free((*tree)->command_arr_expanded);
			(*tree)->command_arr_expanded = NULL;
		}
		i = 0;
		while ((*tree)->command_arr[i])
			i++;
		(*tree)->command_arr_expanded = malloc(sizeof(char **) * (i + 1));
		if (!(*tree)->command_arr_expanded)
			return ;
		if (!(*tree)->split)
		{
			(*tree)->split = malloc(sizeof(t_split *) * (i + 1));
			if (!(*tree)->split)
			{
				free((*tree)->command_arr_expanded);
				(*tree)->command_arr_expanded = NULL;
				return ;
			}
			j = 0;
			while (j < i)
			{
				(*tree)->split[j] = malloc(sizeof(t_split));
				if (!(*tree)->split[j])
				{
					while (j > 0)
					{
						j--;
						free((*tree)->split[j]);
					}
					free((*tree)->split);
					(*tree)->split = NULL;
					free((*tree)->command_arr_expanded);
					(*tree)->command_arr_expanded = NULL;
					return ;
				}
				(*tree)->split[j]->arr_index = j;
				(*tree)->split[j]->split_flag = 0;
				j++;
			}
			(*tree)->split[i] = NULL;
		}
		i = 0;
		while ((*tree)->command_arr[i])
		{
			if ((*tree)->split && (*tree)->split[i] && countwords(
					(*tree)->command_arr[i], 32) > 1 && (
					*tree)->split[i]->split_flag == 1)
			{
				(*tree)->command_arr_expanded[i] = ft_split(
						(*tree)->command_arr[i], ' ');
			}
			else
			{
				cmd = malloc(sizeof(char *) * 2);
				if (!cmd)
				{
					i++;
					continue ;
				}
				cmd[0] = ft_strdup((*tree)->command_arr[i]);
				cmd[1] = NULL;
				(*tree)->command_arr_expanded[i] = cmd;
			}
			i++;
		}
		(*tree)->command_arr_expanded[i] = NULL;
		i = 0;
		count = 0;
		while ((*tree)->command_arr_expanded[i])
		{
			j = 0;
			while ((*tree)->command_arr_expanded[i][j])
			{
				count++;
				// printf("(*tree)->command_arr_expanded == %s\n", (*tree)->command_arr_expanded[i][j]);
				j++;
			}
			i++;
		}
		if (count == 0)
		{
			cmd = malloc(sizeof(char *));
			if (!cmd)
				return ;
			cmd[0] = NULL;
		}
		else
		{
			cmd = malloc(sizeof(char *) * (count + 1));
			if (!cmd)
				return ;
			i = 0;
			k = 0;
			while ((*tree)->command_arr_expanded[i])
			{
				j = 0;
				while ((*tree)->command_arr_expanded[i][j])
				{
					cmd[k] = ft_strdup((*tree)->command_arr_expanded[i][j]);
					k++;
					j++;
				}
				i++;
			}
			cmd[k] = NULL;
			// int m = 0;
			// while (cmd[m])
			// {
			// 	printf("cmd = %s\n", cmd[m]);
			// 	m++;
			// }
			if (k > 0)
			{
				head = malloc(sizeof(t_list));
				if (!head)
				{
					i = 0;
					while (cmd[i])
					{
						free(cmd[i]);
						i++;
					}
					free(cmd);
					return ;
				}
				head->data = ft_strdup(cmd[0]);
				head->token = NULL;
				head->prev = NULL;
				head->next = NULL;
				tmp = head;
				i = 1;
				while (cmd[i])
				{
					new_node = malloc(sizeof(t_list));
					if (!new_node)
					{
						free_list(&head);
						j = 0;
						while (cmd[j])
						{
							free(cmd[j]);
							j++;
						}
						free(cmd);
						return ;
					}
					new_node->data = ft_strdup(cmd[i]);
					new_node->token = NULL;
					new_node->prev = tmp;
					new_node->next = NULL;
					tmp->next = new_node;
					tmp = new_node;
					i++;
				}
				tmp = head;
				while (tmp)
				{
					if ((tmp->data && tmp->prev && tmp->next) && (
							tmp->prev->data && tmp->next->data
						) && !ft_strcmp(tmp->data, "="))
					{
						new_str = ft_strjoin_three(tmp->prev->data,
								tmp->data, tmp->next->data);
						free(tmp->data);
						tmp->data = ft_strdup(new_str);
						free(new_str);
						free(tmp->prev->data);
						tmp->prev->data = NULL;
						free(tmp->next->data);
						tmp->next->data = NULL;
					}
					tmp = tmp->next;
				}
			}
			i = 0;
			while (cmd[i])
			{
				free(cmd[i]);
				i++;
			}
			free(cmd);
			j = 0;
			if (head)
			{
				j = lst_size(&head);
			}
			cmd = malloc(sizeof(char *) * (j + 1));
			if (!cmd)
			{
				if (head)
					free_list(&head);
				return ;
			}
			i = 0;
			if (head)
			{
				tmp = head;
				while (tmp)
				{
					if (tmp->data)
					{
						cmd[i] = ft_strdup(tmp->data);
						i++;
					}
					tmp = tmp->next;
				}
				free_list(&head);
			}
			cmd[i] = NULL;
		}
		i = 0;
		while ((*tree)->command_arr[i])
		{
			free((*tree)->command_arr[i]);
			(*tree)->command_arr[i] = NULL;
			i++;
		}
		free((*tree)->command_arr);
		(*tree)->command_arr = cmd;
		// printf("\n\n");
		// i = 0;
		// while ((*tree)->command_arr[i])
		// {
		// 	printf("++++++++++++%s\n", (*tree)->command_arr[i]);
		// 	i++;
		// }
		if ((*tree)->split)
		{
			i = 0;
			while ((*tree)->split[i])
			{
				free((*tree)->split[i]);
				i++;
			}
			free((*tree)->split);
		}
		i = 0;
		while ((*tree)->command_arr[i])
			i++;
		(*tree)->split = malloc(sizeof(t_split *) * (i + 1));
		if (!(*tree)->split)
			return ;
		i = 0;
		while ((*tree)->command_arr[i])
		{
			(*tree)->split[i] = malloc(sizeof(t_split));
			if (!(*tree)->split[i])
			{
				j = 0;
				while (j < i)
				{
					free((*tree)->split[j]);
					j++;
				}
				free((*tree)->split);
				(*tree)->split = NULL;
				return ;
			}
			(*tree)->split[i]->arr_index = i;
			(*tree)->split[i]->split_flag = 0;
			i++;
		}
		(*tree)->split[i] = NULL;
	}
}

int	count_variables(char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		i++;
	}
	return (count);
}

t_list	*new_list_init(char *str)
{
	int	i;
	int	in_quotes;
	int	open_par;
	int	closed_par;
	char quote_type;
	char *word;
	char *tmp_char;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if (!str)
		return (NULL);
	i = 0;
	in_quotes = 0;
	open_par = 0;
	closed_par = 0;
	word = NULL;
	if (*str == '"' || *str == '\'')
	{
		while (str[i])
		{
			if (!in_quotes && (str[i] == '"' || str[i] == '\''))
			{
				in_quotes = 1;
				quote_type = str[i];
			}
			else if (in_quotes && str[i] == quote_type)
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i + 1);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
		in_quotes = 0;
	}
	else if (str[i] == '(')
	{
		while (str[i])
		{
			if (str[i] == '(')
				open_par++;
			else if (str[i] == ')')
				closed_par++;
			if (open_par == closed_par && open_par != 0)
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i + 1);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (*str == '$')
	{
		i = 1;
		while (str[i])
		{
			if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z') && !(str[i] >= '0' && str[i] <= '9'))
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i, ft_strlen(str) - (i));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if(*str == '+')
	{
		if (str[1] == '=')
		{
			tmp_char = word;
			word = ft_substr(str, 0, 2);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
		}
		else
		{
			tmp_char = word;
			word = ft_substr(str, 0, 1);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
		}
		tmp_char = str;
		str = ft_substr(str, ft_strlen(word), ft_strlen(str) - (ft_strlen(word)));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (is_operator(*str))
	{
		tmp_char = word;
		if (str[0] == '&' && ((str[1] && str[1] != '&') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '*' && ((str[1] && str[1] != '*') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '|' && ((str[1] && str[1] != '|') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '>' && ((str[1] && str[1] != '>') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '<' && ((str[1] && str[1] != '<') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '&' && str[1] && str[1] == '&')
			word = ft_substr(str, 0, 2);
		else if (str[0] == '|' && str[1] && str[1] == '|')
			word = ft_substr(str, 0, 2);
		else if (str[0] == '>' && str[1] && str[1] == '>')
			word = ft_substr(str, 0, 2);
		else if (str[0] == '<' && str[1] && str[1] == '<')
			word = ft_substr(str, 0, 2);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, ft_strlen(word), ft_strlen(str) - ft_strlen(word));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else
	{
		while (str[i])
		{
			if (str[i] == '"' || str[i] == '\'' || str[i] == '$' || is_operator(str[i]) || str[i] == '(' || str[i] == '=')
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i, ft_strlen(str) - (i));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);		
	}
	head = malloc(sizeof(t_list));
	head->data = ft_strdup(word);
	head->token = NULL;
	head->prev = NULL;
	head->next = NULL;
	tmp = head;
	i = 0;
	while (str && *str)
	{
		if (*str == '"' || *str == '\'')
		{
			i = 0;
			while (str[i])
			{
				if (!in_quotes && (str[i] == '"' || str[i] == '\''))
				{
					in_quotes = 1;
					quote_type = str[i];
				}
				else if (in_quotes && str[i] == quote_type)
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i + 1);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
			in_quotes = 0;
		}
		else if (str[i] == '(')
		{
			i = 0;
			while (str[i])
			{
				if (str[i] == '(')
					open_par++;
				else if (str[i] == ')')
					closed_par++;
				if (open_par == closed_par && open_par != 0)
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i + 1);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (*str == '$')
		{
			i = 1;
			while (str[i])
			{
				if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z') && !(str[i] >= '0' && str[i] <= '9'))
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i, ft_strlen(str) - (i));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (is_operator(*str))
		{
			tmp_char = word;
			if (str[0] == '&' && ((str[1] && str[1] != '&') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '*' && ((str[1] && str[1] != '*') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '|' && ((str[1] && str[1] != '|') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '>' && ((str[1] && str[1] != '>') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '<' && ((str[1] && str[1] != '<') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '&' && str[1] && str[1] == '&')
				word = ft_substr(str, 0, 2);
			else if (str[0] == '|' && str[1] && str[1] == '|')
				word = ft_substr(str, 0, 2);
			else if (str[0] == '>' && str[1] && str[1] == '>')
				word = ft_substr(str, 0, 2);
			else if (str[0] == '<' && str[1] && str[1] == '<')
				word = ft_substr(str, 0, 2);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, ft_strlen(word), ft_strlen(str) - ft_strlen(word));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (*str == '=')
		{
			tmp_char = word;
			word = ft_substr(str, 0, 1);
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, 1, ft_strlen(str) - 1);
			free(tmp_char);
		}
		else
		{
			i = 0;
			while (str[i])
			{
				if (str[i] == '"' || str[i] == '\'' || str[i] == '$' || is_operator(str[i]) || str[i] == '(')
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i, ft_strlen(str) - (i));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);		
		}
		new_node = malloc(sizeof(t_list));
		new_node->data = ft_strdup(word);
		new_node->token = NULL;
		new_node->prev = tmp;
		new_node->next = NULL;
		tmp->next = new_node;
		tmp = new_node;
	}
	if (str)
		free(str);
	if (word)
		free(word);
	return (head);
}


void	reset_vars(t_tree **tree, t_env **env)
{
	int	i;
	int	j;
	int	k;
	int	l;
	int	m;
	// int	n;
	int	in_quotes;
	int	final_len;
	int	list_size;
	char quote_type;
	char *new_str;
	// char *tmp_char;
	// char *before;
	// char *after;
	char *old_cmd;
	char **cmd;
	char ***cmd2;
	t_list	*head;
	t_list	*tmp;

	if ((*tree) && (*tree)->left)
		reset_vars(&(*tree)->left, env);
	if ((*tree) && (*tree)->right)
		reset_vars(&(*tree)->right, env);
	if ((*tree) && (*tree)->command_arr)
	{
		list_size = 0;
		final_len = 0;
		i = 0;
		head = NULL;
		while ((*tree)->command_arr[i])
		{
			if (ft_strchr((*tree)->command_arr[i], '"') || ft_strchr((*tree)->command_arr[i], '\'') || ft_strchr((*tree)->command_arr[i], '$'))
			{
				old_cmd = ft_strdup((*tree)->command_arr[i]);
				head = list_init(old_cmd);
				list_size = lst_size(&head);
				j = 0;
				tmp = head;
				while (tmp)
				{
					// if (ft_strchr(tmp->data, '"') || ft_strchr(tmp->data, '\'') || ft_strchr(tmp->data, '$'))
					// {
						k = 0;
						l = 0;
						// if (ft_strchr(tmp->data, '$'))
						// {
							in_quotes = 0;
							quote_type = 0;
							j = 0;
							while (tmp->data && tmp->data[j])
							{
								if (!in_quotes && (tmp->data[j] == '"' || tmp->data[j] == '\''))
								{
									in_quotes = 1;
									quote_type = tmp->data[j];
								}
								else if (in_quotes && tmp->data[j] == quote_type)
									in_quotes = 0;
								if (tmp->data[j] == '$' && (!in_quotes || (in_quotes && quote_type == '"')))
								{
									// n = j;
									l = j;
									if (tmp->data[j] == '$' && (((!in_quotes && tmp->data[j + 1] && (tmp->data[j + 1] == '"' || tmp->data[j + 1] == '\'')) || (in_quotes && !tmp->data[j + 1]))))
										j++;
									else
									{
										process_array_variable(&tmp->data, 0, &j, env);
										if (j != -1 && j < 0)
										{
											l = 0;
											k = 0;
										}
										else
										{
											k = l + j;
											k -= 2;
											l++;
										}
										in_quotes = 0;
										j = -1;
									}
								}
								j++;
							}
					// 	}
					// }

					tmp = tmp->next;
				}
				tmp = head;
				new_str = ft_strdup(tmp->data);
				free((*tree)->command_arr[i]);
				(*tree)->command_arr[i] = new_str;
				free_list(&head);
			}
			i++;
		}
		list_size = 0;
		final_len = 0;
		i = 0;
		head = NULL;
		while ((*tree)->command_arr[i])
			i++;
		cmd2 = malloc(sizeof(char **) * (i + 1));
		i = 0;
		while ((*tree)->command_arr[i])
		{
			old_cmd = ft_strdup((*tree)->command_arr[i]);
			head = list_init(old_cmd);
			list_size = lst_size(&head);
			if (list_size == 0)
			{
				cmd = malloc(sizeof(char *) * (list_size + 1));
				j = 0;
				tmp = head;
				while (tmp)
				{
					new_str = ft_strdup(tmp->data);
					cmd[j] = new_str;
					j++;
					tmp = tmp->next;
				}
				cmd[j] = NULL;
				cmd2[i] = cmd;
				free_list(&head);
				free(old_cmd);
				i++;
				continue ;
			}
			cmd = malloc(sizeof(char *) * (list_size + 1));
			j = 0;
			tmp = head;
			while (tmp)
			{
				new_str = ft_strdup(tmp->data);
				cmd[j] = new_str;
				j++;
				tmp = tmp->next;
			}
			cmd[j] = NULL;
			cmd2[i] = cmd;
			free_list(&head);
			i++;
		}
		cmd2[i] = NULL;
		i = 0;
		while ((*tree)->command_arr[i])
		{
			free((*tree)->command_arr[i]);
			i++;
		}
		free((*tree)->command_arr);
		m = 0;
		i = 0;
		j = 0;
		while (cmd2[i])
		{
			j = 0;
			while (cmd2[i][j])
			{
				m++;
				j++;
			}
			i++;
		}
		(*tree)->command_arr = malloc(sizeof(char *) * (m + 1));
		i = 0;
		j = 0;
		m = 0;
		while (cmd2[i])
		{
			j = 0;
			while (cmd2[i][j])
			{
				(*tree)->command_arr[m] = ft_strdup(cmd2[i][j]);
				m++;
				j++;
			}
			i++;
		}
		(*tree)->command_arr[m] = NULL;
		i = -1;
		while (cmd2[++i])
		{
			if (cmd2[i])
			{
				j = -1;
				while (cmd2[i][++j])
				{
					if (cmd2[i][j])
					{
						free(cmd2[i][j]);
						cmd2[i][j] = NULL;
					}
				}
				free(cmd2[i]);
				cmd2[i] = NULL;
			}
		}
		(free(cmd2), cmd2 = NULL);
		i = 0;
		while ((*tree)->command_arr[i])
		{
			if (ft_strchr((*tree)->command_arr[i], '"') || ft_strchr((*tree)->command_arr[i], '\''))
			{
				old_cmd = (*tree)->command_arr[i];
				final_len = count_filtered_length(old_cmd, &(*tree)->var, l, k);
				new_str = create_filtered_string(old_cmd, final_len, l, k);
				if (!new_str)
					return ;
				free((*tree)->command_arr[i]);
				(*tree)->command_arr[i] = new_str;
			}
			i++;
		}
		i = 0;
		j = 0;
		while ((*tree)->command_arr[i])
		{
			j = 0;
			while ((*tree)->command_arr[i][j])
			{
				if ((*tree)->command_arr[i][j] == 10)
					(*tree)->command_arr[i][j] = '"';
				else if ((*tree)->command_arr[i][j] == 11)
					(*tree)->command_arr[i][j] = '\'';
				j++;
			}
			i++;
		}
	}
}

void export_cases(t_tree **tree)
{
	int	i;
	int	j;
	int	flag;
	int	in_quotes;
	char quote_type;
	char *old_cmd;
	char *tmp_char;
	char *new_str;
	t_list *head;
	t_list *tmp;
	t_list *tmp2;

    if ((*tree) && (*tree)->left)
		export_cases(&(*tree)->left);
    if ((*tree) && (*tree)->right)
		export_cases(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		i = 1;
		if (!ft_strcmp((*tree)->command_arr[0], "export"))
		{
			while ((*tree)->command_arr[i])
			{
				old_cmd = ft_strdup((*tree)->command_arr[i]);
				head = new_list_init(old_cmd);
				tmp = head;
				new_str = NULL;
				while (tmp)
				{
					if (!ft_strcmp(tmp->data, "="))
					{
						tmp2 = tmp;
						if (!isnt_valid(tmp2->prev->data, 1))
						{
							tmp2 = tmp->next;
							while (tmp2)
							{
								j = 0;
								flag = 0;
								in_quotes = 0;
								quote_type = 0;
								while (tmp2->data[j])
								{
									if (!in_quotes && (tmp2->data[j] == '"' || tmp2->data[j] == '\''))
									{
										in_quotes = 1;
										quote_type = tmp2->data[j];
									}
									else if (in_quotes && tmp2->data[j] == quote_type)
										in_quotes = 0;
									if (tmp2->data[j] == '$' && !in_quotes)
										flag = 1;
									j++;
								}
								if (flag == 1)
								{
									tmp_char = tmp2->data;
									tmp2->data = ft_strjoin_three("\"", tmp2->data, "\"");
									free(tmp_char);
								}
								tmp2 = tmp2->next;
							}
						}
					}
					tmp = tmp->next;
				}
				tmp = head;
				while (tmp)
				{
					tmp_char = new_str;
					new_str = ft_strjoin(new_str, tmp->data);
					free(tmp_char);
					tmp = tmp->next;
				}
				free((*tree)->command_arr[i]);
				(*tree)->command_arr[i] = new_str;
				free_list(&head);
				i++;
			}
		}
	}
}

// void	ft_my_hand(int sig)
// {
// 	// dprintf(2, "entered here\n");
// 	// if ()
// 	if (sig == 2)
// 	{
// 		write(1, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		global_status = 130;
// 	}
// 	if (sig == 3)
// 	{
// 		write(1, "Quit: 3\n", 9);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		global_status = 131;
// 		// exit (131);
// 	}
// }

void	ft_sig(int sig)
{
	if (sig == 2)
	{
		global_status = 130;
		write(1, "\n", 1);
		// rl_redisplay();
	}
	else if (sig == 3)
	{
		global_status = 131;
		write(1, "\n", 1);
		// rl_redisplay();
	}
	// signal(SIGINT, SIG_IGN);
	// signal(SIGQUIT, SIG_IGN);
}

void	ft_st(t_tree *tree, int stat_flag)
{
	if (stat_flag == 0)
		tree->status = 1;
}

void	ft_signal_exec(void)
{
	if (global_status == 2)
	{
		// dprintf(2, "entered here");
		ft_putstr_fd(1, "\n");
		// tree->status = 130;
	}
	else if (global_status == 3)
	{
		ft_putstr_fd(1, "Quit: 3\n");
		// tree->status = 131;
	}
}

void	display_terminal_control_chars(void)
{
	struct termios	terminos_p;

	tcgetattr(0, &terminos_p);
	terminos_p.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &terminos_p);
}

// int	ft_hdoc_flag(t_tree *tree)
// {
	
// }
void f()
{
	system("leaks minishell");
}
int	main(int argc, char **argv, char **argev)  //ambiguous  $
{
	char		*str;
	int			flag;
	t_env		*env;
	t_tree		*tree;
	char 		**e;
	t_env 		*tmp;
	int			status;
	int			sig_flag;
	struct termios	termios_a;
	int check;

	// atexit(f);
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return 0;
	((void)argc, (void)argv, inits_main(&env, &tree, argev));
	e = ft_env_str(env);
	tmp = env;
	// tree->status = 0;// tanchof fin ki3mr tree
	tcgetattr(0, &termios_a);
	while (1)
	{
		sig_flag = 1;
		global_status = 0;
		check = 0;
		// if (isatty(0))
		// {
		hide_terminal_control_chars();
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, SIG_IGN);
		// }
		flag = 0;
		str = readline("minishell$> ");
		if (!str)
		{
			ft_putstr_fd(1, "exit\n");
			ft_free_array(e);
			//YOU MUST PRINT EXIT
			free_env(&env);
			break ;
		}
		else if (!*str || check_empty(str))
		{
			free(str);
			continue ;
		}
		flag = 0;
		add_history(str);
		quote_parse(&str, &flag);
		// dprintf(2, "this is stat: %d\n", tree->status);
		lexer_to_tree(str, &tree, &flag);
		tree_to_rediropen(tree, &flag);
		var_set(&tree);
		// if (!flag)
		// 	reset_command_arr(&tree);
		// print_tree_visual(tree, 1, 1);
		export_cases(&tree);
		// print_tree_visual(tree, 1, 1);
		redirections_list_maker(&tree);
		if (!flag && has_wild_cards_comarr(&tree) == 1)
			handle_wildcards_in_cmdarr(&tree);
		if (!flag && has_wild_cards_fdlst(&tree) == 1)
			handle_wildcards_in_fdlst(&tree);
			// reset_vars(&tree, &env);
		// if (!flag)
			// if_zero_not_export()
		// print_tree_visual(tree, 1, 1);
		
		// quote_remove(&tree);
		// quote_remove(&tree);
		// quote_remove_lst(&tree);
		// print_tree_visual(tree, 1, 1);
		// if (variable_search(&tree) == 1) //TO EXPAND WITH IN EXECUTION THIS SEARCHES FOR VARIABLES AND THE NEXT ONE EXPANDS THEM
		// 	variable_expantion(&tree, &env);
															// if (variable_search_inlnkedlst(&tree) == 1)
																// variable_expantion_inlnkedlst(&tree, &env);
															
															// ambiguous_set(&tree);
															// quote_remove_lst(&tree);
		// if (!flag)
		// 	split_adjustments(&tree);
		// if (!flag)
		// 	command_arr_readjustments(&tree);
		// print_tree_visual(tree, 1, 1);
		// if (ambiguous_syntax_error(&tree, &env) == 1)
		// 	(write(2, "ambiguous redirect\n", 19), flag = 1);
		// if (!flag && ambiguous_syntax_error(&tree, &env) == 2)
		// 	(write(2, "No such file or directory\n", 26), flag = 1);
		// if (ambiguous_syntax_error(&tree, &env) != 2)
		// 	tree_empty_error(&tree, &flag);
		if (!flag)
		{
			tree->status = status;
			ft_hdoc_handle(tree, &sig_flag);
			ft_st(tree, sig_flag);
			if (sig_flag)
			{
				ft_execute(tree, &env, e, &check);
				ft_signal_exec();
			}
			status = tree->status;
			dprintf(2, "this is status: %d\n", status);
		}
		tcsetattr(0, TCSANOW, &termios_a);
		lasfree(&tree);
	}
	free_env(&env);
}
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt
//((ls > file) && echo | la > file2) > file3

//((ls)>file2) > file
// "((ls)>file2) > file"
// >file4(>file5 ls>file>file2>file3 -la>file6)>file7>file8
//> file4 (>file5 ls>file>file2>file3 -la>file6)>file7>file8make

//"*"
