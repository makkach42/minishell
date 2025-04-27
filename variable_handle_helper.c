/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:53:49 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 14:09:08 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *variable_name, t_env **env)
{
	char	*value;
	t_env	*tmp;

	value = NULL;
	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, variable_name, ft_strlen(variable_name)
			))
		{
			value = ft_strdup(tmp->value);
			return (value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (
			c >= 'A' && c <= 'Z') || (
			c >= '0' && c <= '9'));
}

int	get_var_info(char *str, int i, char **var_name)
{
	int	j;

	j = i + 1;
	if ((str[j] >= '0' && str[j] <= '9') || !str[j])
		return (-1);
	while (is_valid_var_char(str[j]))
		j++;
	*var_name = ft_substr(str, i + 1, j - (i + 1));
	return (j);
}

char	**prepare_parts(char *str, int i, int j)
{
	char	**parts;

	parts = malloc(sizeof(char *) * 2);
	if (!parts)
		return (NULL);
	parts[0] = ft_substr(str, 0, i);
	parts[1] = ft_substr(str, j, ft_strlen(str) - j);
	return (parts);
}

char	*find_var_value(char *var_name, t_env **env)
{
	int		k;
	char	*value;
	char	*tmp;
	t_env	*tmp_env;

	k = 0;
	tmp_env = *env;
	while (tmp_env && ft_strncmp(tmp_env->key, var_name, ft_strlen(var_name)))
		tmp_env = tmp_env->next;
	if (!tmp_env)
		return (NULL);
	value = ft_strdup(tmp_env->value);
	tmp = value;
	value = ft_strtrim(value, "=");
	free(tmp);
	return (value);
}
