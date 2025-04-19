/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_extraction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:39:46 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 10:39:59 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *str_extractor(char *str)
{
	int i;
	int flag;
	char *word;
	int quote_counter;
	i = 1;
	flag = 0;
	if (!str || !*str)
		return (NULL);
	while (str[i] != '\"' && str[i] != '\0')
		i++;
	if (str[i] != '\0' && str[i + 1] && (str[i + 1] == 32 || str[i + 1] == '\0'))
	{
		word = ft_substr_leak(str, 0, i + 1, 456);
		return (word);
	}
	else
	{
		quote_counter = 2;
		while (str[i] != '\0' && flag == 0)
		{
			if (str[i] == '\"')
				quote_counter++;
			i++;
			if (str[i] == 32 && quote_counter % 2 != 0)
				flag = 1;
		}
		word = ft_substr_leak(str, 0, i + 1, 470);
	}
	return (word);
}
char *str_remover(char *str, char *word)
{
	int strlenth;
	char *new_str;
	int word_len;
	if (!str | !*str)
		return (NULL);
	strlenth = ft_strlen(str);
	word_len = ft_strlen(word);
	new_str = ft_substr_leak(str, word_len, strlenth - word_len, 485);
	return (new_str);
}