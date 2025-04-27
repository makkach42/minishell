/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener_helper_two.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:44:24 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 11:45:08 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list_fd	*process_single_redirection(t_tree *tree,
	char *redirection, int *pos)
{
	t_list_fd	*new_node;
	int			i;
	int			flag;

	i = *pos;
	flag = determine_redirection_flag(redirection, i);
	skip_redirection_and_spaces(redirection, &i);
	new_node = malloc(sizeof(t_list_fd));
	if (!new_node)
		return (NULL);
	new_node->name = extract_filename(redirection, &i);
	new_node->redir = NULL;
	new_node->next = NULL;
	new_node->fd = -1;
	set_command_for_node(tree, new_node);
	set_redirection_type(new_node, flag);
	*pos = i;
	return (new_node);
}

int	skip_to_next_redirection(char *redirections, int i)
{
	while (redirections[i] && redirections[i
		] != '>' && redirections[i] != '<')
		i++;
	return (i);
}

int	initialize_redirection_processing(t_tree *tree,
				char **redirections_copy)
{
	if (!tree->redirections || check_empty(tree->redirections))
		return (0);
	*redirections_copy = ft_strdup(tree->redirections);
	if (!*redirections_copy)
		return (0);
	return (1);
}

int	process_first_redirection(t_tree *tree, t_list_fd **head,
				char *redirections_copy, int *i)
{
	*head = process_single_redirection(tree, redirections_copy, i);
	if (!*head)
	{
		free(redirections_copy);
		return (0);
	}
	return (1);
}

void	append_new_redirection(t_list_fd **tmp, t_tree *tree,
				char *redirections_copy, int *i)
{
	t_list_fd	*new_node;

	new_node = process_single_redirection(tree, redirections_copy, i);
	if (new_node)
	{
		(*tmp)->next = new_node;
		*tmp = new_node;
	}
}
