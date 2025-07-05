/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:30:59 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/03 18:45:14 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cmd_redir_end(int org_stdout, int org_stdin)
{
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	close (org_stdin);
	close (org_stdout);
}

int	ft_cmd_redir_helper(t_list_fd *tmp, int i)
{
	if (i == 1)
	{
		tmp->fd = ft_file_check(tmp->name);
		if (tmp->fd == -1)
			return (perror(""), 1);
		dup2(tmp->fd, 0);
		close(tmp->fd);
	}
	else if (i == 2)
	{
		tmp->fd = ft_file_create(tmp->name, 1);
		if (tmp ->fd == -1)
			return (perror(""), 1);
		dup2(tmp->fd, 1);
		close(tmp->fd);
	}
	else if (i == 4)
	{
		tmp->fd = ft_file_create(tmp->name, 2);
		if (tmp ->fd == -1)
			return (perror(""), 1);
		dup2(tmp->fd, 1);
		close(tmp->fd);
	}
	return (0);
}

int	ft_cmd_redir_support(t_list_fd *tmp)
{
	close(tmp->fd1);
	if (ft_redir_check(tmp->redir) == 1)
	{
		if (ft_cmd_redir_helper(tmp, 1))
			return (1);
	}
	else if (ft_redir_check(tmp->redir) == 2)
	{
		if (ft_cmd_redir_helper(tmp, 2))
			return (1);
	}
	else if (ft_redir_check(tmp->redir) == 4)
	{
		if (ft_cmd_redir_helper(tmp, 4))
			return (1);
	}
	return (0);
}

void	ft_close_st(int org_stdout, int org_stdin)
{
	close (org_stdin);
	close (org_stdout);
}

int	ft_cmd_redir(t_tree *tree, t_hdoc_data *h_data)
{
	int			org_stdout;
	int			org_stdin;
	int			check;
	t_list_fd	*tmp;

	check = 0;
	tmp = tree->fd_list;
	org_stdout = dup(1);
	org_stdin = dup(0);
	if (check_empty(tree->fd_list->name) == 1)
		return (1);
	while (tmp != NULL && tree->ambiguous == 0)
	{
		if (ft_cmd_redir_support(tmp))
		{
			dup2(org_stdin, 0);
			return (ft_close_st(org_stdout, org_stdin), 1);
		}
		tmp = tmp->next;
	}
	if (tree->ambiguous == 1)
		return (1);
	return (ft_cmd_exec(tree, h_data),
		ft_cmd_redir_end(org_stdout, org_stdin), 0);
}
