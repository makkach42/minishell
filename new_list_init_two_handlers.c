/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_two_handlers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:17:27 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 11:17:41 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cut_word_str_handle_quotes_two(char **str, char **word, int i)
{
	char	*tmp_char;

	*word = ft_substr(*str, 0, i);
	if (!*word)
		return (1);
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	if (!*word)
		return (1);
	tmp_char = *str;
	*str = ft_substr(*str, i, ft_strlen(*str) - i);
	free(tmp_char);
	if (!*str)
		return (1);
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp_char);
	if (!*str)
		return (1);
	return (0);
}

void	handle_quotes_two_inits(int *i, int *in_quotes)
{
	*i = -1;
	*in_quotes = 0;
}

void	handle_quotes_two(char **word, char **str)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	handle_quotes_two_inits(&i, &in_quotes);
	while ((*str)[++i])
	{
		if (!in_quotes && ((*str)[i] == '"' || (*str)[i] == '\''))
		{
			in_quotes = 1;
			quote_type = (*str)[i];
		}
		else if (in_quotes && (*str)[i] == quote_type)
			in_quotes = 0;
		if (!in_quotes)
		{
			i++;
			if ((*str)[i] == 32 || is_operator((*str)[i]
					) || (*str)[i] == '(' || (*str)[i] == ')')
				break ;
			i--;
		}
	}
	if (cut_word_str_handle_quotes_two(str, word, i))
		return ;
}

int	handle_else_two_cut_word_str(char **word, char **str, int i)
{
	char	*tmp;

	*word = ft_substr(*str, 0, i);
	if (!*word)
		return (1);
	tmp = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp);
	if (!*word)
		return (1);
	tmp = *str;
	*str = ft_substr(*str, i, ft_strlen(*str) - i);
	free(tmp);
	if (!*str)
		return (1);
	tmp = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp);
	if (!*str)
		return (1);
	return (0);
}

void	handle_else_two(char **str, char **word)
{
	int		i;

	i = 0;
	while ((*str)[i] && !is_operator((*str)[i]) && (*str
			)[i] != '(' && (*str)[i] != ')' && (*str
			)[i] != '$' && (*str)[i] != 32)
		i++;
	handle_else_two_cut_word_str(word, str, i);
}
