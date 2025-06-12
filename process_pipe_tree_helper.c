/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe_tree_helper.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 10:11:11 by makkach           #+#    #+#             */
/*   Updated: 2025/06/12 10:14:55 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_tree_node(void *command, char *type)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->command = command;
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->redirections = NULL;
	node->fd_list = NULL;
	node->command_arr = NULL;
	node->ambiguous = 0;
	node->quotes = 0;
	return (node);
}

void	build_command_str(char **command_str, t_list *current)
{
	char	*temp_str;

	if (*command_str)
	{
		temp_str = *command_str;
		*command_str = ft_strjoin(*command_str, " ");
		free(temp_str);
		temp_str = *command_str;
		*command_str = ft_strjoin(*command_str, current->data);
		free(temp_str);
	}
	else if (current->data)
		*command_str = ft_strdup(current->data);
}

void	free_list_to_position(t_list **head, t_list *position)
{
	t_list	*current;
	t_list	*next;

	current = *head;
	next = NULL;
	while (current && current != position)
	{
		next = current->next;
		if (current->data)
			free(current->data);
		free(current);
		current = next;
	}
	if (position && current == position)
	{
		next = current->next;
		if (current->data)
			free(current->data);
		free(current);
		current = next;
	}
	if (!position)
		*head = NULL;
	else
		*head = current;
}

t_list	*copy_list_segment(t_list *head, t_list *end_pos)
{
	t_list	*tmp_list;
	t_list	*new_node;
	t_list	*last;

	tmp_list = NULL;
	while (head && head != end_pos)
	{
		new_node = malloc(sizeof(t_list));
		new_node->data = ft_strdup(head->data);
		new_node->token = head->token;
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
		head = head->next;
	}
	return (tmp_list);
}

t_tree	*handle_pipe_token(t_list **head, char *left_cmd,
									t_list *pipe_pos)
{
	t_tree	*root;
	t_tree	*command_node;
	t_list	*right_head;

	root = create_tree_node(NULL, "PIPE");
	command_node = create_tree_node(left_cmd, "COMMAND");
	root->left = command_node;
	free_list_to_position(head, pipe_pos);
	right_head = *head;
	if (right_head)
		root->right = build_pipe_tree(&right_head);
	*head = NULL;
	return (root);
}
