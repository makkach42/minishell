/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:38:41 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/01 10:05:08 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	delimiter(char c, char del)
{
	return (c == del);
}

void	ft_free_array(char **p)
{
	int	i;

	i = 0;
	if (!p)
		return ;
	while (p[i] != NULL)
	{
		free(p[i]);
		i++;
	}
	free (p);
}

void	ft_execute(t_tree *tree, char **e, int *check, t_hdoc_data *h_data)
{
	if (!tree)
		return (ft_free_data(h_data),
			free_env(h_data->env), ft_check_exit(check, 1));
	if (!ft_cmd_word_check(tree))
		ft_cmd_word(tree, h_data, check, e);
	else if (ft_strcmp("PARENTHASIS", tree->type) == 0)
	{
		tree->status = ft_parenthasis(tree, h_data, e, check);
		ft_check_exit(check, tree->status);
	}
	else if (ft_strcmp("OPERATION_&&", tree->type) == 0)
	{
		tree->status = ft_op_and(tree, h_data, e);
		ft_check_exit(check, tree->status);
	}
	else if (ft_strcmp("OPERATION_||", tree->type) == 0)
	{
		tree->status = ft_op_or(tree, h_data, e);
		ft_check_exit(check, tree->status);
	}
	else if (ft_strcmp("VARIABLE", tree->type) == 0)
		ft_var_helper(&tree, e, check, h_data);
	else if (ft_strcmp("PIPE", tree->type) == 0)
		tree->status = ft_pip(tree, h_data, e, check);
}

int	ft_exit(char **s, t_hdoc_data *h_data, int status)
{
	int	m;

	m = 0;
	if (isatty(0) && isatty(1))
		ft_putstr_fd(1, "exit\n");
	s[1] = ft_skip_space(s[1]);
	if (s[1] == NULL)
		ft_exit_empty(status, h_data->stat);
	else
	{
		if (ft_check_string(s[1]) != 0)
			return (ft_exit_error(1, s));
		else if (s[2] != NULL)
			return (ft_exit_error(2, s));
		else
		{
			m = ft_modulo(s[1]);
			ft_exit_m(m, h_data->env);
		}
	}
	return (0);
}

void	ft_word_expand(t_tree *tree, t_hdoc_data *h_data)
{
	int		i;

	i = 0;
	while ((tree)->command_arr[i])
		i++;
	i--;
	while (i >= 0)
	{
		if (tree->command_arr)
		{
			if (ft_strchr((tree)->command_arr[i], '$'))
			{
				if_dollar_two(tree, i, h_data);
			}
			else 
				reset_var_remove_quotes(&tree);
		}
		i--;
	}
}
