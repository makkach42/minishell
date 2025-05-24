/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:20:09 by makkach           #+#    #+#             */
/*   Updated: 2025/05/24 13:04:35 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_search(t_tree **tree)
{
	int	found_in_current;
	int	found_in_left;
	int	found_in_right;
	int	j;

	found_in_current = 0;
	found_in_left = 0;
	found_in_right = 0;
	j = 0;
	if ((*tree) && (*tree)->command_arr)
	{
		while ((*tree)->command_arr[j])
		{
			if (variable_search_instr((*tree)->command_arr[j]))
				found_in_current = 1;
			j++;
		}
	}
	if ((*tree) && (*tree)->left)
		found_in_left = variable_search(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		found_in_right = variable_search(&(*tree)->right);
	return (found_in_current || found_in_left || found_in_right);
}

int	check_for_variable(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	if (!str[i])
		return (0);
	return (1);
}

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

int	process_array_variable(char **command_arr,
		int arr_idx, int *var_pos, t_env **env)
{
	int		i;
	int		var_end;
	char	*var_name;
	char	*var_value;
	char	*tmp_char;
	char	*before;
	char	*after;
	char	*new_str;
	char	c;
	t_env	*tmp_env;

	if (!command_arr || !command_arr[arr_idx] || (*var_pos) < 0 || 
		!command_arr[arr_idx][(*var_pos)])
		return (-1);
	var_end = (*var_pos) + 1;
	if (command_arr[arr_idx][var_end] == '$')
	{
		before = ft_substr(command_arr[arr_idx], 0, *var_pos);
		if (!before)
			return (0);
		after = ft_substr(command_arr[arr_idx], var_end, ft_strlen(command_arr[arr_idx]) - var_end);
		if (!after)
		{
			free(before);
			return (0);
		}
		new_str = ft_strjoin(before, after);
		if (!new_str)
		{
			free(before);
			free(after);
			return (0);
		}
		free(command_arr[arr_idx]);
		command_arr[arr_idx] = new_str;
		free(before);
		free(after);
		return (0);
	}
	while (command_arr[arr_idx][var_end])
	{
		c = command_arr[arr_idx][var_end];
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
			((c >= '0' && c <= '9')) || c == '_')
		{
			if (c >= '0' && c <= '9' && command_arr[arr_idx][var_end - 1] == '$')
			{
				var_end++;
				break ;
			}
			else                  
			var_end++;	
		}
		else
			break ;
	}
	if (var_end == (*var_pos) + 1)
	{
		if (command_arr[arr_idx][var_end] == '?' || 
			command_arr[arr_idx][var_end] == '@' ||
			command_arr[arr_idx][var_end] == '#')
		{
			var_end++;
		}
	}
	var_name = ft_substr(command_arr[arr_idx], (*var_pos) + 1, var_end - (*var_pos) - 1);
	if (!var_name)
		return (-1);
	// }
	var_value = NULL;
	tmp_env = *env;
	while (tmp_env)
	{
		if (ft_strcmp(tmp_env->key, var_name) == 0)
		{
			var_value = ft_strdup(tmp_env->value);
			i = 0;
			while (var_value[i])
			{
				if (var_value[i] == '"')
					var_value[i] = 10;
				else if (var_value[i] == '\'')
					var_value[i] = 11;
				i++;
			}
			break ;
		}
		tmp_env = tmp_env->next;
	}
	before = ft_substr(command_arr[arr_idx], 0, (*var_pos));
	if (!before)
	{
		free(var_name);
		return (-1);
	}
	after = ft_substr(command_arr[arr_idx], var_end,
			ft_strlen(command_arr[arr_idx]) - var_end);
	if (!after)
	{
		free(var_name);
		free(before);
		return (-1);
	}
	if (var_value)
	{
		if (ft_strchr(var_value, '"') || ft_strchr(var_value, '\''))
		{
			tmp_char = var_value;
			var_value = ft_strjoin_three("\"", var_value, "\"");
			free(tmp_char);
			*var_pos = ft_strlen(var_value);
		}
		else
			*var_pos = ft_strlen(var_value) * -1;
	}
	if (var_value)
		new_str = ft_strjoin_three(before, var_value, after);
	else
		new_str = ft_strjoin(before, after);
	free(var_name);
	free(before);
	free(after);
	free(var_value);
	if (!new_str)
		return (-1);
	free(command_arr[arr_idx]);
	command_arr[arr_idx] = new_str;
	return (0);
}

void	variable_expantion(t_tree **tree, t_env **env)
{
	int		i;
	int		j;
	int		in_quote;
	char	quote_type;

	
	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		variable_expantion(&(*tree)->left, env);
	if ((*tree)->right)
		variable_expantion(&(*tree)->right, env);
	if ((*tree)->command_arr)
	{
		i = 0;
		while ((*tree)->command_arr[i])
		{
			if (!(*tree)->command_arr[i] || (*tree)->command_arr[i][0] == '\0')
			{
				i++;
				continue;
			}
			j = 0;
			in_quote = 0;
			quote_type = '\0';
			while ((*tree)->command_arr[i][j])
			{
				if (!in_quote && ((*tree)->command_arr[i][j] == '\'' || 
								 (*tree)->command_arr[i][j] == '\"'))
				{
					in_quote = 1;
					quote_type = (*tree)->command_arr[i][j];
				}
				else if (in_quote && (*tree)->command_arr[i][j] == quote_type)
				{
					in_quote = 0;
				}
				if ((*tree)->command_arr[i][j] == '$' && 
					(in_quote == 0 || (in_quote && (quote_type == '\"' || 
					(quote_type == '\'' && (*tree)->var != 2)))) && 
					(*tree)->var != 2)
				{
					if ((*tree)->command_arr[i][j + 1] && 
						process_array_variable((*tree)->command_arr, i, &j, env) == -1)
						break;
					if (!variable_search(tree))
						j = 0;
					else
						j = 0;
				}
				if ((*tree)->command_arr[i][j])
					j++;
				else
					break ;
			}
			i++;
		}
	}
}
