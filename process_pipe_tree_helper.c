/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe_tree_helper.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:00:35 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 16:23:36 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_cmd_list(t_list **cmd_list)
{
	t_list	*next;
	t_list	*current;

	if (*cmd_list)
	{
		current = *cmd_list;
		while (current)
		{
			next = current->next;
			if (current->data)
				free(current->data);
			free(current);
			current = next;
		}
		*cmd_list = NULL;
	}
}

void	lexing_list_and_setting_tree(t_list **cmd_list, t_tree **command_tree)
{
	lexer(cmd_list);
	*command_tree = build_pipe_tree(cmd_list);
}

void	process_command_with_pipes(char *command_str, t_tree **command_tree)
{
	t_list	*cmd_list;
	char	*cmd_copy;

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
		free(cmd_copy);
		*command_tree = NULL;
		return ;
	}
	lexing_list_and_setting_tree(&cmd_list, command_tree);
	if_cmd_list(&cmd_list);
}

void	extract_command_with_redirects_helper(
		char **command_str, t_list *current)
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
