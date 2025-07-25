/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils10.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:19:50 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/06 10:34:30 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_wait_for_child(int status, int status1, int *check, int *s)
{
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		if (status == 2)
			g_global_status = SIGINT;
		else if (status == 3)
			g_global_status = SIGQUIT;
		status += 128;
	}
	if (WIFSIGNALED(status1))
	{
		status1 = WTERMSIG(status1);
		if (status1 == 2)
			g_global_status = SIGINT;
		else if (status1 == 3)
			g_global_status = SIGQUIT;
	}
	if (!WIFSIGNALED(status))
		status = WEXITSTATUS(status);
	*s = status;
	if (*check)
		exit (status);
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
		ft_exec(tree, *h, env);
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

int	ft_para_redir(t_tree *tree)
{
	int			status;
	t_list_fd	*tmp;

	status = 0;
	tmp = tree->fd_list;
	if (check_amb(tree) == 1)
		return (1);
	while (tmp != NULL && check_empty(tmp->name) != 1)
	{
		status = ft_cmd_redir_support(tmp);
		tmp = tmp->next;
	}
	return (status);
}

int	ft_parenthasis(t_tree *tree, t_hdoc_data *h_data, char **e, int *check)
{
	int	org_stdout;
	int	org_stdin;
	int	n;
	int	id1;
	int	status;

	n = 0;
	id1 = 0;
	status = 0;
	org_stdout = dup(1);
	org_stdin = dup(0);
	ft_signal_ign();
	if (!tree)
		return (-1);
	n = ft_para_redir(tree);
	if (n)
		return (ft_n_return(org_stdin, org_stdout, check));
	id1 = fork();
	if (id1 == 0)
		return (close (org_stdout), close (org_stdin),
			ft_para_child(tree, check, h_data, e), 0);
	waitpid(id1, &status, 0);
	return (ft_para_signal(status, org_stdout, org_stdin));
}
