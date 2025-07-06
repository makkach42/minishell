/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_extraction.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:42:38 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 10:25:39 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	extract_parenthesis_helper(char *str, int *i,
// 	int *open_par, int *closed_par)
// {
// 	while (str[*i] != '\0' && str[*i] != ')')
// 	{
// 		if (str[*i] == '(')
// 			(*open_par)++;
// 		(*i)++;
// 	}
// 	if (str[*i] == ')')
// 		(*closed_par)++;
// 	(*i)++;
// 	while (*i < (int)ft_strlen(str) && str[(*i)] != '\0' && (
// 			*open_par) > (*closed_par))
// 	{
// 		if (str[(*i)] == '(')
// 			(*open_par)++;
// 		if (str[(*i)] == ')')
// 			(*closed_par)++;
// 		(*i)++;
// 	}
// }

// static void	extract_parenthesis_helper_two(char *str, int *i,
// 	int *flag, char **word)
// {
// 	int	open_par;
// 	int	closed_par;

// 	open_par = 1;
// 	closed_par = 1;
// 	while (str[*i] != '\0' && !is_operator(str[*i]) && (*flag) == 0)
// 	{
// 		if (str[*i] == '(')
// 			open_par++;
// 		if (str[*i] == ')')
// 			closed_par++;
// 		(*i)++;
// 		if (str[*i] == 32 && open_par == closed_par)
// 			(*flag) = 1;
// 	}
// 	*word = ft_substr(str, 0, (*i));
// 	if (!*word)
// 		return ;
// }

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

// char	*extract_parenthesis(char *str)
// {
// 	int		i;
// 	int		flag;
// 	int		open_par;
// 	int		closed_par;
// 	char	*word;

// 	extract_parenthesis_inits(&i, &flag, &open_par, &closed_par);
// 	word = NULL;
// 	if (!str || !*str)
// 		return (NULL);
// 	extract_parenthesis_helper(str, &i, &open_par, &closed_par);
// 	if (i > (int)ft_strlen(str))
// 		return (ft_substr(str, 0, ft_strlen(str)));
// 	if ((str[i] == '\0' || str[i] == 32))
// 	{
// 		word = ft_substr(str, 0, i);
// 		if (!word)
// 			return (NULL);
// 		return (word);
// 	}
// 	else
// 		extract_parenthesis_helper_two(str, &i, &flag, &word);
// 	return (word);
// }

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
