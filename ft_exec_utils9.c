#include <minishell.h>

void	*env_fill_pwd(t_env *head)
{
	t_env	*tmp;

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
		env_fill_pwd(head);
	}
	return (head);
}

void	ft_close_handle(t_list_fd *head)
{
	if (!head)
		return ;
	while (head)
	{
		close (head->fd1);
		head = head->next;
	}
}

void	ft_first_child(t_tree *tree, int *check, char **e, t_env **h)
{
	signal(SIGINT, SIG_DFL);
	// signal(SIGPIPE, SIG_IGN);
	*check = 1;
	dup2(tree->fd[1], 1);
	close (tree->fd[1]);
	close (tree->fd[0]);
	ft_execute(tree->left, h, e, check);
	exit (tree->left->status);
}

void	ft_second_child(t_tree *tree, int *check, char **e, t_env **h)
{
	signal(SIGINT, SIG_DFL);
	*check = 1;
	dup2(tree->fd[0], 0);
	close (tree->fd[0]);
	close (tree->fd[1]);
	ft_execute(tree->right, h, e, check);
	exit(tree->right->status);
}

int	ft_wait_for_child(int status, int status1)
{
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		if (status == 2)
			global_status = SIGINT;
		else if (status == 3)
			global_status = SIGQUIT;
		status += 128;
	}
	else if (WIFSIGNALED(status1))
	{
		status1 = WTERMSIG(status1);
		if (status1 == 2)
			global_status = SIGINT;
		else if (status1 == 3)
			global_status = SIGQUIT;
	}
	else
	{
		status = WEXITSTATUS(status);
	}
	return (status);
}

void	ft_exec_redir(t_tree *tree, t_env **h, char **env)
{
	while (tree->fd_list != NULL && tree->ambiguous == 0)
	{
		if (ft_redir_check(tree->fd_list->redir) == 3)
			ft_exec_redir_helper(tree, 3);
		else if (ft_redir_check(tree->fd_list->redir) == 2)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
			ft_exec_redir_helper(tree, 2);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 1)
		{
			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
			ft_exec_redir_helper(tree, 1);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 4)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
			ft_exec_redir_helper(tree, 4);
		}
		tree->fd_list = tree->fd_list->next;
	}
	if (tree->ambiguous == 1)
		exit (1);
	if (tree->command_arr[0] && check_empty(tree->command_arr[0]) == 0)
	{
		ft_exec(tree, *h, env);
	}
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

int	ft_para_redir(t_tree *tree, t_env **h)
{
	int	status;

	status = 0;
	if (check_amb(tree) == 1)
		return (1);
	while (tree->fd_list != NULL && check_empty(tree->fd_list->name) != 1)
	{
		if (ft_redir_check(tree->fd_list->redir) == 1)
		{
			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
			if (tree->fd_list ->fd == -1)
			{
				ft_putstr_fd(2, "minishell: ");
				ft_putstr_fd(2, tree->fd_list->name);
				ft_putstr_fd(2, ": No such file or directory");
				return (1);
			}
			dup2(tree->fd_list->fd, 0);
			close (tree->fd_list->fd);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 2)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return (1);
			}
			dup2(tree->fd_list->fd, 1);
			close (tree->fd_list->fd);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 4)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return (1);
			}
			dup2(tree->fd_list->fd, 1);
			close (tree->fd_list->fd);
		}
		tree->fd_list = tree->fd_list->next;
	}
	return (status);
}

int	ft_parenthasis(t_tree *tree, t_env **h, char **e, int *check)
{
	int	org_stdout;
	int	org_stdin;
	int	n;

	n = 0;
	org_stdout = dup(1);
	org_stdin = dup(0);
	if (!tree)
		return (-1);
	n = ft_para_redir(tree, h);
	if (n)
		return (1);
	if (tree->left)
		ft_execute(tree->left, h, e, check);
	if (tree->right)
		ft_execute(tree->right, h, e, check);
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	return (0);
}

void	ft_signal_ign(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}