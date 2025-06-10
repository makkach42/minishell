# include <minishell.h>

void	ft_execute(t_tree *tree, t_env **h, char **e, int *check)
{
	int id;
	// int	exec_stat;

	// exec_stat = 0;
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
		tree->status = ft_parenthasis(tree, h, e, check);
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
		if (variable_search_inlnkedlst(&tree) == 1)
			variable_expantion_inlnkedlst(&tree, h);
		ambiguous_set(&tree);
		quote_remove_lst(&tree);
		if (ambiguous_syntax_error(&tree, h) == 1)
			(write(2, "ambiguous redirect\n", 19));
		if (ambiguous_syntax_error(&tree, h) == 2)
		{
			(write(2, "No such file or directory\n", 26));
		}
		ft_exec_redir(tree, h, e);
		id = fork();
		if (id == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (tree->command_arr[0] && check_empty(tree->command_arr[0]) == 0)
				ft_exec(tree, *h, e);
		}
		if (*check == 0)
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
		}
		close (tree->fd_list->fd1);
		waitpid(id, &(tree->status), 0);
		if (WIFEXITED(tree->status))
			tree->status = WEXITSTATUS(tree->status);
		else if (WIFSIGNALED(tree->status))
		{
			tree->status = WTERMSIG(tree->status);
			if (tree->status == 2)
				global_status = SIGINT;
			else if (tree->status == 3)
				global_status = SIGQUIT;
			tree->status += 128;
		}
		if (*check)
			exit (tree->status);
	}
	if (ft_strcmp("PIPE", tree->type) == 0)
	{
		tree->status = ft_pip(tree, h, e, check);
	}
}

int	ft_hdoc_first(t_list_fd *tmp)
{
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

	prev_fd = -1;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3 && (*sig_flag) == 1)
		{
			if (prev_fd != -1)
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
					exit (0);
				}
				else if (id < 0)
				{
					perror("fork");
					return ;
				}
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
		}
		tmp = tmp->next;
	}
	// return (tree->sig_flag);
}

int	ft_hdoc_count_second(t_tree *tree)
{
	t_list_fd *tmp;
	int 	lent;

	lent = 0;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3)
			lent++;
		tmp = tmp->next;
	}
	// dprintf(2, "this is it %d\n", lent);
	return (lent);
}

int	ft_hdoc_count(t_tree *tree)
{
	int lent;
	int	left_lent;
	int	right_lent;

	lent = 0;
	left_lent = 0;
	right_lent = 0;
	if (!tree)
		return (-1);
	if (tree->left)
		left_lent += ft_hdoc_count(tree->left);
	if (tree->right)
		right_lent += ft_hdoc_count(tree->right);
	lent += ft_hdoc_count_second(tree);
	return (lent + left_lent+ right_lent);
}