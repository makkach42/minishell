/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker_helper_three.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:12:02 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 10:42:21 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_operation_node(t_list **head, t_tree **tree, t_list *tmp,
							int *node_info)
{
	t_list	*prev_part;

	init_tree_node(tree, "OPERATION");
	if (!(*tree))
		return ;
	handle_operation_command(tree, tmp, node_info[0], node_info[1]);
	if (!(*tree))
		return ;
	if_prev(tmp, &prev_part, tree, head);
	*head = NULL;
}

void	process_pipe_node(t_list **head, t_tree **tree, t_list *tmp,
						int node_info[2])
{
	t_list	*prev_part;

	init_tree_node(tree, "PIPE");
	if (!(*tree))
		return ;
	handle_operation_command(tree, tmp, node_info[0], node_info[1]);
	if (!(*tree))
		return ;
	if_prev(tmp, &prev_part, tree, head);
	*head = NULL;
}
