/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils15.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:27:18 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/27 11:54:51 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exec_test_error(int *test)
{
	perror("fork: ");
	if (*test)
		exit(1);
}

void	ft_test_children(int status, int status1, int *test, int *sig_flag)
{
	status = WEXITSTATUS(status);
	status1 = WEXITSTATUS(status1);
	if (*test)
	{
		if (status)
			exit(status);
		else
			exit(status1);
	}
	if (status == 1 || status1 == 1)
		*sig_flag = 0;
}

t_pip	*ft_test_x(t_pip *x)
{
	x = malloc(sizeof(t_pip));
	if (!x)
		return (NULL);
	x->id1 = 0;
	x->id2 = 0;
	x->status = 0;
	x->status1 = 0;
	return (x);
}

void	ft_exec_test2(t_tree *tree, int *test, t_hdoc_data *h_data)
{
	t_pip	*x;

	x = ft_test_x(x);
	if (!x)
		return ;
	x->id1 = fork();
	if (x->id1 == -1)
		return (ft_exec_test_error(test));
	if (x->id1 == 0)
	{
		*test = 1;
		return (ft_exec_test(tree->left, test, h_data), exit (0));
	}
	x->id2 = fork();
	if (x->id2 == -1)
		return (ft_exec_test_error(test));
	if (x->id2 == 0)
	{
		*test = 1;
		ft_exec_test(tree->right, test, h_data);
		exit(0);
	}
	waitpid(x->id1, &(x->status), 0);
	waitpid(x->id2, &(x->status1), 0);
	ft_test_children(x->status, x->status1, test, h_data->sig_flag);
}

void	ft_exec_test(t_tree *tree, int *test, t_hdoc_data *h_data)
{
	if (!tree)
		return ;
	if (ft_strcmp("PIPE", tree->type) == 0)
		ft_exec_test2(tree, test, h_data);
}
