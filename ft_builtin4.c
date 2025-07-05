/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:27:00 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/03 21:46:21 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **s, t_env *h, int var)
{
	char		*t;
	t_env		*n;
	char		*past;
	static char	*tmp;

	n = ft_check(h, "1PWD");
	past = ft_str_back(n->value);
	tmp = n->value;
	if (s[1] == NULL || (check_empty(s[1]) && var))
		return (free(past), ft_cd_helper(h));
	else
	{
		if (chdir(s[1]) == -1)
			return (ft_chdir_fail(s, past));
		t = getcwd(NULL, 0);
		if (t)
			return (free (tmp), free (past), n->value = t, 0);
		else
			return (ft_cd_fail(n, s, past));
	}
	return (0);
}

void	ft_f_node(t_env *node)
{
	free (node->key);
	free (node->value);
	free (node);
	node = NULL;
}

int	ft_cmd_exec(t_tree *tree, t_hdoc_data *h_data)
{
	int	status;

	status = 0;
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		status = ft_cd(tree->command_arr, *(h_data->env), tree->var);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		status = ft_echo(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		status = ft_env(*(h_data->env));
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		status = ft_exit(tree->command_arr, h_data, tree->status);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		status = ft_export(tree->command_arr, *(h_data->env), tree);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		status = ft_pwd(*(h_data->env));
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		status = ft_unset(h_data->env, tree->command_arr);
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

int	remove_dollar(char **str, int i)
{
	t_rm_dollar	x;

	x.j = i;
	while ((*str)[x.j] == '$')
		x.j++;
	if (x.j % 2 == 1 && (*str)[x.j] && (
			(*str)[x.j] == '"' || (*str)[x.j] == '\''))
	{
		x.before = ft_substr((*str), 0, i);
		if (!x.before)
			return (1);
		x.dollars = ft_substr((*str), i, x.j - 1);
		if (!x.dollars)
			return (free(x.before), 1);
		x.after = ft_substr((*str), x.j, ft_strlen((*str)) - x.j);
		if (!x.after)
			return (free(x.before), free(x.dollars), 1);
		x.new_str = ft_strjoin_three(x.before, x.dollars, x.after);
		if (!x.new_str)
			return (free(x.before), free(x.after), free(x.dollars), 1);
		free((*str));
		(*str) = x.new_str;
		(free(x.before), free(x.after), free(x.dollars));
	}
	return (0);
}
