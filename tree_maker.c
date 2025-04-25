/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:09:28 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 15:54:10 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_node_type(t_tree **tree, t_list **head)
{
	if ((*head)->token && (*head)->token[0] != '\0')
		(*tree)->type = (*head)->token;
	else
		(*tree)->type = "WORD";
}

void	process_regular_node(t_list **head, t_tree **tree, int i)
{
	t_list	*current;
	t_list	*to_free;

	init_tree_node(tree, "");
	if (!(*tree))
		return ;
	set_node_type(tree, head);
	(*tree)->command = side_maker(head, i, 0);
	(*tree)->left = NULL;
	(*tree)->right = NULL;
	current = *head;
	while (current)
	{
		to_free = current;
		current = current->next;
		if (to_free->data)
			free(to_free->data);
		free(to_free);
	}
	*head = NULL;
}

void	find_and_process_operation(t_list **head, t_tree **tree,
								t_list *last, int total_nodes)
{
	t_list	*tmp;
	int		i;
	int		node_info[2];

	tmp = last;
	i = total_nodes;
	while (tmp)
	{
		if (ft_strcmp(tmp->token, "OPERATION") == 0)
		{
			node_info[0] = total_nodes;
			node_info[1] = i;
			process_operation_node(head, tree, tmp, node_info);
			return ;
		}
		tmp = tmp->prev;
		i--;
	}
}

void	find_and_process_pipe(t_list **head, t_tree **tree,
							t_list *last, int total_nodes)
{
	t_list	*tmp;
	int		i;
	int		node_info[2];

	tmp = last;
	i = total_nodes;
	while (tmp)
	{
		if (ft_strcmp(tmp->token, "PIPE") == 0)
		{
			node_info[0] = total_nodes;
			node_info[1] = i;
			process_pipe_node(head, tree, tmp, node_info);
			return ;
		}
		tmp = tmp->prev;
		i--;
	}
}

void	tree_maker(t_list **head, t_tree **tree)
{
	t_list	*last;
	int		total_nodes;
	int		node_index;

	if (!(*head))
	{
		*tree = NULL;
		return ;
	}
	total_nodes = count_total_nodes(*head, &last);
	find_and_process_operation(head, tree, last, total_nodes);
	if (*head == NULL)
		return ;
	find_and_process_pipe(head, tree, last, total_nodes);
	if (*head == NULL)
		return ;
	node_index = total_nodes;
	process_regular_node(head, tree, node_index);
}

//(ls>file | ls>file2) && (la>file3 | la>file4)
//"(ls>file | ls>file2) && (la>file3 | la>file4)"
//"sksksksk | kksksl"
//"sksksksk && kksksl"