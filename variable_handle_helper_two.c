/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper_two.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:56:56 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:57:46 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_variable(t_list *tmp, int i, char **argev)
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

int	process_word_variable(t_list *tmp, char **argev)
{
	int		i;
	char	quote_type;
	int		in_quote;

	i = 0;
	in_quote = 0;
	quote_type = '\0';
	while (tmp->data[i])
	{
		update_quote_state(tmp->data[i], &in_quote, &quote_type);
		if ((tmp->data[i] == '$' && (in_quote && quote_type == '"'))
			|| (tmp->data[i] == '$' && !in_quote))
		{
			if (process_variable(tmp, i, argev) == -1)
				return (-1);
			i = 0;
			continue ;
		}
		i++;
	}
	return (0);
}
