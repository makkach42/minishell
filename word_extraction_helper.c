/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_extraction_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:50:28 by makkach           #+#    #+#             */
/*   Updated: 2025/05/08 12:48:54 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	else_if_str_is_quote(int *i, char *str)
{
	(*i)++;
	while (str[*i] != '\"' && str[*i] != '\0')
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

void	if_string_while_loop(int *i, char *str, char **word)
{
	int	count_quotes;

	count_quotes = 0;
	while ((count_quotes == 0 || (count_quotes % 2 != 0)))
	{
		while (str[*i] && str[*i] != '\"' && str[*i] != 32 && str[*i] != '\0')
			(*i)++;
		if (str[*i] == 32)
			break ;
		else if (str[*i] == '\"')
		{
			if (else_if_str_is_quote(i, str) == 1)
				break ;
		}
		if (!str[*i])
			(*i)--;
		(*i)++;
		if (!str[*i])
			break ;
	}
	*word = ft_substr(str, 0, *i);
}

int	else_if_str_is_par(int *i, char *str)
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
}
