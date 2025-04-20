/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_extraction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:36:34 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 15:55:06 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	string_in_word_recognizer(char *str)
{
	int	i;
	int	flag;
	int	quote_counter;

	i = 0;
	flag = 0;
	quote_counter = 0;
	while ((quote_counter % 2 != 0 && quote_counter != 0) || flag == 0)
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
	int	flag;
	int	open_par;
	int	closed_par;

	i = 0;
	flag = 0;
	open_par = 0;
	closed_par = 0;
	while ((open_par == closed_par && open_par != 0 && closed_par) || flag == 0)
	{
		if (str[i] == '(')
			open_par++;
		if (str[i] == ')')
			closed_par++;
		if (!str[i] || (
				str[i] == 32 && open_par == 0 && closed_par == 0) || (
				str[i] == 32 && open_par == closed_par))
			flag = 1;
		i++;
	}
	if (open_par == closed_par && (open_par != 0 && closed_par != 0))
		return (1);
	return (0);
}

char	*word_extractor(char *str)
{
	int		i;
	int		flag;
	int		count_quotes;
	int		open_par;
	int		closed_par;
	char	*word;

	i = -1;
	if (!str || !*str)
		return (NULL);
	flag = 0;
	if (string_in_word_recognizer(str) == 1)
	{
		i = 0;
		count_quotes = 0;
		while ((count_quotes == 0 || (count_quotes % 2 != 0)))
		{
			while (str[i] && str[i] != '\"' && str[i] != 32 && str[i] != '\0')
				i++;
			if (str[i] == 32)
				break ;
			else if (str[i] == '\"')
			{
				i++;
				while (str[i] != '\"' && str[i] != '\0')
					i++;
				if (str[i] == '\0')
					break ;
				i++;
				if (str[i] == '\0')
					break ;
				if (str[i] == 32)
					break ;
				else
				{
					while (str[i] != 32 && str[i] != '\0')
						i++;
				}
			}
			if (!str[i])
				i--;
			i++;
			if (!str[i])
				break ;
		}
		word = ft_substr_leak(str, 0, i, 354);
		return (word);
	}
	else if (parenthesis_in_word_recogniser(str) == 1)
	{
		i = 0;
		open_par = 0;
		closed_par = 0;
		while (((open_par == 0 && closed_par == 0) || (open_par != closed_par)))
		{
			while (str[i] && str[i] != '(' && str[i] != 32 && str[i] != '\0')
				i++;
			if (str[i] == 32)
				break ;
			else if (str[i] == '(')
			{
				i++;
				while (str[i] != ')' && str[i] != '\0')
					i++;
				if (str[i] == '\0')
					break ;
				i++;
				if (str[i] == '\0')
					break ;
				if (str[i] == 32)
					break ;
				else
				{
					while (str[i] != 32 && str[i] != '\0')
						i++;
				}
			}
			if (!str[i])
				i--;
			i++;
			if (!str[i])
				break ;
		}
		word = ft_substr_leak(str, 0, i, 392);
		return (word);
	}
	else
	{
		while (str[++i] == 32)
		{
		}
		while (str[i] != 32 && !is_operator(str[i]) && str[i] != '\0')
			i++;
		word = ft_substr_leak(str, 0, i, 400);
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
	new_str = ft_substr_leak(str, wordlenth, strlenth - wordlenth + 1, 415);
	return (new_str);
}
