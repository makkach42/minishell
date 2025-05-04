/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_nested_parentheses_helper.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:42:56 by makkach           #+#    #+#             */
/*   Updated: 2025/04/30 11:55:13 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sub_tree_attachment(t_tree **sub_tree, t_tree **tree,
		char **original_redirs)
{
	if (still_has_parentheses(sub_tree))
		process_nested_parentheses(sub_tree);
	if ((*tree)->command)
	{
		free((*tree)->command);
		(*tree)->command = NULL;
	}
	(*tree)->type = "PARENTHASIS";
	if ((*tree)->redirections)
		free((*tree)->redirections);
	(*tree)->redirections = *original_redirs;
	if ((*tree)->left)
		free_tree((*tree)->left);
	(*tree)->left = *sub_tree;
}

void	sub_tree_creation(t_tree **sub_tree, char **content, t_list **sub_list)
{
	*sub_list = list_init(*content);
	lexer(sub_list);
	tree_maker(sub_list, sub_tree);
}

void	free_all(char **content, char **original_redirs)
{
	free(*content);
	if (*original_redirs)
		free(*original_redirs);
}

void	redirections_in_par_handle(t_tree **tree, char **cmd_part,
		char **original_redirs, char **content)
{
	extract_redirections((*tree)->command, cmd_part, original_redirs);
	if ((*tree)->redirections)
		redirections_handle(original_redirs, tree);
	if (*cmd_part)
		*content = extract_content_from_parentheses(*cmd_part);
	else
		*content = extract_content_from_parentheses((*tree)->command);
	if (*cmd_part)
		free(*cmd_part);
}

int	dyn_buf_init(t_dynbuf *buf, size_t initial_capacity)
{
	buf->data = malloc(initial_capacity);
	if (!buf->data)
		return (0);
	buf->size = initial_capacity;
	buf->pos = 0;
	return (1);
}
