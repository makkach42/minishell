/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:20:09 by makkach           #+#    #+#             */
/*   Updated: 2025/06/25 11:19:46 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	new_str_fill(char *var_value, char **str, char *before, char *after)
{
	char	*new_str;

	if (var_value)
		new_str = ft_strjoin_three(before, var_value, after);
	else
		new_str = ft_strjoin(before, after);
	(free(before), free(after), free(var_value));
	if (!new_str)
		return (-1);
	free((*str));
	(*str) = new_str;
	return (0);
}

int	fill_before(char **before, char **str, int *var_pos, char *var_name)
{
	*before = ft_substr((*str), 0, (*var_pos));
	if (!*before)
		return (free(var_name), -1);
	return (0);
}

int	fill_after(char **after, char **str, int var_end)
{
	*after = ft_substr((*str), var_end,
			ft_strlen((*str)) - var_end);
	if (!*after)
		return (-1);
	return (0);
}

int	process_variable_expantion(char **command_arr,
		int arr_idx, int *var_pos, t_env **env)
{
	t_expand	tmp;

	tmp.var_end = (*var_pos) + 1;
	if (command_arr[arr_idx][tmp.var_end] == '$')
		return (if_var_end_is_dollar(command_arr,
				var_pos, arr_idx, tmp.var_end));
	var_end_set(command_arr, arr_idx, &tmp.var_end);
	tmp.var_name = ft_substr(command_arr[arr_idx],
			(*var_pos) + 1, tmp.var_end - (*var_pos) - 1);
	if (!tmp.var_name)
		return (-1);
	if (!ft_strcmp(tmp.var_name, "?"))
		return (free(tmp.var_name), *var_pos = -1, 0);
	if (underscore_case_check(tmp.var_name, command_arr, arr_idx))
		return (0);
	tmp.var_value = NULL;
	extract_var_value(env, tmp.var_name, &tmp.var_value);
	if (fill_before(&tmp.before, &command_arr[arr_idx],
			var_pos, tmp.var_name) == -1)
		return (-1);
	if (fill_after(&tmp.after, &command_arr[arr_idx], tmp.var_end) == -1)
		return (free(tmp.var_name), free(tmp.before), -1);
	free(tmp.var_name);
	return (new_str_fill(tmp.var_value, &command_arr[arr_idx],
			tmp.before, tmp.after), 0);
}

int	return_error_cases(char *str, int *i, t_qfilter *qfil, t_par *par)
{
	if ((str[*i] == '>' || str[*i] == '<'
		) && !qfil->in_quotes && par->open_par > par->closed_par)
		if (if_redir_in_para(str, i, &qfil->in_quotes, &qfil->quote_type))
			return (1);
	if ((is_operator(str[*i])
		) && !qfil->in_quotes && str[*i + 1] && str[*i + 1] == ')')
		return (1);
	if (!qfil->in_quotes && str[*i + 1] && str[*i] == '(' && str[*i + 1] == ')')
		return (1);
	if (!qfil->in_quotes && str[*i + 1] && str[*i] == '(' && (
			is_operator(str[*i + 1])))
		return (1);
	return (0);
}
