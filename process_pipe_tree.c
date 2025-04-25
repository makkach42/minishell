/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:13:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 14:27:34 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*handle_operation_node(t_list **head, t_list *pipe_pos, char *left_cmd)
{
	t_tree	*root;
	t_list	*left_head;
	t_list	*tmp_list;
	t_list	*current;
	t_list	*right_head;

	(void)left_cmd;
	root = create_tree_node(NULL, "OPERATION");
	left_head = *head;
	tmp_list = create_list_copy(left_head, pipe_pos);
	root->left = build_pipe_tree(&tmp_list);
	current = free_until_node(*head, pipe_pos);
	if (current)
	{
		right_head = current;
		root->right = build_pipe_tree(&right_head);
	}
	*head = NULL;
	return (root);
}

t_tree	*handle_pipe_node(t_list **head, t_list *pipe_pos, char *left_cmd)
{
	t_tree	*root;
	t_tree	*command_node;
	t_list	*current;
	t_list	*right_head;

	root = create_tree_node(NULL, "PIPE");
	command_node = create_tree_node(left_cmd, "COMMAND");
	root->left = command_node;
	current = free_until_node(*head, pipe_pos);
	if (current)
	{
		right_head = current;
		root->right = build_pipe_tree(&right_head);
	}
	*head = NULL;
	return (root);
}

t_tree	*build_pipe_tree(t_list **head)
{
	t_list	*pipe_pos;
	t_tree	*root;
	char	*left_cmd;

	pipe_pos = NULL;
	root = NULL;
	if (!head || !(*head))
		return (NULL);
	left_cmd = extract_command_with_redirects(head, &pipe_pos);
	if (!pipe_pos)
		return (handle_simple_command(head, left_cmd));
	if (ft_strcmp(pipe_pos->token, "PIPE") == 0)
		return (handle_pipe_node(head, pipe_pos, left_cmd));
	else if (ft_strcmp(pipe_pos->token, "OPERATION") == 0)
		return (handle_operation_node(head, pipe_pos, left_cmd));
	return (root);
}

void	if_cmd_tree(t_tree *tree, t_tree **cmd_tree)
{
	if ((*cmd_tree))
	{
		t_free(tree->command, __LINE__, "parsing.c");
		tree->command = NULL;
		tree->type = (*cmd_tree)->type;
		if (tree->left)
			free_tree(tree->left);
		if (tree->right)
			free_tree(tree->right);
		tree->left = (*cmd_tree)->left;
		tree->right = (*cmd_tree)->right;
		(*cmd_tree)->left = NULL;
		(*cmd_tree)->right = NULL;
		t_free((*cmd_tree), __LINE__, "parsing.c");
	}
}

void	process_pipe_trees(t_tree *tree)
{
	t_tree	*cmd_tree;

	if (!tree)
		return ;
	if (tree->command && tree->type && (
			ft_strcmp(tree->type, "PARENTHASIS") != 0) && (
			ft_strchr(tree->command, '|') || ft_strchr(tree->command, '&')))
	{
		cmd_tree = NULL;
		process_command_with_pipes(tree->command, &cmd_tree);
	}
	if (tree->left)
		process_pipe_trees(tree->left);
	if (tree->right)
		process_pipe_trees(tree->right);
}
