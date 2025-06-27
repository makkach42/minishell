/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:10:18 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/27 13:43:17 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_close_pip(t_tree *tree)
{
	close (tree->fd[0]);
	close (tree->fd[1]);
}

int	ft_pip(t_tree *tree, t_hdoc_data *h_data, char **e, int *check)
{
	t_pip	*x;

	x = NULL;
	if (!ft_test_x(&x))
		return (1);
	tree->left->status = tree->status;
	if (pipe(tree->fd) == -1)
		perror("minishell: pipe: ");
	ft_signal_ign();
	x->id1 = fork();
	if (x->id1 == -1)
		return (perror("minishell: fork: "), 1);
	if (x->id1 == 0)
		ft_first_child(tree, check, e, h_data);
	x->id2 = fork();
	if (x->id2 == -1)
		return (perror("minishell: fork: "), 1);
	if (x->id2 == 0)
		ft_second_child(tree, check, e, h_data);
	ft_close_pip(tree);
	ft_close_fd(tree);
	if (waitpid(x->id1, &(x->status1), 0) == -1
		|| waitpid(x->id2, &(x->status), 0) == -1)
		return (1);
	return (ft_wait_for_child(x->status, x->status1, check));
}

int	cmd_check(t_tree *tree)
{
	if (!tree)
		return (1);
	if (!tree->command_arr || !tree->command_arr[0])
		return (1);
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		return (0);
	return (1);
}

int	ft_cmd_exec(t_tree *tree, t_env **h)
{
	int	status;

	status = 0;
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		status = ft_cd(tree->command_arr, *h);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		status = ft_echo(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		status = ft_env(*h);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		status = ft_exit(tree->command_arr, h, tree->status);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		status = ft_export(tree->command_arr, *h, tree);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		status = ft_pwd(*h);
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		status = ft_unset(h, tree->command_arr);
	return (status);
}

int	ft_redir_check(char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (1);
	else if (ft_strcmp(str, ">") == 0)
		return (2);
	else if (ft_strcmp(str, "<<") == 0)
		return (3);
	else if (ft_strcmp(str, ">>") == 0)
		return (4);
	return (-1);
}
