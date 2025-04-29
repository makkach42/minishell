/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_new_other_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:09:50 by makkach           #+#    #+#             */
/*   Updated: 2025/04/29 09:53:42 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ambiguous_syntax_error(t_tree **tree)
{
	int i;

	i = 0;
	if ((*tree)->ambiguous == 1 && (*tree)->quotes == 0)
		i = 1;
	if ((*tree)->left)
		ambiguous_syntax_error(&(*tree)->left);
	if ((*tree)->right)
		ambiguous_syntax_error(&(*tree)->right);
	return (i);
}

// int	quote_set()
// {

// }