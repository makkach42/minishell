/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:49:23 by makkach           #+#    #+#             */
/*   Updated: 2025/07/01 14:23:48 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	more_ifs(char *prev_token, char *prev_data, t_list *tmp, int *flag)
{
	if (!*flag && tmp && (!ft_strcmp("PARENTHASIS", prev_token) && (
				!ft_strcmp("PARENTHASIS", tmp->token))))
		(print_syntax_error(), *flag = 1);
	if ((!*flag && tmp && (!ft_strcmp("OPERATION_&&", prev_token
				) || ft_strcmp("OPERATION_||", prev_token) == 0
			) && ((ft_strcmp("OPERATION_||", tmp->token) == 0
				) || !ft_strcmp("OPERATION_&&", tmp->token))))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (ft_strcmp("PIPE", prev_token
			) == 0 && ft_strcmp(prev_token, tmp->token) == 0))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && ((ft_strcmp("OPERATION_&&", prev_token
				) == 0 || ft_strcmp("OPERATION_||", prev_token) == 0
			) && ft_strcmp(tmp->token, "PIPE") == 0))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (ft_strcmp("PIPE", prev_token) == 0 && (
				ft_strcmp(tmp->token, "OPERATION_&&") == 0 || ft_strcmp(
					tmp->token, "OPERATION_||") == 0)))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (!ft_strcmp("REDIRECTION", prev_token) && !ft_strcmp(
				tmp->token, "PIPE")))
		(print_syntax_error(), *flag = 1);
	if (!*flag)
		(even_more_ifs(prev_token, prev_data, tmp, flag));
}

void	while_loop_syntax_error(t_list *tmp,
		char *prev_token, char *prev_data, int *flag)
{
	while (tmp)
	{
		prev_token = tmp->token;
		prev_data = tmp->data;
		tmp = tmp->next;
		if (!*flag && !ft_strcmp(prev_token, "PARENTHASIS"
			) && (parenth_case(prev_data)))
			(print_syntax_error(), *flag = 1);
		if (!*flag && (ft_strcmp("REDIRECTION", prev_token) == 0 && !tmp))
		{
			if (ft_strcmp(prev_data, ">") == 0 && ft_strlen(prev_data) == 1)
				(print_syntax_error(), *flag = 1);
			else if (ft_strcmp(prev_data, "<") == 0 && ft_strlen(
					prev_data) == 1)
				(print_syntax_error(), *flag = 1);
			else
				(print_syntax_error(), *flag = 1);
		}
		if (!*flag && ((!ft_strcmp("OPERATION_&&", prev_token) || !ft_strcmp(
						"OPERATION_||", prev_token)) && !tmp))
			(print_syntax_error(), *flag = 1);
		if (!*flag)
			(more_ifs(prev_token, prev_data, tmp, flag));
	}
}

int	parenth_case(char *str)
{
	t_qfilter	qfil;
	t_par		par;
	int			i;

	qfil.in_quotes = 0;
	par.open_par = 0;
	par.closed_par = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '(')
			par.open_par++;
		else if (str[i] == ')')
			par.closed_par++;
		if (!qfil.in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			qfil.in_quotes = 1;
			qfil.quote_type = str[i];
		}
		else if (qfil.in_quotes && str[i] == qfil.quote_type)
			qfil.in_quotes = 0;
		if (return_error_cases(str, &i, &qfil, &par))
			return (1);
	}
	return (0);
}

void	syntax_error(t_list **head, int *flag)
{
	t_list	*tmp;
	char	*prev_token;
	char	*prev_data;
	int		i;

	if (!head || !*head)
		return ;
	tmp = *head;
	i = 0;
	prev_data = NULL;
	prev_token = NULL;
	if (!i && (!ft_strcmp((*head)->token, "PARENTHASIS") && !(
				*head)->next && no_words_beside((*head)->data)))
		(print_syntax_error(), *flag = 1, i = 1);
	if (!i && (ft_strcmp((*head)->token, "PIPE") == 0))
		(print_syntax_error(), *flag = 1, i = 1);
	if (!i && (ft_strcmp((*head)->token, "OPERATION_&&") == 0 || ft_strcmp(
				(*head)->token, "OPERATION_||") == 0))
		(print_syntax_error(), *flag = 1, i = 1);
	if (!i)
		while_loop_syntax_error(tmp, prev_token, prev_data, flag);
}

int	syntax_error_parentheses(t_list **head)
{
	t_list	*tmp;
	int		i;
	int		open_par;
	int		flag;

	tmp = *head;
	flag = 0;
	if (!head || !*head)
		return (0);
	while (tmp)
	{
		if (ft_strcmp(tmp->token, "PARENTHASIS") == 0)
		{
			i = 0;
			open_par = 0;
			syntax_error_parentheses_while_loop(tmp, &i, &flag, &open_par);
			if (flag == 1 || flag == 2 || flag == 3)
				return (print_syntax_error(), 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
