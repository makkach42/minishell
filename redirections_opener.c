/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:47:08 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 11:48:48 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_remaining_redirections(t_tree *tree, t_list_fd **tmp,
				char *redirections_copy, int *i)
{
	t_list_fd	*new_node;

	while (!check_empty(redirections_copy + *i))
	{
		while (redirections_copy[*i] == ' ')
			(*i)++;
		*i = skip_to_next_redirection(redirections_copy, *i);
		if (!redirections_copy[*i])
			break ;
		new_node = process_single_redirection(tree, redirections_copy, i);
		if (new_node)
		{
			(*tmp)->next = new_node;
			*tmp = new_node;
		}
		else
			break ;
	}
}

static void	process_first_redirection_node(t_tree *tree, t_list_fd **head)
{
	char		*redirections_copy;
	int			i;
	t_list_fd	*tmp;

	if (!initialize_redirection_processing(tree, &redirections_copy))
		return ;
	i = skip_to_next_redirection(redirections_copy, 0);
	if (!redirections_copy[i])
	{
		free(redirections_copy);
		return ;
	}
	if (!process_first_redirection(tree, head, redirections_copy, &i))
		return ;
	tmp = *head;
	process_remaining_redirections(tree, &tmp, redirections_copy, &i);
	free(redirections_copy);
}

static void	process_additional_redirection_node(t_tree *tree, t_list_fd **head)
{
	char		*redirections_copy;
	int			i;
	t_list_fd	*tmp;

	if (!initialize_redirection_processing(tree, &redirections_copy))
		return ;
	i = skip_to_next_redirection(redirections_copy, 0);
	if (!redirections_copy[i])
	{
		free(redirections_copy);
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	append_new_redirection(&tmp, tree, redirections_copy, &i);
	process_remaining_redirections(tree, &tmp, redirections_copy, &i);
	free(redirections_copy);
}

static void	process_node_redirections(t_tree *tree, t_list_fd **head)
{
	if (!*head)
		process_first_redirection_node(tree, head);
	else
		process_additional_redirection_node(tree, head);
}

void	redirections_opener(t_tree **tree, t_list_fd **head)
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		redirections_opener(&(*tree)->left, head);
	if ((*tree)->right)
		redirections_opener(&(*tree)->right, head);
	if ((*tree)->redirections)
		process_node_redirections(*tree, head);
}
