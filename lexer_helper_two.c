/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:01:01 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 14:43:06 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_redirection_recognizer(int *i, int *in_quotes)
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

static void	parenthesis_recognizer_inits(
		int *i, int *j, int *flag, int *open_par)
{
	*i = 0;
	*j = 0;
	*flag = 0;
	*open_par = 0;
}

static int	while_loop(char **str,
	char *quote_type, int *in_quotes, int *open_par)
{
	int	i;
	int	closed_par;
	int	flag;

	i = 0;
	closed_par = 0;
	flag = 0;
	while ((*str)[i])
	{
		if (((*str)[i] == '"' || (*str)[i] == '\'') && !*in_quotes)
			open_quotes(in_quotes, quote_type, str, &i);
		else if (in_quotes && (*str)[i] == *quote_type)
			*in_quotes = 0;
		if (((*str)[i] == '(' || (*str)[i] == ')') && !*in_quotes)
			parenthesis_recognizer_helper(open_par, &closed_par, &i, (*str));
		if (*open_par != 0 || closed_par != 0)
		{
			flag = 1;
			break ;
		}
		i++;
	}
	if (flag == 1)
		return (1);
	return (0);
}

int	parenthasis_recognizer(char *str)
{
	int		flag;
	int		open_par;
	int		closed_par;
	int		in_quotes;
	char	quote_type;

	parenthesis_recognizer_inits(&in_quotes, &closed_par, &flag, &open_par);
	if (!str)
		return (0);
	if (*str == '(' || *str == ')')
		return (1);
	return (while_loop(&str, &quote_type, &in_quotes, &open_par));
}
