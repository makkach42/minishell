/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils12.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:43:13 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/06 11:05:46 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_first_child(t_tree *tree, int *check, char **e, t_hdoc_data *h_data)
{
	signal(SIGINT, SIG_DFL);
	*check = 1;
	dup2(tree->fd[1], 1);
	close (tree->fd[1]);
	close (tree->fd[0]);
	ft_execute(tree->left, e, check, h_data);
	exit (tree->left->status);
}

void	ft_second_child(t_tree *tree, int *check, char **e, t_hdoc_data *h_data)
{
	signal(SIGINT, SIG_DFL);
	*check = 1;
	dup2(tree->fd[0], 0);
	close (tree->fd[0]);
	close (tree->fd[1]);
	ft_execute(tree->right, e, check, h_data);
	exit(tree->right->status);
}

int	ft_n_return(int org_stdin, int org_stdout, int *check)
{
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	if (*check)
		exit (1);
	return (1);
}

int	ft_para_signal(int status, int org_stdout, int org_stdin)
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
	else
		status = WEXITSTATUS(status);
	dup2(org_stdout, 1);
	dup2(org_stdin, 0);
	close (org_stdout);
	close (org_stdin);
	return (status);
}

void	ft_para_child(t_tree *tree, int *check, t_hdoc_data *h_data, char **e)
{
	signal(SIGINT, SIG_DFL);
	*check = 1;
	if (tree->left)
	{
		tree->left->status = tree->status;
		ft_execute(tree->left, e, check, h_data);
	}
}
