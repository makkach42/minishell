/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:45:05 by makkach           #+#    #+#             */
/*   Updated: 2025/06/06 12:46:11 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	new_quote_handle(char **str, char **word)
// {
// 	int		i;
// 	int		in_quotes;
// 	char	quote_type;

// 	i = 0;
// 	in_quotes = 0;
// 	while ((*str)[i])
// 	{
// 		if (!in_quotes && ((*str)[i] == '"' || (*str)[i] == '\''))
// 		{
// 			in_quotes = 1;
// 			quote_type = (*str)[i];
// 		}
// 		else if (in_quotes && (*str)[i] == quote_type)
// 			break ;
// 		i++;
// 	}
// 	new_quote_word_extractor(word, str, i);
// }

void	new_parenthasis_handle(char **str, char **word)
{
	int		i;
	int		open_par;
	int		closed_par;

	i = 0;
	open_par = 0;
	closed_par = 0;
	while (*str[i])
	{
		if (*str[i] == '(')
			open_par++;
		else if (*str[i] == ')')
			closed_par++;
		if (open_par == closed_par && open_par != 0)
			break ;
		i++;
	}
	new_parenthasis_word_extractor(word, i, str);
}

void	new_variable_handle(char **str, char **word)
{
	int		i;

	i = 1;
	while ((*str)[i])
	{
		if (!((*str)[i] >= 'a' && (*str)[i] <= 'z') && !(
				(*str)[i] >= 'A' && (*str)[i] <= 'Z') && !(
				(*str)[i] >= '0' && (*str)[i] <= '9'))
			break ;
		i++;
	}
	new_variable_word_extractor(str, word, i);
}

// void	plus_handle(char **str, char **word)
// {
// 	char	*tmp_char;

// 	if (*str[1] == '=')
// 	{
// 		tmp_char = *word;
// 		*word = ft_substr(*str, 0, 2);
// 		free(tmp_char);
// 		tmp_char = *word;
// 		*word = ft_strtrim(*word, " ");
// 		free(tmp_char);
// 	}
// 	else
// 	{
// 		tmp_char = *word;
// 		*word = ft_substr(*str, 0, 1);
// 		free(tmp_char);
// 		tmp_char = *word;
// 		*word = ft_strtrim(*word, " ");
// 		free(tmp_char);
// 	}
// 	plus_handle_word_extractor(str, word);
// }

void	new_operator_handle(char **str, char **word)
{
	if (*str[0] == '&' && ((*str[1] && *str[1] != '&') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '*' && ((*str[1] && *str[1] != '*') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '|' && ((*str[1] && *str[1] != '|') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '>' && ((*str[1] && *str[1] != '>') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '<' && ((*str[1] && *str[1] != '<') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '&' && *str[1] && *str[1] == '&')
		*word = ft_substr(*str, 0, 2);
	else if (*str[0] == '|' && *str[1] && *str[1] == '|')
		*word = ft_substr(*str, 0, 2);
	else if (*str[0] == '>' && *str[1] && *str[1] == '>')
		*word = ft_substr(*str, 0, 2);
	else if (*str[0] == '<' && *str[1] && *str[1] == '<')
		*word = ft_substr(*str, 0, 2);
	if (!*word)
		return ;
	new_operator_word_extractor(word, str);
}
