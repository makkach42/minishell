/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_extraction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:59:35 by makkach           #+#    #+#             */
/*   Updated: 2025/05/16 09:33:59 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_operator(char *str)
{
	char	*operator;

	if (!str | !*str)
		return (NULL);
	operator = NULL;
	if (str[0] == '&' && ((str[1] && str[1] != '&') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '*' && ((str[1] && str[1] != '*') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '|' && ((str[1] && str[1] != '|') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '>' && ((str[1] && str[1] != '>') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '<' && ((str[1] && str[1] != '<') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '&' && str[1] && str[1] == '&')
		operator = ft_substr(str, 0, 2);
	else if (str[0] == '|' && str[1] && str[1] == '|')
		operator = ft_substr(str, 0, 2);
	else if (str[0] == '>' && str[1] && str[1] == '>')
		operator = ft_substr(str, 0, 2);
	else if (str[0] == '<' && str[1] && str[1] == '<')
		operator = ft_substr(str, 0, 2);
	return (operator);
}

char	*remove_operator(char *str, char *word)
{
	int		wordlen;
	int		str_len;
	char	*str_word;

	if (!str || !word)
		return (NULL);
	wordlen = ft_strlen(word);
	str_len = ft_strlen(str);
	str_word = ft_substr(str, wordlen, str_len - wordlen + 1);
	return (str_word);
}

char	*extract_variable(char *str)
{
	int		i;
	char	*word;

	i = 1;
	while (str[i] && str[i] != 32 && !is_operator(str[i]))
	{
		if (str[i] >= '0' && str[i] <= '9' && str[i - 1] == '$')
		{
			i++;
			break ;
		}
		i++;
	}
	word = ft_substr(str, 0, i);
	return (word);
}
