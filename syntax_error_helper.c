/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:14:43 by makkach           #+#    #+#             */
/*   Updated: 2025/04/24 16:16:12 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *str2)
{
	write(2, "syntax error near unexpected token `", 37);
	write(2, str2, ft_strlen(str2));
	write(2, "\'\n", 2);
}

void	syntax_error_parentheses_while_loop_parentheses(t_list *tmp,
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
		syntax_error_parentheses_while_loop_parentheses(tmp,
			i, open_par, &closed_par);
		if (((*open_par) == closed_par && (*open_par) != 0
			) && tmp->data[*i + 1])
		{
			*flag = 2;
			break ;
		}
		if ((tmp->data[*i] == 32 || is_operator(tmp->data[*i])))
			break ;
		(*i)++;
	}
	if (*open_par != closed_par)
		*flag = 3;
}
