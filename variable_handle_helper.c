/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:53:49 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:55:06 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *variable_name, char **ev)
{
	int		i;
	char	*value;

	i = 0;
	value = NULL;
	while (ev[i])
	{
		if (!ft_strncmp(ev[i], variable_name, ft_strlen(variable_name)
			) && ev[i][ft_strlen(variable_name)] == '=')
		{
			value = ft_substr(ev[i], ft_strlen(variable_name),
					ft_strlen(ev[i]) - ft_strlen(variable_name));
			return (value);
		}
		i++;
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

char	*find_var_value(char *var_name, char **argev)
{
	int		k;
	int		l;
	char	*value;
	char	*tmp;

	k = 0;
	while (argev[k] && ft_strncmp(argev[k], var_name, ft_strlen(var_name)))
		k++;
	if (!argev[k])
		return (NULL);
	l = 0;
	while (argev[k][l] && argev[k][l] != '=')
		l++;
	value = ft_substr(argev[k], l, ft_strlen(argev[k]) - l);
	tmp = value;
	value = ft_strtrim(value, "=");
	free(tmp);
	return (value);
}
