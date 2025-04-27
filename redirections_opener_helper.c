/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener_helper.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:38:31 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 11:43:02 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	determine_redirection_flag(char *redirection, int i)
{
	if (redirection[i] == '>' && (redirection[i + 1
			] == ' ' || redirection[i + 1] == '\0' || redirection[
				i + 1] == '\t'))
		return (1);
	else if (redirection[i] == '>' && redirection[i + 1] == '>')
		return (2);
	else if (redirection[i] == '<' && (redirection[i + 1
			] == ' ' || redirection[i + 1] == '\0' || redirection[
				i + 1] == '\t'))
		return (3);
	else if (redirection[i] == '<' && redirection[i + 1] == '<')
		return (4);
	return (0);
}

void	skip_redirection_and_spaces(char *redirection, int *i)
{
	while ((redirection[*i] == '>' || redirection[*i] == '<')
		&& redirection[*i] != '\0')
		(*i)++;
	while (redirection[*i] == ' ' && redirection[*i] != '\0')
		(*i)++;
}

char	*extract_filename(char *redirection, int *i)
{
	int	start;

	start = *i;
	while (redirection[*i] != ' ' && redirection[*i] != '\0')
		(*i)++;
	return (ft_substr(redirection, start, *i - start));
}

void	set_command_for_node(t_tree *tree, t_list_fd *new_node)
{
	if (ft_strcmp(tree->type, "OPERATION") == 0)
	{
		if (tree->right && tree->right->command)
			new_node->command = ft_strdup(tree->right->command);
		else if (tree->left && tree->left->command)
			new_node->command = ft_strdup(tree->left->command);
		else
			new_node->command = ft_strdup("");
	}
	else if (ft_strcmp(tree->type, "PARENTHASIS") == 0)
	{
		if (tree->command)
			new_node->command = ft_strdup(tree->command);
		else
			new_node->command = ft_strdup("");
	}
	else
	{
		new_node->command = ft_strdup(tree->command);
	}
}

void	set_redirection_type(t_list_fd *new_node, int flag)
{
	if (flag == 1 && !check_empty(new_node->name))
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup(">");
	}
	else if (flag == 2 && !check_empty(new_node->name))
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup(">>");
	}
	else if (flag == 3)
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup("<");
	}
	else if (flag == 4)
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup("<<");
	}
}
