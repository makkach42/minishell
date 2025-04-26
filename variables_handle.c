/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:20:09 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:11:50 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_search(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (!ft_strcmp(tmp->token, "VARIABLE"))
			break ;
		tmp = tmp->next;
	}
	if (tmp)
		return (1);
	return (0);
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

static char	*get_env_value(char *variable_name, char **ev)
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

void	variable_expantion(t_list **head, char **ev)
{
	t_list	*tmp;
	char	*variable_name;
	char	*tmp_char;
	char	*env_value;

	tmp = *head;
	variable_name = NULL;
	while (tmp && ft_strcmp(tmp->token, "VARIABLE"))
		tmp = tmp->next;
	if (tmp && tmp->prev && (ft_strcmp(tmp->prev->data, "<<")))
	{
		variable_name = ft_substr(
				tmp->data, 1, ft_strlen(tmp->data) - 1);
		env_value = get_env_value(variable_name, ev);
		if (env_value)
		{
			tmp_char = tmp->data;
			tmp->data = env_value;
			free(tmp_char);
			tmp_char = tmp->data;
			tmp->data = ft_strtrim(tmp->data, "=");
			free(tmp_char);
		}
		free(variable_name);
	}
}

static int	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (
			c >= 'A' && c <= 'Z') || (
			c >= '0' && c <= '9'));
}

void	update_quote_state(char c, int *in_quote, char *quote_type)
{
	if (*in_quote == 0 && (c == '"' || c == '\''))
	{
		*quote_type = c;
		*in_quote = 1;
	}
	else if (*in_quote && c == *quote_type)
	{
		*in_quote = 0;
		*quote_type = '\0';
	}
}

static int	get_var_info(char *str, int i, char **var_name)
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

static char	**prepare_parts(char *str, int i, int j)
{
	char	**parts;

	parts = malloc(sizeof(char *) * 2);
	if (!parts)
		return (NULL);
	parts[0] = ft_substr(str, 0, i);
	parts[1] = ft_substr(str, j, ft_strlen(str) - j);
	return (parts);
}

static char	*find_var_value(char *var_name, char **argev)
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

static int	process_variable(t_list *tmp, int i, char **argev)
{
	int		j;
	char	*var_name;
	char	**parts;
	char	*val;
	char	*new_str;

	j = get_var_info(tmp->data, i, &var_name);
	if (j == -1)
		return (-1);
	parts = prepare_parts(tmp->data, i, j);
	val = find_var_value(var_name, argev);
	if (!val)
		new_str = ft_strjoin(parts[0], parts[1]);
	else
	{
		new_str = ft_strjoin(parts[0], val);
		free(val);
		val = new_str;
		new_str = ft_strjoin(val, parts[1]);
		free(val);
	}
	(free(var_name), free(parts[0]), free(parts[1]), free(parts));
	free(tmp->data);
	tmp->data = new_str;
	return (0);
}

int	variable_in_word(t_list **head, char **argev)
{
	t_list	*tmp;
	int		i;
	char	quote_type;
	int		in_quote;

	tmp = *head;
	while (tmp)
	{
		if (!ft_strcmp(tmp->token, "WORD") && check_for_variable(tmp->data))
		{
			i = 0;
			in_quote = 0;
			quote_type = '\0';
			while (tmp->data[i])
			{
				update_quote_state(tmp->data[i], &in_quote, &quote_type);
				if ((tmp->data[i] == '$' && (in_quote && quote_type == '"')
					) || (tmp->data[i] == '$' && !in_quote))
				{
					if (process_variable(tmp, i, argev) == -1)
						return (-1);
					i = 0;
					continue ;
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
