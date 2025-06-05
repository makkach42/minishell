/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:14:43 by makkach           #+#    #+#             */
/*   Updated: 2025/06/04 17:30:49 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *str2)
{
	write(2, "syntax error near unexpected token `", 37);
	write(2, str2, ft_strlen(str2));
	write(2, "\'\n", 2);
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

void	even_more_ifs(char *prev_token, char *prev_data, t_list *tmp, int *flag)
{
	int	i;

	i = 0;
	if (!i && (ft_strcmp("REDIRECTION", prev_token) == 0 && (
				ft_strcmp(tmp->token, "OPERATION_&&") == 0 || ft_strcmp(
					tmp->token, "OPERATION_||") == 0)))
		(print_syntax_error(prev_data), *flag = 1, i = 1);
	// if (!i && ((ft_strcmp("OPERATION_&&", prev_token) == 0 || ft_strcmp(
	// 				"OPERATION_||", prev_token) == 0) && ft_strcmp(
	// 			tmp->token, "REDIRECTION") == 0))
	// 	(print_syntax_error(prev_data), *flag = 1, i = 1);
	if (!i && (ft_strcmp("REDIRECTION", prev_token) == 0 && ft_strcmp(
				tmp->token, prev_token) == 0))
		(print_syntax_error(prev_data), *flag = 1, i = 1);
	if (!i && (ft_strcmp("WORD", prev_token) == 0 && ft_strcmp(
				tmp->token, "PARENTHASIS") == 0))
		(print_syntax_error(prev_data), *flag = 1, i = 1);
	if (!i && (ft_strcmp("PARENTHASIS", prev_token) == 0 && ft_strcmp(
				tmp->token, "WORD") == 0))
		(print_syntax_error(prev_data), *flag = 1);
}
