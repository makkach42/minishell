/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper_three.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:52:23 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 14:52:51 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_args_in_chunk(char *redir_start, int i, int redir_active)
{
	int	j;
	int	result;

	result = process_break_point(redir_start, i, &j, redir_active);
	if (result == 1)
		return (j);
	else if (result == 2 && (redir_start[j] == '>' || redir_start[j] == '<'))
		return (j - 1);
	else if (result == 2)
		return (-2);
	return (-1);
}
