/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_extraction.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:42:38 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 13:59:09 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_parenthesis_inits(int *i, int
		*in_quotes, int *open_par, int *closed_par)
{
	*i = 0;
	*open_par = 0;
	*closed_par = 0;
	*in_quotes = 0;
}

char	*extract_parenthesis(char *str)
{
	int		i;
	int		open_par;
	int		closed_par;
	int		in_quotes;
	char	quote_type;

	extract_parenthesis_inits(&i, &in_quotes, &open_par, &closed_par);
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		if (!in_quotes && str[i] == '(')
			open_par++;
		else if (!in_quotes && str[i] == ')')
			closed_par++;
		if (open_par == closed_par && open_par != 0)
			break ;
		i++;
	}
	return (ft_substr(str, 0, i + 1));
}

char	*parenthesis_remover(char *str, char *word)
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
