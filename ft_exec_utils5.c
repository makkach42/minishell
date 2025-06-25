/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:30:59 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/25 10:27:32 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_cmd_redir_end(int org_stdout, int org_stdin)
{
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	close (org_stdin);
	close (org_stdout);
}

int	ft_cmd_redir_helper(t_tree *tree, int i)
{
	if (i == 1)
	{
		tree->fd_list->fd = ft_file_check(tree->fd_list->name);
		if (tree->fd_list ->fd == -1)
			return (perror(""), 1);
		dup2(tree->fd_list->fd, 0);
		close(tree->fd_list->fd);
	}
	else if (i == 2)
	{
		tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
		if (tree->fd_list ->fd == -1)
			return (perror(""), 1);
		dup2(tree->fd_list->fd, 1);
		close(tree->fd_list->fd);
	}
	else if (i == 4)
	{
		tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
		if (tree->fd_list ->fd == -1)
			return (perror(""), 1);
		dup2(tree->fd_list->fd, 1);
		close(tree->fd_list->fd);
	}
	return (0);
}

int	ft_cmd_redir_support(t_tree *tree)
{
	if (ft_redir_check(tree->fd_list->redir) == 1)
	{
		if (ft_cmd_redir_helper(tree, 1))
			return (1);
	}
	else if (ft_redir_check(tree->fd_list->redir) == 2)
	{
		if (ft_cmd_redir_helper(tree, 2))
			return (1);
	}
	else if (ft_redir_check(tree->fd_list->redir) == 4)
	{
		if (ft_cmd_redir_helper(tree, 4))
			return (1);
	}
	return (0);
}

int	ft_cmd_redir(t_tree *tree, t_env **h)
{
	int	org_stdout;
	int	org_stdin;
	int	status;
	int	check;

	status = 0;
	check = 0;
	org_stdout = dup(1);
	org_stdin = dup(0);
	if (check_empty(tree->fd_list->name) == 1)
		return (1);
	while (tree->fd_list != NULL && tree->ambiguous == 0)
	{
		if (ft_cmd_redir_support(tree))
		{
			dup2(org_stdin, 0);
			return (1);
		}
		tree->fd_list = tree->fd_list->next;
	}
	if (tree->ambiguous == 1)
		return (1);
	status = ft_cmd_exec(tree, h);
	ft_cmd_redir_end(org_stdout, org_stdin);
	return (status);
}

int	ft_variable(t_tree *tree, t_env **h, char **e, int *check)
{
	if (!tree->command || !tree->command_arr || !tree->command_arr[0] || check_empty(tree->command_arr[0]))
		return (0);
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
