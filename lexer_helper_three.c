/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:07:40 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 12:02:15 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_quotes(int *in_quotes, char *quote_type, char **str, int *i)
{
	*in_quotes = 1;
	*quote_type = (*str)[*i];
}

void	parenthesis_recognizer_helper(int *open_par,
	int *closed_par, int *i, char *str)
{
	if (str[*i] == '(')
		(*open_par)++;
	if (str[*i] == ')')
		(*closed_par)++;
}

void	remove_quotes_from_var_two(char	**arr)
{
	int		i;
	char	*old_cmd;
	int		final_len;
	char	*new_str;

	i = 0;
	if (arr[i])
	{
		if (ft_strchr(arr[i], '"'
			) || ft_strchr(
				arr[i], '\''))
		{
			old_cmd = arr[i];
			final_len = count_filtered_length(old_cmd);
			new_str = create_filtered_string(old_cmd, final_len);
			if (!new_str)
				return ;
			free(arr[i]);
			arr[i] = new_str;
		}
	}
}

void	get_var_name_in_str(char **str, int *j)
{
	while ((*str)[*j] == '$')
		(*j)++;
	while ((*str)[*j])
	{
		if (((*str)[*j] >= 'a' && (*str)[*j] <= 'z') || (
			(*str)[*j] >= 'A' && (*str)[*j] <= 'Z'
			) || (((*str)[*j] >= '0' && (*str)[*j] <= '9')
			) || (*str)[*j] == '?')
		{
			if ((*str)[*j] >= '0' && (*str)[*j] <= '9' && (*str)[*j] == '$')
			{
				(*j)++;
				break ;
			}
			if ((*str)[*j] == '?' && (*str)[*j - 1] != '$')
				break ;
			else
				(*j)++;
		}
		else
			break ;
	}
}

int	expand_str_if_dollar(char **str, int *i, t_rm_dollar *s)
{
	int			n;
	int			j;

	n = *i;
	j = *i;
	get_var_name_in_str(str, &j);
	s->before = ft_substr((*str), 0, n);
	if (!s->before)
		return (-1);
	s->after = ft_substr((*str), j, ft_strlen((*str)) - j);
	if (!s->after)
		return (free(s->before), -1);
	s->new_str = ft_strjoin(s->before, s->after);
	if (!s->new_str)
		return (free(s->before), free(s->after), -1);
	free((*str));
	(*str) = s->new_str;
	free(s->before);
	free(s->after);
	return (0);
}
