/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker_helper_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:03:26 by makkach           #+#    #+#             */
/*   Updated: 2025/06/24 16:04:33 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*create_command_node(t_list **node_list, int offset)
{
	t_tree	*command;
	t_list	*node;

	node = *node_list;
	command = malloc(sizeof(t_tree));
	if (!command)
		return (NULL);
	if (node->token && node->token[0] != '\0')
		command->type = node->token;
	else
		command->type = "WORD";
	command->command = side_maker(node_list, offset, 0);
	command->left = NULL;
	command->right = NULL;
	command->command_arr = NULL;
	command->redirections = NULL;
	command->fd_list = NULL;
	command->ambiguous = 0;
	command->var = 0;
	command->status = 0;
	return (command);
}

int	count_total_nodes(t_list *head, t_list **last)
{
	t_list	*tmp;
	int		total_nodes;

	tmp = head;
	total_nodes = 0;
	while (tmp)
	{
		*last = tmp;
		tmp = tmp->next;
		total_nodes++;
	}
	return (total_nodes);
}

void	if_prev(t_list *tmp, t_list **prev_part, t_tree **tree, t_list **head)
{
	if (tmp->prev)
	{
		*prev_part = *head;
		tmp->prev->next = NULL;
		free_node_data(tmp);
		if (*prev_part)
		{
			tree_maker(prev_part, &(*tree)->left);
			if (*prev_part)
				cleanup_left_nodes(*prev_part);
		}
	}
	else
	{
		free_node_data(tmp);
		(*tree)->left = NULL;
	}
}

static t_tree	*setup_operation_command(t_list **next_list, int offset)
{
	t_tree	*command;

	command = create_command_node(next_list, offset);
	if (!command)
		return (NULL);
	cleanup_right_nodes(*next_list);
	return (command);
}

void	handle_operation_command(t_tree **tree, t_list *tmp,
		int total_nodes, int i)
{
	t_tree	*command;

	command = NULL;
	if (tmp->next)
	{
		command = setup_operation_command(&(tmp->next), total_nodes - i);
		if (!command)
		{
			free(*tree);
			*tree = NULL;
			return ;
		}
	}
	(*tree)->right = command;
}
