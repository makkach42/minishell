/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:07:40 by makkach           #+#    #+#             */
/*   Updated: 2025/04/24 11:07:52 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_quotes(int	*in_quotes, char *quote_type, char **str, int *i)
{
	*in_quotes = 1;
	*quote_type = (*str)[*i];
}

void	parenthesis_recognizer_helper(int *open_par,
	int *closed_par, int *i, char *str)
{
	if (str[*i] == '(')
		(*open_par)++;
	if (str[*i] == ')')
		(*closed_par)++;
}
