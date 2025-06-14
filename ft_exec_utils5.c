/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:30:59 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/12 17:58:32 by aakroud          ###   ########.fr       */
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
	org_stdout = 0;
	org_stdin = 0;
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
	if (!tree->command || !tree->command_arr || !tree->command_arr[0])
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

// int	check_amb(t_tree *tree)
// {
// 	int	amb_check;
// 	int	left_check;
// 	int	right_check;

// 	amb_check = 0;
// 	left_check = 0;
// 	right_check = 0;
// 	if (tree->ambiguous == 1)
// 		amb_check = 1;
// 	if (tree->left)
// 		left_check = check_amb(tree->left);
// 	if (tree->right)
// 		right_check = check_amb(tree->right);
// 	return (amb_check || left_check || right_check);
// }

// int	ft_para_redir(t_tree *tree, t_env **h)
// {
// 	int	status;

// 	status = 0;
// 	if (check_amb(tree) == 1)
// 		return (1);
// 	while (tree->fd_list != NULL && check_empty(tree->fd_list->name) != 1)
// 	{
// 		if (ft_redir_check(tree->fd_list->redir) == 1)
// 		{
// 			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
// 			if (tree->fd_list ->fd == -1)
// 			{
// 				ft_putstr_fd(2, "minishell: ");
// 				ft_putstr_fd(2, tree->fd_list->name);
// 				ft_putstr_fd(2, ": No such file or directory");
// 				return (-1);
// 			}
// 			dup2(tree->fd_list->fd, 0);
// 		}
// 		else if (ft_redir_check(tree->fd_list->redir) == 2)
// 		{
// 			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
// 			if (tree->fd_list ->fd == -1)
// 			{
// 				perror("minishell: ");
// 				return (-1);
// 			}
// 			dup2(tree->fd_list->fd, 1);
// 		}
// 		else if (ft_redir_check(tree->fd_list->redir) == 4)
// 		{
// 			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
// 			if (tree->fd_list ->fd == -1)
// 			{
// 				perror("minishell: ");
// 				return (-1);
// 			}
// 			dup2(tree->fd_list->fd, 1);
// 		}
// 		tree->fd_list = tree->fd_list->next;
// 	}
// 	return (status);
// }

// int	ft_parenthasis(t_tree *tree, t_env **h, char **e, int *check)
// {
// 	int	org_stdout;
// 	int	org_stdin;
// 	int	n;

// 	n = 0;
// 	org_stdout = dup(1);
// 	org_stdin = dup(0);
// 	if (!tree)
// 		return (-1);
// 	n = ft_para_redir(tree, h);
// 	if (n)
// 		return (1);
// 	if (tree->left)
// 		ft_execute(tree->left, h, e, check);
// 	if (tree->right)
// 		ft_execute(tree->right, h, e, check);
// 	dup2(org_stdout, 1);
// 	dup2(org_stdin, 0);
// 	return (0);
// }
