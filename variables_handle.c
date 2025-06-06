/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:20:09 by makkach           #+#    #+#             */
/*   Updated: 2025/06/06 13:06:57 by makkach          ###   ########.fr       */
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

int	first_return_check(char **command_arr, int arr_idx, int *var_pos)
{
	if (!command_arr || !command_arr[arr_idx
		] || (*var_pos) < 0 || !command_arr[
			arr_idx][(*var_pos)])
		return (1);
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

int	process_array_variable(char **command_arr,
		int arr_idx, int *var_pos, t_env **env)
{
	int		var_end;
	char	*var_name;
	char	*var_value;
	char	*before;
	char	*after;

	if (first_return_check(command_arr, arr_idx, var_pos))
		return (-1);
	var_end = (*var_pos) + 1;
	if (command_arr[arr_idx][var_end] == '$')
		return (if_var_end_is_dollar(command_arr, var_pos, arr_idx, var_end));
	var_end_set(command_arr, arr_idx, &var_end);
	var_name = ft_substr(command_arr[arr_idx],
			(*var_pos) + 1, var_end - (*var_pos) - 1);
	if (!var_name)
		return (-1);
	var_value = NULL;
	extract_var_value(env, var_name, &var_value);
	if (fill_before(&before, &command_arr[arr_idx], var_pos, var_name) == -1)
		return (-1);
	if (fill_after(&after, &command_arr[arr_idx], var_end) == -1)
		return (free(var_name), free(before), -1);
	free(var_name);
	return (new_str_fill(var_value, &command_arr[arr_idx], before, after), 0);
}
