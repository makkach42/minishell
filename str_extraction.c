/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_extraction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:39:46 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 08:35:15 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	str_extractor_helper(char *str, int *i, int *flag, char **word)
// {
// 	int	quote_counter;

// 	quote_counter = 2;
// 	while (str[*i] != '\0' && *flag == 0)
// 	{
// 		if (str[*i] == '\"' || str[*i] == '\'')
// 			quote_counter++;
// 		(*i)++;
// 		if (str[*i] == 32 && quote_counter % 2 != 0)
// 			*flag = 1;
// 	}
// 	*word = ft_substr(str, 0, *i + 1);
// }
void	str_extractor_helper(char *str, int *i, int *flag, char **word)
{
	int	in_quotes;
	char quote_type;

	*flag = 0;
	in_quotes = 0;
	quote_type = 0;
	while (str[*i])
	{
		if (!in_quotes && (str[*i] == '"' || str[*i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[*i];
		}
		else if (in_quotes && (str[*i] == quote_type))
			in_quotes = 0;
		if (!in_quotes && (str[*i] && (str[*i] == 32 || str[*i] == '\0' || is_operator(str[*i]))))
			break ;
		(*i)++;
	}
	*word = ft_substr(str, 0, *i + 1);
}

char	*str_extractor(char *str)
{
	int		i;
	int		in_quotes;
	int		flag;
	char	quote_type;
	char	*word;

	i = 0;
	in_quotes = 0;
	quote_type = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && (str[i] == quote_type))
		{
			in_quotes = 0;
			break ;
		}
		i++;
	}
	if (str[i] != '\0' && str[i + 1] && (
			str[i + 1] == 32 || str[i + 1
			] == '\0' || is_operator(str[i + 1])))
	{
		// printf("aaaaa\n");
		word = ft_substr(str, 0, i + 1);
		return (word);
	}
	else
	{
		i++;
		str_extractor_helper(str, &i, &flag, &word);
	}
	return (word);

}

char	*str_remover(char *str, char *word)
{
	int		strlenth;
	char	*new_str;
	int		word_len;

	if (!str | !*str)
		return (NULL);
	strlenth = ft_strlen(str);
	word_len = ft_strlen(word);
	new_str = ft_substr(str, word_len, strlenth - word_len);
	return (new_str);
}
