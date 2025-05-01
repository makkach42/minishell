/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper_two.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:56:56 by makkach           #+#    #+#             */
/*   Updated: 2025/05/01 09:23:27 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_variable(t_list *tmp, int i, t_env **env)
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
	val = find_var_value(var_name, env);
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

int	process_word_variable(t_list *tmp, t_env **env)
{
	int		i;
	char	quote_type;
	int		in_quote;
	t_env	*tmp_env;

	i = 0;
	in_quote = 0;
	quote_type = '\0';
	tmp_env = *env;
	while (tmp->data[i])
	{
		update_quote_state(tmp->data[i], &in_quote, &quote_type);
		if ((tmp->data[i] == '$' && (in_quote && quote_type == '"'))
			|| (tmp->data[i] == '$' && !in_quote))
		{
			if (process_variable(tmp, i, env) == -1)
				return (-1);
			i = 0;
			continue ;
		}
		i++;
	}
	return (0);
}

void	if_token_is_variable(t_list *tmp, t_env **env)
{
	char	*variable_name;
	char	*env_value;
	char	*new_name;

	variable_name = NULL;
	new_name = NULL;
	if (tmp && ((tmp->prev && (
					ft_strcmp(tmp->prev->data, "<<"))) || !tmp->prev))
	{
		variable_name = ft_substr(tmp->data, 1,
				ft_strlen(tmp->data) - 1);
		env_value = get_env_value(variable_name, env);
		if (env_value && countwords(env_value, 32) != 1)
			(free(env_value), new_name = ft_strjoin(
					"$", variable_name),
				env_value = ft_strdup(new_name));
		if (env_value)
			if_env_value(tmp, &env_value);
		(free(variable_name), free(new_name));
	}
}
