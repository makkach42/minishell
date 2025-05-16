/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handle_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:29:57 by makkach           #+#    #+#             */
/*   Updated: 2025/05/16 09:30:19 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_command_array_two(t_tree **tree)
{
	int	k;

	k = 0;
	while ((*tree)->command_arr[k])
	{
		if (variable_search_instr((*tree)->command_arr[k]))
			process_command_string(tree, k);
		k++;
	}
}

void	quote_remove(t_tree **tree)
{
	if ((*tree) && (*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
		process_command_array(tree);
}

void	quote_remove_two(t_tree **tree)
{
	if ((*tree) && (*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
		process_command_array_two(tree);
}
