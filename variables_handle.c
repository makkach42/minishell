/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:20:09 by makkach           #+#    #+#             */
/*   Updated: 2025/04/29 15:51:09 by makkach          ###   ########.fr       */
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

void	variable_expantion(t_list **head, t_env **env)
{
	t_list	*tmp;
	char	*variable_name;
	char	*env_value;
	char	*new_name;

	tmp = *head;
	variable_name = NULL;
	new_name = NULL;
	while (tmp && ft_strcmp(tmp->token, "VARIABLE"))
		tmp = tmp->next;
	if (tmp && ((tmp->prev && (
					ft_strcmp(tmp->prev->data, "<<"))) || !tmp->prev))
	{
		variable_name = ft_substr(tmp->data, 1, ft_strlen(tmp->data) - 1);
		env_value = get_env_value(variable_name, env);
		if (env_value && countwords(env_value, 32) != 1)
			(free(env_value), new_name = ft_strjoin(
					"$", variable_name), env_value = ft_strdup(new_name));
		if (env_value)
			if_env_value(tmp, &env_value);
		(free(variable_name), free(new_name));
	}
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

int	variable_in_word(t_list **head, t_env **env)
{
	t_list	*tmp;
	int		result;

	tmp = *head;
	while (tmp)
	{
		if (!ft_strcmp(tmp->token, "WORD") && check_for_variable(tmp->data))
		{
			result = process_word_variable(tmp, env);
			if (result == -1)
				return (-1);
		}
		tmp = tmp->next;
	}
	return (0);
}
