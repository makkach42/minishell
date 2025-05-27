/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:45:05 by makkach           #+#    #+#             */
/*   Updated: 2025/05/27 12:11:05 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_quote_handle(char *str)
{
	int		i;
	int		in_quotes;
	char	quote_type;
	char	*tmp_char;
	char	*word;

	i = 0;
	in_quotes = 0;
	word = NULL;
	tmp_char = NULL;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			break ;
		i++;
	}
	new_quote_word_extractor(tmp_char, word, str, i);
	in_quotes = 0;
	return (word);
}

char	*new_parenthasis_handle(char *str)
{
	int		i;
	int		open_par;
	int		closed_par;
	char	*tmp_char;
	char	*word;

	i = 0;
	open_par = 0;
	closed_par = 0;
	word = NULL;
	tmp_char = NULL;
	while (str[i])
	{
		if (str[i] == '(')
			open_par++;
		else if (str[i] == ')')
			closed_par++;
		if (open_par == closed_par && open_par != 0)
			break ;
		i++;
	}
	new_parenthasis_word_extractor(word, tmp_char, i, str);
	return (word);
}

char	*new_variable_handle(char *str)
{
	int		i;
	char	*word;

	word = NULL;
	i = 1;
	while (str[i])
	{
		if (!(str[i] >= 'a' && str[i] <= 'z') && !(
				str[i] >= 'A' && str[i] <= 'Z') && !(
				str[i] >= '0' && str[i] <= '9'))
			break ;
		i++;
	}
	new_variable_word_extractor(str, word, i);
	return (word);
}

char	*plus_handle(char *str)
{
	char	*tmp_char;
	char	*word;

	word = NULL;
	if (str[1] == '=')
	{
		tmp_char = word;
		word = ft_substr(str, 0, 2);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
	}
	else
	{
		tmp_char = word;
		word = ft_substr(str, 0, 1);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
	}
	plus_handle_word_extractor(tmp_char, str, word);
	return (word);
}

char	*new_operator_handle(char *str)
{
	char	*tmp_char;
	char	*word;

	word = NULL;
	tmp_char = NULL;
	if (str[0] == '&' && ((str[1] && str[1] != '&') || !str[1]))
		word = ft_substr(str, 0, 1);
	else if (str[0] == '*' && ((str[1] && str[1] != '*') || !str[1]))
		word = ft_substr(str, 0, 1);
	else if (str[0] == '|' && ((str[1] && str[1] != '|') || !str[1]))
		word = ft_substr(str, 0, 1);
	else if (str[0] == '>' && ((str[1] && str[1] != '>') || !str[1]))
		word = ft_substr(str, 0, 1);
	else if (str[0] == '<' && ((str[1] && str[1] != '<') || !str[1]))
		word = ft_substr(str, 0, 1);
	else if (str[0] == '&' && str[1] && str[1] == '&')
		word = ft_substr(str, 0, 2);
	else if (str[0] == '|' && str[1] && str[1] == '|')
		word = ft_substr(str, 0, 2);
	else if (str[0] == '>' && str[1] && str[1] == '>')
		word = ft_substr(str, 0, 2);
	else if (str[0] == '<' && str[1] && str[1] == '<')
		word = ft_substr(str, 0, 2);
	new_operator_word_extractor(word, tmp_char, str);
	return (word);
}
