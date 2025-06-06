/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/06/05 18:49:41 by aakroud          ###   ########.fr       */
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

void	ft_hdoc_expand(char **line, t_env **env, int status)
{
	int	in_quotes;
	int	i;
	int	n;
	int	flag;
	char quote_type;
	
	i = 0;
	in_quotes = 0;
	flag = 0;
	quote_type = 0;
	while ((*line) && (*line)[i])
	{
		if (!in_quotes && ((*line)[i] == '"' || (*line)[i] == '\''))
		{
			in_quotes = 1;
			quote_type = (*line)[i];
		}
		else if ((*line)[i] == quote_type)
			in_quotes = 0;
		if ((*line)[i] == '$' && (
				!in_quotes || (in_quotes && quote_type == '"')))
		{
			// if (variable_expantion_two(&tmp->name, i, env, &flag) == -1)
			// 	break ;
			n = i;
			if (process_array_variable(line, 0, &i, env) == -1)
				break ;
			if (i == -1)
				if_question_mark_two(line, status, n);
			// if ((*line) && countwords((*line), 32) != 1)
			// 	tmp->name_split = ft_split(tmp->name, 32);
			// if (flag == 1)
			// 	(*tree)->ambiguous = 1;
			// if (!variable_search_inlnkedlst(tree))
			// dprintf(2, "this is str in parsing: %s\n", (*line));
			i = -1;
		}
		i++;
	}
}

void	ft_hdoc(char *limiter, int fd, t_env **env, int status)
{
	char	*str;
	char	*tmp;

	str = readline("> ");
	while (str != NULL && ft_strcmp(limiter, str) != 0)
	{
		ft_hdoc_expand(&str, env, status);
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		tmp = str;
		str = readline("> ");
		free(tmp);
	}
	ft_hdoc_free(&str, &limiter, fd);
	exit (0);
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

int	ft_variable(t_tree *tree, t_env **h, char **e, int *check)
{
	if (cmd_check(tree) == 1)
		ft_word_handle(tree, h, e, check);
	else if (cmd_check(tree) == 0)
		tree->status = ft_cmd_exec(tree, h);
	else
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, tree->command_arr[0]);
		ft_putstr_fd(2, ": command not found\n");
	}
	return (tree->status);
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

void	display_terminal_control_chars(void)
{
	struct termios	terminos_p;

	tcgetattr(0, &terminos_p);
	terminos_p.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &terminos_p);
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
		}
		tree->status += 128;
	}
	// signal(SIGINT, ft_new_handler);
}

void	ft_execute(t_tree *tree, t_env **h, char **e, int *check)
{
	int id;

	if (!tree)
	{
		free_env(h);
		exit (1);
	}
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections == NULL) || (ft_strcmp("WORD", tree->type) == 0 && cmd_check(tree) == 0))
	{
		reset_vars(&tree, h);
		tree->status = ft_cmd_exec(tree, h);
	}
	if (ft_strcmp("PARENTHASIS", tree->type) == 0)
		tree->status = ft_parenthasis(tree, h, e);
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections != NULL) || (cmd_check(tree) == 0 && ft_strcmp("REDIRECTION", tree->type) == 0))
	{
		// dprintf(2, "this iun here\n");
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
		tree->status = ft_variable(tree, h, e, check);
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
		close (tree->fd_list->fd1);
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
		tree->status = ft_pip(tree, h, e, check);
	}
}

int	ft_hdoc_first(t_list_fd *tmp)
{
	int		fd;
	char	*temp;
	int		fd1;
	// t_list_fd *tmp;

	// tmp = tree->fd_list;
	unlink(tmp->name);
	tmp->name = ft_name_check(tmp->name);
	tmp->fd = open(tmp->name, O_RDWR | O_CREAT, 0777);
	if (tmp->fd == -1)
		return (1);
	tmp->fd1 = open(tmp->name, O_RDONLY, 0777);
	if (tmp->fd1 == -1)
		return (1);
	unlink(tmp->name);
	return (0);
}

void	 ft_hdoc_check(t_tree *tree, int *sig_flag, t_env **env, int status)
{
	t_list_fd *tmp;
	int		id;
	int		prev_fd;

	if (!tree)
		return ;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3 && (*sig_flag) == 1)
		{
			close (prev_fd);
			if (!ft_hdoc_first(tmp))
			{
				prev_fd = tmp->fd1;
				if (tmp-> fd == -1 || tmp->fd1 == -1)
				{
					perror("minishell: ");
					return ;
				}
				id = fork();
				if (id == 0)
				{
					signal(SIGINT, SIG_DFL);
					ft_hdoc(ft_strdup(tmp->name), tmp->fd, env, status);
					close (tmp->fd1);
				}
				else if (id < 0)
					perror("fork");
				signal(SIGINT, SIG_IGN);
				waitpid(id, &(tree->status), 0);
				if (WIFSIGNALED(tree->status))
				{
					tree->status = WTERMSIG(tree->status);
					if (tree->status == 2)
						ft_putstr_fd(1, "\n");
					*sig_flag = 0;
				}
				close (tmp->fd);
				// close (tmp->fd1);
			}
			// dup2(tmp->fd1, 0);
		}
		tmp = tmp->next;
	}
	// return (tree->sig_flag);
}

void	ft_hdoc_handle(t_tree *tree, int *sig_flag, t_env **env, int status)
{
	if (!tree)
		return ;
	// dprintf(2, "got out here\n");
	if (tree->left)
		ft_hdoc_handle(tree->left, sig_flag, env, status);
	if (tree->right)
		ft_hdoc_handle(tree->right, sig_flag, env, status);
	ft_hdoc_check(tree, sig_flag, env, status);
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

void	ft_st(t_tree *tree, int stat_flag)
{
	if (stat_flag == 0)
		tree->status = 1;
}

void	ft_signal_exec(void)
{
	if (global_status == 2)
	{
		ft_putstr_fd(1, "\n");
	}
	else if (global_status == 3)
	{
		ft_putstr_fd(1, "Quit: 3\n");
	}
}

int	main(int argc, char **argv, char **argev)
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

	if (!isatty(0) || !isatty(1))
		return (0);
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return 0;
	((void)argc, (void)argv, inits_main(&env, &tree, argev));
	e = ft_env_str(env);
	tmp = env;
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
		lexer_to_tree(str, &tree, &flag);
		tree_to_rediropen(tree, &flag);
		if (!flag)
		{
			export_cases(&tree);
			redirections_list_maker(&tree);
		}
		if (!flag && has_wild_cards_comarr(&tree) == 1)
			handle_wildcards_in_cmdarr(&tree);
		if (!flag && has_wild_cards_fdlst(&tree) == 1)
			handle_wildcards_in_fdlst(&tree);
		if (flag != 1)
			print_tree_visual(tree, 1, 1);
		printf("*******************%d\n", flag);
		if (!flag)
		{
			tree->status = status;
			// dprintf(2, "this is stat: %d\n", tree->status);
			ft_hdoc_handle(tree, &sig_flag, &env, tree->status);
			ft_st(tree, sig_flag);
			if (sig_flag)
			{
				ft_execute(tree, &env, e, &check);
				ft_signal_exec();
			}
			status = tree->status;
		}
		tcsetattr(0, TCSANOW, &termios_a);
		if (tree && flag != 1)
			free_tree(tree);
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
