/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper_two.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:56:56 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 18:01:04 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}

int	if_var_end_is_dollar(char **command_arr,
	int *var_pos, int arr_idx, int var_end)
{
	char	*before;
	char	*after;
	char	*new_str;

	before = ft_substr(command_arr[arr_idx], 0, *var_pos);
	if (!before)
		return (0);
	after = ft_substr(command_arr[arr_idx
		], var_end, ft_strlen(command_arr[
				arr_idx]) - var_end);
	if (!after)
		return (free(before), 0);
	new_str = ft_strjoin(before, after);
	if (!new_str)
		return (free(before), free(after), 0);
	free(command_arr[arr_idx]);
	command_arr[arr_idx] = new_str;
	return (free(before), free(after), 0);
}

void	var_end_set(char **command_arr, int arr_idx, int *var_end)
{
	char	c;

	while (command_arr[arr_idx][*var_end])
	{
		c = command_arr[arr_idx][*var_end];
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'
			) || ((c >= '0' && c <= '9')) || c == '_')
		{
			if (c >= '0' && c <= '9' && command_arr[
					arr_idx][*var_end - 1] == '$')
			{
				(*var_end)++;
				break ;
			}
			else
				(*var_end)++;
		}
		else
			break ;
	}
}

void	extract_var_value(t_env **env, char *var_name, char **var_value)
{
	t_env	*tmp_env;
	int		i;

	tmp_env = *env;
	while (tmp_env)
	{
		if (ft_strcmp(tmp_env->key, var_name) == 0)
		{
			(*var_value) = ft_strdup(tmp_env->value);
			i = 0;
			while ((*var_value)[i])
			{
				if ((*var_value)[i] == '"')
					(*var_value)[i] = 10;
				else if ((*var_value)[i] == '\'')
					(*var_value)[i] = 11;
				i++;
			}
			break ;
		}
		tmp_env = tmp_env->next;
	}
}
