/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:01:55 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/15 17:10:29 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_array(char **s)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		free (s[i]);
		i++;
	}
	free (s);
}

int	ft_space_check(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == 32)
			return (1);
		i++;
	}
	return (0);
}

void	ft_exec_error(t_tree *tree, char *temp)
{
	tree->command_arr[0] = temp;
	ft_putstr_fd(2, "minishell: ");
	ft_putstr_fd(2, tree->command_arr[0]);
	ft_putstr_fd(2, ": command not found\n");
	free_array(tree->command_arr);
	exit (127);
}

void	ft_exec(t_tree *tree, t_env *h, char **e)
{
	char	*temp;
	t_env	*path;
	char	*env;
	int		flag;

	flag = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	path = ft_check(h, "PATH");
	if (path == NULL || path->value == NULL || check_empty(path->value) == 1)
		env = NULL;
	else
		env = path->value;
	temp = tree->command_arr[0];
	tree->command_arr[0] = ft_cmd_check(env, tree->command_arr[0]);
	if (tree->command_arr[0] == NULL || (tree->var && tree->command_arr[1])
		|| ft_space_check(temp))
		ft_exec_error(tree, temp);
	execve(tree->command_arr[0], tree->command_arr, e);
	ft_exec_error(tree, temp);
}

void	ft_close_fd(t_tree *tree)
{
	if (tree->left)
		ft_close_fd(tree->left);
	if (tree->right)
		ft_close_fd(tree->right);
	ft_close_handle(tree->fd_list);
}
