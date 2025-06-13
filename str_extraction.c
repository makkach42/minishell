/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_extraction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:39:46 by makkach           #+#    #+#             */
/*   Updated: 2025/06/12 17:13:14 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// char	*str_extractor(char *str)
// {
// 	int		i;
// 	int		in_quotes;
// 	int		flag;
// 	char	quote_type;
// 	char	*word;

// 	i = 0;
// 	in_quotes = 0;
// 	quote_type = 0;
// 	while (str[i])
// 	{
// 		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
// 		{
// 			in_quotes = 1;
// 			quote_type = str[i];
// 		}
// 		else if (in_quotes && (str[i] == quote_type))
// 		{
// 			in_quotes = 0;
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (str[i] != '\0' && str[i + 1] && (
// 			str[i + 1] == 32 || str[i + 1
// 			] == '\0' || is_operator(str[i + 1])))
// 	{
// 		word = ft_substr(str, 0, i + 1);
// 		return (word);
// 	}
// 	else
// 	{
// 		i++;
// 		str_extractor_helper(str, &i, &flag, &word);
// 	}
// 	return (word);
// }
int	process_quotes(char *str, int *in_quotes, char *quote_type)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(*in_quotes) && (str[i] == '"' || str[i] == '\''))
		{
			*in_quotes = 1;
			*quote_type = str[i];
		}
		else if (*in_quotes && (str[i] == *quote_type))
		{
			*in_quotes = 0;
			break ;
		}
		i++;
	}
	return (i);
}

int	check_early_return(char *str, int i)
{
	if (str[i] != '\0' && str[i + 1] && (
			str[i + 1] == 32 || str[i + 1
			] == '\0' || is_operator(str[i + 1])))
		return (1);
	return (0);
}

char	*handle_direct_extraction(char *str, int i)
{
	char	*word;

	word = ft_substr(str, 0, i + 1);
	if (!word)
		return (NULL);
	return (word);
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
	i = process_quotes(str, &in_quotes, &quote_type);
	if (check_early_return(str, i))
		return (handle_direct_extraction(str, i));
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

	if (!str || !*str)
		return (NULL);
	strlenth = ft_strlen(str);
	word_len = ft_strlen(word);
	new_str = ft_substr(str, word_len, strlenth - word_len);
	if (!new_str)
		return (NULL);
	return (new_str);
}
