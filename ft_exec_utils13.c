/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils13.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:15:04 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/24 16:58:05 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

void	ft_exec_test2(t_tree *tree, int *test, t_hdoc_data *h_data)
{
	int	id1;
	int	id2;
	int	status;
	int	status1;

	id1 = 0;
	id2 = 0;
	status = 0;
	status1 = 0;
	id1 = fork();
	if (id1 == -1)
	{
		perror("fork: ");
		if (*test)
			exit (1);
		return ;
	}
	if (id1 == 0)
	{
		*test = 1;
		ft_exec_test(tree->left, test, h_data);
		exit (0);
	}
	id2 = fork();
	if (id2 == -1)
	{
		perror("fork: ");
		if (*test)
			exit (1);
		return ;
	}
	if (id2 == 0)
	{
		*test = 1;
		ft_exec_test(tree->right, test, h_data);
		exit (0);
	}
	waitpid(id1, &status, 0);
	waitpid(id2, &status1, 0);
	status = WEXITSTATUS(status);
	status1 = WEXITSTATUS(status1);
	if (*test)
	{
		if (status)
			exit (status);
		else
			exit (status1);
	}
	if (status == 1 || status1 == 1)
	{
		*(h_data->sig_flag) = 0;
	}
	return ;
}

void	ft_exec_test(t_tree *tree, int *test, t_hdoc_data *h_data)
{
	if (!tree)
		return ;
	if (ft_strcmp("PIPE", tree->type) == 0)
	{
		ft_exec_test2(tree, test, h_data);
	}
	return ;
}

void	ft_free_data(t_hdoc_data *h_data)
{
	free (h_data->sig_flag);
	h_data->sig_flag = NULL;
	free (h_data);
	h_data = NULL;
}
