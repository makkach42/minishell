/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:01:01 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 10:03:34 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_redirection_recognizer(int *i, int *in_quotes)
{
	*i = -1;
	*in_quotes = 0;
}

int	redirection_recognizer(char *str)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	init_redirection_recognizer(&i, &in_quotes);
	if (!str)
		return (0);
	while (str[++i])
	{
		if ((str[i] == '"' || str[i] == '\'') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
		{
			in_quotes = 0;
			quote_type = '\0';
		}
		if ((str[i] == '>' || str[i] == '<') && !in_quotes)
			break ;
	}
	if (str[i] == '>' || str[i] == '<')
		return (1);
	return (0);
}

void	parenthesis_recognizer_inits(int *i, int *j, int *flag, int *open_par)
{
	*i = 0;
	*j = 0;
	*flag = 0;
	*open_par = 0;
}

void	parenthesis_recognizer_helper(int *open_par,
	int *closed_par, int *i, char *str)
{
	if (str[*i] == '(')
		(*open_par)++;
	if (str[*i] == ')')
		(*closed_par)++;
}

int	parenthasis_recognizer(char *str)
{
	int	i;
	int	j;
	int	flag;
	int	open_par;
	int	closed_par;

	parenthesis_recognizer_inits(&i, &j, &flag, &open_par);
	closed_par = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '(' || str[i] == ')')
			parenthesis_recognizer_helper(&open_par, &closed_par, &i, str);
		if (open_par != 0 && closed_par != 0 && open_par == closed_par)
		{
			flag = 1;
			j = i;
			break ;
		}
		i++;
	}
	if (flag == 1 && (str[i + 1] == 32 || str[i + 1] == '\0'))
		return (1);
	return (0);
}
