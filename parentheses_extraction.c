/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_extraction.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:42:38 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 09:12:38 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *extract_parenthesis(char *str)
{
	int i;
	int flag;
	int open_par;
	int closed_par;
	char *word;
	i = 0;
	flag = 0;
	open_par = 0;
	closed_par = 0;
	word = NULL;
	if (!str || !*str)
		return (NULL);
	while (str[i] != ')' && str[i] != '\0')
	{
		if (str[i] == '(')
			open_par++;
		i++;
	}
	if (str[i] == ')')
		closed_par++;
	i++;
	while (open_par > closed_par && str[i] != '\0')
	{
		if (str[i] == '(')
			open_par++;			
		if (str[i] == ')')
			closed_par++;
		i++;
	}
	if ((str[i] == '\0' || str[i] == 32))
	{
		word = ft_substr_leak(str, 0, i, 588);
		return (word);
	}
	else
	{
		open_par = 1;
		closed_par = 1;
		while (str[i] != '\0' && flag == 0)
		{
			if (str[i] == '(')
				open_par++;
			if (str[i] == ')')
				closed_par++;
			i++;
			if (str[i] == 32 && open_par == closed_par)
				flag = 1;
		}
		word = ft_substr_leak(str, 0, i + 1, 605);
	}
	return (word);
}

char *parenthesis_remover(char *str, char *word)
{
	int strlenth;
	char *new_str;
	int word_len;
	if (!str | !*str)
		return (NULL);
	strlenth = ft_strlen(str);
	word_len = ft_strlen(word);
	new_str = ft_substr_leak(str, word_len, strlenth - word_len, 498);
	return (new_str);
}
