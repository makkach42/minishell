/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_extraction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:59:35 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 09:12:31 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_operator(char *str)
{
	int	i;
	char *operator;
	if (!str | !*str)
		return (NULL);
	i = 0;
	operator = NULL;
	if (str[i] == '&' && ((str[i + 1] && str[i + 1] != '&') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 511);
	else if (str[i] == '*' && ((str[i + 1] && str[i + 1] != '*') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 513);
	else if (str[i] == '|' && ((str[i + 1] && str[i + 1] != '|') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 515);
	else if (str[i] == '>' && ((str[i + 1] && str[i + 1] != '>') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 517);
	else if (str[i] == '<' && ((str[i + 1] && str[i + 1] != '<') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1,519);
	else if (str[i] == '&' && str[i + 1] && str[i + 1] == '&')
		operator = ft_substr_leak(str, 0, 2, 521);
	else if (str[i] == '|' && str[i + 1] && str[i + 1] == '|')
		operator = ft_substr_leak(str, 0, 2, 523);
	else if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
		operator = ft_substr_leak(str, 0, 2, 525);
	else if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
		operator = ft_substr_leak(str, 0, 2, 527);
	return (operator);
}
char *remove_operator(char *str, char *word)
{
	int wordlen;
	int str_len;
	char *str_word;
	if (!str || !word)
		return (NULL);
	wordlen = ft_strlen(word);
	str_len = ft_strlen(str);
	str_word = ft_substr_leak(str, wordlen, str_len - wordlen + 1, 541);
	return (str_word);
}
char *extract_variable(char *str)
{
	int i;
	char *word;
	i = 0;
	while (str[i] != 32 && str[i] != '\0')
		i++;
	word = ft_substr_leak(str, 0, i + 1, 552);
	return (word);
}