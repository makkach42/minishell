/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_extraction_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:50:28 by makkach           #+#    #+#             */
/*   Updated: 2025/06/29 19:17:26 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

void	if_string_while_loop(int *i, char *str, char **word)
{
	int		open_quotes;
	char	quote_type;

	*i = 0;
	open_quotes = 0;
	while (str[*i])
	{
		if (!open_quotes && (str[*i] == '"' || str[*i] == '\''))
		{
			open_quotes = 1;
			quote_type = str[*i];
		}
		else if (open_quotes && (str[*i] == quote_type))
			open_quotes = 0;
		if (!open_quotes && (str[*i] == 32 || is_operator(str[*i])))
			break ;
		(*i)++;
	}
	*word = ft_substr(str, 0, *i);
	if (!*word)
		return ;
}

static int	else_if_str_is_par(int *i, char *str)
{
	(*i)++;
	while (str[*i] != ')' && str[*i] != '\0')
		(*i)++;
	if (str[*i] == '\0')
		return (1);
	(*i)++;
	if (str[*i] == '\0')
		return (1);
	if (str[*i] == 32)
		return (1);
	else
	{
		while (str[*i] != 32 && str[*i] != '\0')
			(*i)++;
	}
	return (0);
}

void	if_par_loop(int *i, char *str, char **word)
{
	int		open_par;
	int		closed_par;

	open_par = 0;
	closed_par = 0;
	while (((open_par == 0 && closed_par == 0) || (open_par != closed_par)))
	{
		while (str[*i] && str[*i] != '(' && str[*i] != 32 && str[*i] != '\0')
			(*i)++;
		if (str[*i] == 32)
			break ;
		else if (str[*i] == '(')
		{
			if (else_if_str_is_par(i, str) == 1)
				break ;
		}
		if (!str[*i])
			(*i)--;
		(*i)++;
		if (!str[*i])
			break ;
	}
	*word = ft_substr(str, 0, *i);
	if (!*word)
		return ;
}

int	if_redir_in_para(char *str, int *i, int *in_quotes, char *quote_type)
{
	while (str[*i] && (str[*i] == '>' || str[*i] == '<'))
		(*i)++;
	while (str[*i] && str[*i] == 32)
		(*i)++;
	while (str[*i] && str[*i] != 32)
	{
		if (!*in_quotes && (str[*i] == '"' || str[*i] == '\''))
		{
			*in_quotes = 1;
			*quote_type = str[*i];
		}
		else if (*in_quotes && str[*i] == *quote_type)
			*in_quotes = 0;
		if (!is_operator(str[*i]) && !*in_quotes)
			break ;
		(*i)++;
	}
	if (is_operator(str[*i]) && !*in_quotes)
		return (1);
	while (str[*i] && str[*i] == 32)
		(*i)++;
	return (0);
}
