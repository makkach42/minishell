/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_extraction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:36:34 by makkach           #+#    #+#             */
/*   Updated: 2025/05/18 09:35:15 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	string_in_word_recognizer(char *str)
{
	int	i;
	int	flag;
	int	quote_counter;

	i = 0;
	flag = 0;
	quote_counter = 0;
	while (str[i] && ((quote_counter % 2 != 0 && quote_counter != 0
			) || flag == 0))
	{
		if (str[i] == '\"')
			quote_counter++;
		if (!str[i] || (
				str[i] == 32 && quote_counter == 0) || (
				str[i] == 32 && quote_counter % 2 == 0))
			flag = 1;
		i++;
	}
	if (quote_counter % 2 == 0 && quote_counter != 0)
		return (1);
	return (0);
}

int	parenthesis_in_word_recogniser(char *str)
{
	int	i;
	int	open_par;
	int	closed_par;

	i = 0;
	open_par = 0;
	closed_par = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '(')
			open_par++;
		if (str[i] == ')')
			closed_par++;
		if (open_par == closed_par && open_par > 0)
		{
			if (str[i + 1] == ' ' || str[i + 1] == '\0')
				break ;
		}
		i++;
	}
	return (open_par == closed_par && open_par > 0);
}

int	operation_or_pipe_before_condition(char *str, int flag)
{
	int	i;

	i = 0;
	if (flag == 1)
	{
		while (str[i] && str[i] != '(')
		{
			if (is_operator(str[i]))
				break ;
			i++;
		}
	}
	else if (flag == 2)
	{
		while ((str[i] != '"' && str[i] != '\'') && str[i])
		{
			if (is_operator(str[i]))
				break ;
			i++;
		}
	}
	if (str[i] == '(' || str[i] == '"' || str[i] == '\'')
		return (0);
	return (1);
}

char	*word_extractor(char *str)
{
	int		i;
	char	*word;

	i = 0;
	if (!str || !*str)
		return (NULL);
	if (string_in_word_recognizer(str
		) && !operation_or_pipe_before_condition(str, 2))
		return (if_string_while_loop(&i, str, &word), word);
	else if (parenthesis_in_word_recogniser(str
		) && !operation_or_pipe_before_condition(str, 1))
	{
		if_par_loop(&i, str, &word);
		return (word);
	}
	else
	{
		while (str[i++] == 32)
			;
		while (str[i] != 32 && !is_operator(str[i]) && str[i] != '\0')
			i++;
		word = ft_substr(str, 0, i);
	}
	return (word);
}

char	*first_word_remover(char *str, char *word)
{
	int		strlenth;
	int		wordlenth;
	char	*new_str;

	if (!str || !word)
		return (NULL);
	strlenth = ft_strlen(str);
	wordlenth = ft_strlen(word);
	new_str = ft_substr(str, wordlenth, strlenth - wordlenth + 1);
	return (new_str);
}
