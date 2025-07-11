/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:14:43 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 11:21:48 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(void)
{
	ft_putstr_fd(2, "syntax error near unexpected token\n");
}

void	while_loop_parentheses(t_list *tmp,
		int *i, int *open_par, int *closed_par)
{
	if (tmp->data[*i] == '(')
		(*open_par)++;
	if (tmp->data[*i] == ')')
		(*closed_par)++;
}

void	syntax_error_parentheses_while_loop(t_list *tmp,
		int *i, int *flag, int *open_par)
{
	int		closed_par;

	(*open_par) = 0;
	closed_par = 0;
	while (tmp->data[*i])
	{
		if (tmp->data[*i] != '(' && !(*open_par))
		{
			*flag = 1;
			break ;
		}
		while_loop_parentheses(tmp, i, open_par, &closed_par);
		if (((*open_par) == closed_par && (*open_par) != 0
			) && tmp->data[*i + 1])
		{
			*flag = 2;
			break ;
		}
		if (*open_par == closed_par && (
				tmp->data[*i] == 32 || is_operator(tmp->data[*i])))
			break ;
		(*i)++;
	}
	if (*open_par != closed_par)
		*flag = 3;
}

void	last_pack_of_ifs(char *prev_token,
		char *prev_data, t_list *tmp, int *flag)
{
	if (!*flag && tmp && (!ft_strcmp("PIPE", tmp->token) && !tmp->next))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (!ft_strcmp("PARENTHASIS", prev_token)) && (
			!ft_strcmp("REDIRECTION", tmp->token) && tmp->next && (
				!ft_strcmp("WORD", tmp->next->token) || !ft_strcmp(
					"COMMAND", tmp->next->token)) && tmp->next->next && (
				!ft_strcmp("WORD", tmp->next->next->token))))
		(print_syntax_error(), *flag = 1);
	if (!*flag && (!ft_strcmp(prev_token,
				"PARENTHASIS") && in_para_check(prev_data)))
		(print_syntax_error(), *flag = 1);
}

void	even_more_ifs(char *prev_token, char *prev_data, t_list *tmp, int *flag)
{
	if (!*flag && tmp && (!ft_strcmp("REDIRECTION", prev_token) && (
				!ft_strcmp(tmp->token, "OPERATION_&&") || !ft_strcmp(
					tmp->token, "OPERATION_||"))))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (!ft_strcmp("REDIRECTION", prev_token) && !ft_strcmp(
				tmp->token, "PARENTHASIS")))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (!ft_strcmp("REDIRECTION", prev_token) && !ft_strcmp(
				"<<", prev_data) && !ft_strcmp(tmp->token, "PARENTHASIS")))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (!ft_strcmp("REDIRECTION", prev_token) && !ft_strcmp(
				tmp->token, prev_token)))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && ((!ft_strcmp("WORD", prev_token) || !ft_strcmp(
					"COMMAND", prev_token)) && (!ft_strcmp(
					tmp->token, "PARENTHASIS"))))
		(print_syntax_error(), *flag = 1);
	if (!*flag && tmp && (!ft_strcmp("PARENTHASIS", prev_token) && (!ft_strcmp(
					tmp->token, "WORD") || !ft_strcmp(tmp->token, "VARIABLE"))))
		(print_syntax_error(), *flag = 1);
	if (!*flag)
		(last_pack_of_ifs(prev_token, prev_data, tmp, flag));
}
