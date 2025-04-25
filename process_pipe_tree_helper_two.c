/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe_tree_helper_two.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:17:26 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 14:27:42 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list_nodes(t_list *current)
{
	t_list	*next;

	next = NULL;
	while (current)
	{
		next = current->next;
		if (current->data)
			t_free(current->data, __LINE__, "parsing.c");
		t_free(current, __LINE__, "parsing.c");
		current = next;
	}
}

t_tree	*handle_simple_command(t_list **head, char *left_cmd)
{
	t_tree	*root;

	root = create_tree_node(left_cmd, "COMMAND");
	free_list_nodes(*head);
	*head = NULL;
	return (root);
}

t_tree	*create_tree_node(void *command, char *type)
{
	t_tree	*node;

	node = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
	if (!node)
		return (NULL);
	node->command = command;
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->redirections = NULL;
	node->command_arr = NULL;
	return (node);
}

t_list	*create_list_copy(t_list *source, t_list *stop)
{
	t_list	*tmp_list;
	t_list	*new_node;
	t_list	*last;

	tmp_list = NULL;
	while (source && source != stop)
	{
		new_node = t_malloc(sizeof(t_list), __LINE__, "parsing.c");
		new_node->data = ft_strdup(source->data);
		new_node->token = source->token;
		new_node->next = NULL;
		new_node->prev = NULL;
		if (!tmp_list)
			tmp_list = new_node;
		else
		{
			last = tmp_list;
			while (last->next)
				last = last->next;
			last->next = new_node;
			new_node->prev = last;
		}
		source = source->next;
	}
	return (tmp_list);
}

t_list	*free_until_node(t_list *start, t_list *stop)
{
	t_list	*current;
	t_list	*next;

	current = start;
	next = NULL;
	while (current && current != stop)
	{
		next = current->next;
		if (current->data)
			t_free(current->data, __LINE__, "parsing.c");
		t_free(current, __LINE__, "parsing.c");
		current = next;
	}
	if (current == stop)
	{
		next = current->next;
		if (current->data)
			t_free(current->data, __LINE__, "parsing.c");
		t_free(current, __LINE__, "parsing.c");
		current = next;
	}
	return (current);
}
