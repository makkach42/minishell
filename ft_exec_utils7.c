/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:14:27 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/20 11:55:20 by aakroud          ###   ########.fr       */
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
	ft_exec_redir(tree, h, e);
	exit (0);
}

void	ft_var_helper(t_tree **tree, t_env **h, char **e, int *check)
{
	reset_vars(tree, h);
	(*tree)->status = ft_variable(*tree, h, e, check);
}

// int	ft_op_and(t_tree *tree, t_env **h, char **e, int *check)
// {
// 	int	x;

// 	x = 0;
// 	ft_execute(tree->left, h, e, &x);
// 	if (tree->left->status == 0)
// 	{
// 		ft_execute(tree->right, h, e, &x);
// 		return (tree->right->status);
// 	}
// 	return (tree->left->status);
// }

// int	ft_op_or(t_tree *tree, t_env **h, char **e, int *check)
// {
// 	int	x;

// 	x = 0;
// 	ft_execute(tree->left, h, e, &x);
// 	if (tree->left->status != 0)
// 	{
// 		ft_execute(tree->right, h, e, &x);
// 		return (tree->right->status);
// 	}
// 	return (tree->left->status);
// }

void	ft_word_redir(t_tree *tree, t_env **h, char **e, int *check)
{
	int	id;

	id = 0;
	display_terminal_control_chars();
	if (!*check)
	{
		id = fork();
		if (id == 0)
			ft_execute_redir(tree, h, e);
		ft_signal_ign();
	}
	else
		ft_execute_redir(tree, h, e);
	close (tree->fd_list->fd1);
	waitpid(id, &(tree->status), 0);
	ft_word_handle_signal(tree, check);
}

void	ft_check_exit(int *check, int status)
{
	if (*check)
		exit (status);
}

void	ft_execute(t_tree *tree, t_env **h, char **e, int *check)
{
	if (!tree)
	{
		free_env(h);
		ft_check_exit(check, 1);
		return ;
	}
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections == NULL)
		|| (ft_strcmp("WORD", tree->type) == 0 && cmd_check(tree) == 0))
	{
		reset_vars(&tree, h);
		tree->status = ft_cmd_exec(tree, h);
		ft_check_exit(check, tree->status);
	}
	if (ft_strcmp("PARENTHASIS", tree->type) == 0)
	{
		dprintf(2, "this is check before para %d\n", *check);
		tree->status = ft_parenthasis(tree, h, e, check);
		dprintf(2, "this is check in para %d\n", *check);
		ft_check_exit(check, tree->status);
	}
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections != NULL)
		|| (cmd_check(tree) == 0 && ft_strcmp("REDIRECTION", tree->type) == 0))
	{
		ft_execute_com(tree, h);
		ft_check_exit(check, tree->status);
	}
	if (ft_strcmp("OPERATION_&&", tree->type) == 0)
	{
		tree->status = ft_op_and(tree, h, e, check);
		ft_check_exit(check, tree->status);
	}
	if (ft_strcmp("OPERATION_||", tree->type) == 0)
	{
		tree->status = ft_op_or(tree, h, e, check);
		ft_check_exit(check, tree->status);
	}
	if (ft_strcmp("VARIABLE", tree->type) == 0)
		ft_var_helper(&tree, h, e, check);
	if (tree->redirections == NULL && ft_strcmp("WORD", tree->type) == 0
		&& cmd_check(tree) == 1)
	{
		reset_vars(&tree, h);
		print_tree_visual(tree, 1, 1);
		ft_word_handle(tree, h, e, check);
	}
	if ((tree->redirections != NULL && ft_strcmp("WORD", tree->type) == 0)
		|| (ft_strcmp("REDIRECTION", tree->type) == 0 && cmd_check(tree) == 1))
		ft_word_redir(tree, h, e, check);
	if (ft_strcmp("PIPE", tree->type) == 0)
		tree->status = ft_pip(tree, h, e, check);
}
