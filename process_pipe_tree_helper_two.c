/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe_tree_helper_two.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 10:18:57 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 10:22:48 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*handle_operation_token(t_list **head, t_list *pipe_pos)
{
	t_tree	*root;
	t_list	*tmp_list;
	t_list	*right_head;

	root = create_tree_node(NULL, "OPERATION");
	tmp_list = copy_list_segment(*head, pipe_pos);
	root->left = build_pipe_tree(&tmp_list);
	free_list_to_position(head, pipe_pos);
	right_head = *head;
	if (right_head)
		root->right = build_pipe_tree(&right_head);
	*head = NULL;
	return (root);
}

void	free_cmd_list(t_list *cmd_list)
{
	t_list	*current;
	t_list	*next;

	if (!cmd_list)
		return ;
	current = cmd_list;
	while (current)
	{
		next = current->next;
		if (current->data)
			free(current->data);
		free(current);
		current = next;
	}
}

void	process_command_with_pipes_inits(t_list **cmd_list, char **cmd_copy)
{
	*cmd_list = NULL;
	*cmd_copy = NULL;
}

int	check_pipe_or_amp(const char *str, int i, char quote)
{
	if (!quote && (str[i] == '|' || str[i] == '&'))
	{
		if (str[i] == '&' && str[i + 1] != '&')
			return (0);
		return (1);
	}
	return (0);
}

void	update_tree_with_cmd(t_tree *tree, t_tree *cmd_tree)
{
	if (!cmd_tree)
		return ;
	free(tree->command);
	tree->command = NULL;
	tree->type = cmd_tree->type;
	if (tree->left)
		free_tree(tree->left);
	if (tree->right)
		free_tree(tree->right);
	tree->left = cmd_tree->left;
	tree->right = cmd_tree->right;
	cmd_tree->left = NULL;
	cmd_tree->right = NULL;
	free(cmd_tree);
}
