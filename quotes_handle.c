/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:52:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/24 12:34:22 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_lst(t_list_fd **node)
{
	char		*old_str;
	char		*new_str;
	int			final_len;

	old_str = (*node)->name;
	final_len = count_filtered_length(old_str, NULL, 0, 0);
	new_str = create_filtered_string(old_str, final_len, 0, 0);
	if (!new_str)
		return ;
	free((*node)->name);
	(*node)->name = new_str;
}

void	process_lnked_lst(t_tree **tree)
{
	t_list_fd	*tmp;
	int			i;

	i = 0;
	tmp = (*tree)->fd_list;
	while (tmp)
	{
		process_lst(&tmp);
		while (tmp->name[i])
		{
			if (tmp->name[i] == 10)
				tmp->name[i] = '"';
			else if (tmp->name[i] == 11)
				tmp->name[i] = '\'';
			i++;
		}
		tmp = tmp->next;
	}
}

void	process_lnked_lst_two(t_tree **tree)
{
	t_list_fd	*tmp;

	tmp = (*tree)->fd_list;
	while (tmp)
	{
		if (variable_search_instr(tmp->name))
			process_lst(&tmp);
		tmp = tmp->next;
	}
}

void	quote_remove_lst(t_tree **tree)
{
	if ((*tree)->left)
		quote_remove_lst(&(*tree)->left);
	if ((*tree)->right)
		quote_remove_lst(&(*tree)->right);
	if ((*tree)->fd_list)
		process_lnked_lst(tree);
}

void	quote_remove_lst_two(t_tree **tree)
{
	if ((*tree)->left)
		quote_remove_lst(&(*tree)->left);
	if ((*tree)->right)
		quote_remove_lst(&(*tree)->right);
	if ((*tree)->fd_list)
		process_lnked_lst_two(tree);
}
