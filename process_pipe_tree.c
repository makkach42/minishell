/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:13:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/24 17:13:10 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_command_with_redirects_helper(
		char **command_str, t_list *current)
{
	char	*temp_str;

	if (command_str)
	{
		temp_str = command_str;
		command_str = ft_strjoin_leak(command_str, " ", __LINE__);
		t_free(temp_str, __LINE__, "parsing.c");
		temp_str = command_str;
		command_str = ft_strjoin_leak(command_str, current->data, __LINE__);
		t_free(temp_str, __LINE__, "parsing.c");
	}
	else if (current->data)
		command_str = ft_strdup(current->data);
}

char	*extract_command_with_redirects(t_list **head, t_list **pipe_pos)
{
	t_list	*current;
	char	*command_str;

	*pipe_pos = NULL;
	current = *head;
	command_str = NULL;
	while (current)
	{
		if ((current->token && ft_strcmp(current->token, "PIPE") == 0) || (
				current->token && ft_strcmp(current->token, "OPERATION") == 0))
		{
			*pipe_pos = current;
			break ;
		}
		extract_command_with_redirects_helper(&command_str, current);
		current = current->next;
	}
	if (!command_str)
		command_str = ft_strdup("");
	return (command_str);
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

t_tree	*build_pipe_tree(t_list **head)
{
	t_list	*pipe_pos;
	t_tree	*root;
	t_tree	*command_node;
	char	*left_cmd;
	t_list	*current;
	t_list	*next;
	t_list	*right_head;
	t_list	*left_head;
	t_list	*tmp_list;
	t_list	*new_node;
	t_list	*last;

	pipe_pos = NULL;
	root = NULL;
	command_node = NULL;
	left_cmd = NULL;
	if (!head || !(*head))
		return (NULL);
	left_cmd = extract_command_with_redirects(head, &pipe_pos);
	if (!pipe_pos)
	{
		root = create_tree_node(left_cmd, "COMMAND");
		current = *head;
		next = NULL;
		while (current)
		{
			next = current->next;
			if (current->data)
				t_free(current->data, __LINE__, "parsing.c");
			t_free(current, __LINE__, "parsing.c");
			current = next;
		}
		*head = NULL;
		return (root);
	}
	if (ft_strcmp(pipe_pos->token, "PIPE") == 0)
	{
		root = create_tree_node(NULL, "PIPE");
		command_node = create_tree_node(left_cmd, "COMMAND");
		root->left = command_node;
		current = *head;
		next = NULL;
		while (current && current != pipe_pos)
		{
			next = current->next;
			if (current->data)
				t_free(current->data, __LINE__, "parsing.c");
			t_free(current, __LINE__, "parsing.c");
			current = next;
		}
		if (current == pipe_pos)
		{
			next = current->next;
			if (current->data)
				t_free(current->data, __LINE__, "parsing.c");
			t_free(current, __LINE__, "parsing.c");
			current = next;
		}
		if (current)
		{
			right_head = current;
			root->right = build_pipe_tree(&right_head);
		}
		*head = NULL;
	}
	else if (ft_strcmp(pipe_pos->token, "OPERATION") == 0)
	{
		root = create_tree_node(NULL, "OPERATION");
		left_head = *head;
		tmp_list = NULL;
		while (left_head && left_head != pipe_pos)
		{
			new_node = t_malloc(sizeof(t_list), __LINE__, "parsing.c");
			new_node->data = ft_strdup(left_head->data);
			new_node->token = left_head->token;
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
			left_head = left_head->next;
		}
		root->left = build_pipe_tree(&tmp_list);
		current = *head;
		next = NULL;
		while (current && current != pipe_pos)
		{
			next = current->next;
			if (current->data)
				t_free(current->data, __LINE__, "parsing.c");
			t_free(current, __LINE__, "parsing.c");
			current = next;
		}
		if (current == pipe_pos)
		{
			next = current->next;
			if (current->data)
				t_free(current->data, __LINE__, "parsing.c");
			t_free(current, __LINE__, "parsing.c");
			current = next;
		}
		if (current)
		{
			right_head = current;
			root->right = build_pipe_tree(&right_head);
		}
		*head = NULL;
	}
	return (root);
}

void	process_command_with_pipes(char *command_str, t_tree **command_tree)
{
	t_list	*cmd_list;
	char	*cmd_copy;
	t_list	*current;
	t_list	*next;

	cmd_list = NULL;
	cmd_copy = NULL;
	if (!command_str || !(*command_str))
	{
		*command_tree = NULL;
		return ;
	}
	cmd_copy = ft_strdup(command_str);
	if (!cmd_copy)
	{
		*command_tree = NULL;
		return ;
	}
	cmd_list = list_init_leak(cmd_copy, __LINE__, "process_command_with_pipes");
	if (!cmd_list)
	{
		t_free(cmd_copy, __LINE__, "parsing.c");
		*command_tree = NULL;
		return ;
	}
	lexer(&cmd_list);
	*command_tree = build_pipe_tree(&cmd_list);
	if (cmd_list)
	{
		current = cmd_list;
		while (current)
		{
			next = current->next;
			if (current->data)
				t_free(current->data, __LINE__, "parsing.c");
			t_free(current, __LINE__, "parsing.c");
			current = next;
		}
		cmd_list = NULL;
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
		if (cmd_tree)
		{
			t_free(tree->command, __LINE__, "parsing.c");
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
			t_free(cmd_tree, __LINE__, "parsing.c");
		}
	}
	if (tree->left)
		process_pipe_trees(tree->left);
	if (tree->right)
		process_pipe_trees(tree->right);
}
