/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_extraction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:59:35 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 10:24:13 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_operator(char *str)
{
	char	*operator;

	if (!str || !*str)
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
	if (!str_word)
		return (NULL);
	return (str_word);
}

char	*extract_variable(char *str)
{
	int		i;
	int		in_quotes;
	char	quote_type;
	char	*word;

	extract_variable_inits(&i, &in_quotes, &quote_type);
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		if (!in_quotes && (str[i] == 32 || is_operator(
					str[i]) || str[i] == '(' || str[i] == ')'))
			break ;
		i++;
	}
	word = ft_substr(str, 0, i);
	if (!word)
		return (NULL);
	return (word);
}

void	ft_exit_empty(int status, int stat)
{
	if (stat != -1)
		exit (stat);
	exit (status);
}

void	if_var(t_tree **tree)
{
	if ((*tree)->var == 1)
	{
		if (has_wild_cards_comarr(tree) == 1)
			handle_wildcards_in_cmdarr(tree);
		if (has_wild_cards_fdlst(tree) == 1)
			handle_wildcards_in_fdlst(tree);
	}
}
