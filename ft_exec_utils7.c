/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:14:27 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/17 11:14:39 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_execute_com(t_tree *tree, t_env **h)
{
	if (variable_search_inlnkedlst(&tree) == 1)
		variable_expantion_inlnkedlst(&tree, h);
	quote_remove_lst(&tree);
	ambiguous_set(&tree);
	if (ambiguous_syntax_error(&tree, h) == 1)
		(write(2, "ambiguous redirect\n", 19));
	// if (ambiguous_syntax_error(&tree, h) == 2)
	// 	(write(2, "No such file or directory\n", 26));
	tree->status = ft_cmd_redir(tree, h);
}

void	ft_execute_redir(t_tree *tree, t_env **h, char **e)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (variable_search_inlnkedlst(&tree) == 1)
		variable_expantion_inlnkedlst(&tree, h);
	ambiguous_set(&tree);
	quote_remove_lst(&tree);
	if (ambiguous_syntax_error(&tree, h) == 1)
		(write(2, "ambiguous redirect\n", 19));
	// if (ambiguous_syntax_error(&tree, h) == 2)
	// 	(write(2, "No such file or directory\n", 26));
	// print_tree_visual(tree, 1, 1);
	ft_exec_redir(tree, h, e);
	exit (0);
}

void	ft_var_helper(t_tree **tree, t_env **h, char **e, int *check)
{
	reset_vars(tree, h);
	// print_tree_visual(*tree, 1, 1);
	(*tree)->status = ft_variable(*tree, h, e, check);
}

int	ft_op_and(t_tree *tree, t_env **h, char **e, int *check)
{
	int x;

	x = 0;
	ft_execute(tree->left, h, e, &x);
		dprintf(2, "entered in end right && %d\n", tree->left->status);
	if (tree->left->status == 0)
	{
		ft_execute(tree->right, h, e, &x);
		return (tree->right->status);
	}
	return (tree->left->status);
}

int	ft_op_or(t_tree *tree, t_env **h, char **e, int *check)
{
	int x;

	x = 0;
	ft_execute(tree->left, h, e, &x);
	dprintf(2, "entered in left || %d\n", tree->left->status);
	if (tree->left->status != 0)
	{
		ft_execute(tree->right, h, e, &x);
		return (tree->right->status);
	}
	return (tree->left->status);
}

void	ft_execute(t_tree *tree, t_env **h, char **e, int *check)
{
	int	id;

	if (!tree)
	{
		free_env(h);
		exit (1);
	}
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections == NULL)
		|| (ft_strcmp("WORD", tree->type) == 0 && cmd_check(tree) == 0))
	{
		reset_vars(&tree, h);
		tree->status = ft_cmd_exec(tree, h);
		if (*check)
			exit (tree->status);
	}
	if (ft_strcmp("PARENTHASIS", tree->type) == 0)
		tree->status = ft_parenthasis(tree, h, e, check);
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections != NULL)
		|| (cmd_check(tree) == 0 && ft_strcmp("REDIRECTION", tree->type) == 0))
		ft_execute_com(tree, h); 
	if (ft_strcmp("OPERATION_&&", tree->type) == 0)
	{
		tree->status = ft_op_and(tree, h, e, check);
		if (*check)
			exit (tree->status);
	}
	if (ft_strcmp("OPERATION_||", tree->type) == 0)
	{
		dprintf(2, "this is check in }|| %d\n", *check);
		tree->status = ft_op_or(tree, h, e, check);
		if (*check)
		{
			dprintf(2, "entered in the wrong place\n");
			exit (tree->status);
		}
	}
	if (ft_strcmp("VARIABLE", tree->type) == 0)
	{
		// reset_vars(&tree, h);
		// tree->status = ft_variable(tree, h, e, check);
		ft_var_helper(&tree, h, e, check);
	}
	if (tree->redirections == NULL && ft_strcmp("WORD", tree->type) == 0
		&& cmd_check(tree) == 1)
	{
		reset_vars(&tree, h);
		ft_word_handle(tree, h, e, check);
	}
	if ((tree->redirections != NULL && ft_strcmp("WORD", tree->type) == 0)
		|| (ft_strcmp("REDIRECTION", tree->type) == 0 && cmd_check(tree) == 1))
	{
		display_terminal_control_chars();
		if (!*check)
		{
			id = fork();
			if (id == 0)
				ft_execute_redir(tree, h, e);
			ft_signal_ign();
		}
		// if (*check == 0)
		close (tree->fd_list->fd1);
		waitpid(id, &(tree->status), 0);
		ft_word_handle_signal(tree, check);
	}
	if (ft_strcmp("PIPE", tree->type) == 0)
	{
		// dprintf(2, "entered i the pipe\n");
		tree->status = ft_pip(tree, h, e, check);
	}
}

// char **ft_name_split_alloc(char **name_split)
// {
// 	name_split = 
// }

int	ft_hdoc_first(t_list_fd *tmp)
{
	// ft_name_split_alloc();
	tmp->name_split = ft_split("e", 32);
	unlink(tmp->name_split[0]);
	tmp->name_split[0] = ft_name_check(tmp->name_split[0]);
	dprintf(2, "thi si the name %s\n", tmp->name_split[0]);
	tmp->fd = open(tmp->name_split[0], O_RDWR | O_CREAT, 0777);
	if (tmp->fd == -1)
	{
		// dprintf(2, "this is it\n");
		return (1);
	}
	tmp->fd1 = open(tmp->name_split[0], O_RDONLY, 0777);
	if (tmp->fd1 == -1)
		return (1);
	unlink(tmp->name_split[0]);
	return (0);
}

void	ft_hdoc_check_helper(t_list_fd *tmp, t_tree *tree, int *sig_flag)
{
	if (WIFSIGNALED(tree->status))
	{
		tree->status = WTERMSIG(tree->status);
		if (tree->status == 2)
			ft_putstr_fd(1, "\n");
		*sig_flag = 0;
	}
	close (tmp->fd);
}

int	ft_hdoc_child(t_list_fd *tmp, t_env **env, int status, t_tree *tree)
{
	int	id;

	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		ft_hdoc(ft_strdup(tmp->name), tmp->fd, env, status);
		close (tmp->fd1);
		exit (0);
	}
	else if (id < 0)
		return (perror("fork"), 1);
	signal(SIGINT, SIG_IGN);
	waitpid(id, &(tree->status), 0);
	return (id);
}

int	ft_hdoc_process(t_list_fd *tmp, t_tree *tree, t_hdoc_data *h_data, int status)
{
	int	id;

	id = 0;
	if (h_data->prev_fd != -1)
		close (h_data->prev_fd);
	if (!ft_hdoc_first(tmp))
	{
		h_data->prev_fd = tmp->fd1;
		if (tmp->fd == -1 || tmp->fd1 == -1)
			return (perror("minishell: "), 1);
		id = ft_hdoc_child(tmp, h_data->env, status, tree);
		if (id == 1)
			return (1);
		// signal(SIGINT, SIG_IGN);
		// waitpid(id, &(tree->status), 0);
		ft_hdoc_check_helper(tmp, tree, h_data->sig_flag);
	}
	// dprintf(2, "entered one time\n");
	return (0);
}

void	ft_hdoc_check(t_tree *tree, t_hdoc_data *h_data, int status)
{
	t_list_fd	*tmp;
	int			id;

	h_data->prev_fd = -1;
	id = -1;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3 && *(h_data->sig_flag) == 1)
		{
			quote_remove_lst(&tree);
			// print_tree_visual(tree, 1, 1);
			if (ft_hdoc_process(tmp, tree, h_data, status))
				return ;
			// ft_hdoc_process(tmp, tree, h_data, status);
		}
		tmp = tmp->next;
	}
}

int	ft_hdoc_count_second(t_tree *tree)
{
	t_list_fd	*tmp;
	int			lent;

	lent = 0;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3)
			lent++;
		tmp = tmp->next;
	}
	return (lent);
}

int	ft_hdoc_count(t_tree *tree)
{
	int	lent;
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
	return (lent + left_lent + right_lent);
}
