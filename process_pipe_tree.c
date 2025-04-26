/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:13:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 10:21:17 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		build_command_str(&command_str, current);
		current = current->next;
	}
	if (!command_str)
		command_str = ft_strdup("");
	return (command_str);
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
	{
		root = create_tree_node(left_cmd, "COMMAND");
		free_list_to_position(head, NULL);
		return (root);
	}
	if (ft_strcmp(pipe_pos->token, "PIPE") == 0)
		root = handle_pipe_token(head, left_cmd, pipe_pos);
	else if (ft_strcmp(pipe_pos->token, "OPERATION") == 0)
		root = handle_operation_token(head, pipe_pos);
	return (root);
}

void	process_command_with_pipes(char *command_str, t_tree **command_tree)
{
	t_list	*cmd_list;
	char	*cmd_copy;

	process_command_with_pipes_inits(&cmd_list, &cmd_copy);
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
	cmd_list = list_init(cmd_copy);
	if (!cmd_list)
	{
		free(cmd_copy);
		*command_tree = NULL;
		return ;
	}
	lexer(&cmd_list);
	*command_tree = build_pipe_tree(&cmd_list);
	free_cmd_list(cmd_list);
}

int	has_unquoted_pipe_or_amp(const char *str)
{
	int		i;
	char	quote;

	if (!str)
		return (0);
	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && (
				quote == 0 || quote == str[i]))
		{
			if (quote == 0)
				quote = str[i];
			else
				quote = 0;
		}
		else if (check_pipe_or_amp(str, i, quote))
			return (1);
		i++;
	}
	return (0);
}

void	process_pipe_trees(t_tree *tree)
{
	t_tree	*cmd_tree;

	if (!tree)
		return ;
	if (tree->command && tree->type && (
			ft_strcmp(tree->type, "PARENTHASIS") != 0
		) && has_unquoted_pipe_or_amp(tree->command))
	{
		cmd_tree = NULL;
		process_command_with_pipes(tree->command, &cmd_tree);
		update_tree_with_cmd(tree, cmd_tree);
	}
	if (tree->left)
		process_pipe_trees(tree->left);
	if (tree->right)
		process_pipe_trees(tree->right);
}
