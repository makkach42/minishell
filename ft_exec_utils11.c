/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils11.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:21:41 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/28 17:23:57 by makkach          ###   ########.fr       */
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

int	ft_op_and(t_tree *tree, t_hdoc_data *h_data, char **e)
{
	int	x;

	x = 0;
	ft_execute(tree->left, e, &x, h_data);
	if (tree->left->status == 0)
	{
		ft_execute(tree->right, e, &x, h_data);
		return (tree->right->status);
	}
	return (tree->left->status);
}

int	ft_op_or(t_tree *tree, t_hdoc_data *h_data, char **e)
{
	int	x;

	x = 0;
	ft_execute(tree->left, e, &x, h_data);
	if (tree->left->status != 0)
	{
		ft_execute(tree->right, e, &x, h_data);
		return (tree->right->status);
	}
	return (tree->left->status);
}
