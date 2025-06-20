/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils11.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:21:41 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/20 12:14:23 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_signal_ign(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_exec_redir_support(t_tree *tree, int i)
{
	if (i == 1)
	{
		dup2(tree->fd_list->fd, 0);
		close (tree->fd_list->fd);
	}
	else if (i == 2)
	{
		dup2(tree->fd_list->fd, 1);
		close (tree->fd_list->fd);
	}
	else if (i == 4)
	{
		dup2(tree->fd_list->fd, 1);
		close (tree->fd_list->fd);
	}
}

void	ft_exec_redir_helper(t_tree *tree, int i)
{
	if (i == 3)
	{
		dup2(tree->fd_list->fd1, 0);
		close (tree->fd_list->fd1);
	}
	else if (i == 1)
	{
		if (tree->fd_list ->fd == -1)
			return (perror(""), exit (1));
		ft_exec_redir_support(tree, 1);
	}
	else if (i == 2)
	{
		if (tree->fd_list ->fd == -1)
			return (perror(""), exit (1));
		ft_exec_redir_support(tree, 2);
	}
	else if (i == 4)
	{
		if (tree->fd_list ->fd == -1)
			return (perror(""), exit (1));
		ft_exec_redir_support(tree, 4);
	}
}

int	ft_op_and(t_tree *tree, t_env **h, char **e, int *check)
{
	int	x;

	x = 0;
	ft_execute(tree->left, h, e, &x);
	if (tree->left->status == 0)
	{
		ft_execute(tree->right, h, e, &x);
		return (tree->right->status);
	}
	return (tree->left->status);
}

int	ft_op_or(t_tree *tree, t_env **h, char **e, int *check)
{
	int	x;

	x = 0;
	ft_execute(tree->left, h, e, &x);
	if (tree->left->status != 0)
	{
		ft_execute(tree->right, h, e, &x);
		return (tree->right->status);
	}
	return (tree->left->status);
}
