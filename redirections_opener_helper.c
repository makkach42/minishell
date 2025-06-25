/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener_helper.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:38:31 by makkach           #+#    #+#             */
/*   Updated: 2025/06/24 18:22:08 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_check_filename(char **old_redirs, int *i)
{
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	while ((*old_redirs)[(*i)])
	{
		if (!in_quotes && ((*old_redirs)[(*i
				)] == '"' || (*old_redirs)[(*i)] == '\''))
		{
			in_quotes = 1;
			quote_type = (*old_redirs)[(*i)];
		}
		else if (in_quotes && (*old_redirs)[(*i)] == quote_type)
			in_quotes = 0;
		if ((!in_quotes) && (*old_redirs)[(*i)] == 32)
			break ;
		(*i)++;
	}
}

void	init_list_fd_node(t_list_fd *node)
{
	node->name = NULL;
	node->command = NULL;
	node->redir = NULL;
	node->next = NULL;
	node->in_quotes = 0;
	node->name_split = NULL;
	node->fd = -1;
	node->fd1 = -1;
}

void	syntax_error_two(t_tree **tree, int *flag)
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		syntax_error_two(&(*tree)->left, flag);
	if ((*tree)->right)
		syntax_error_two(&(*tree)->right, flag);
	if ((*tree)->command)
		if (check_quotes((*tree)->command, flag) == 1)
			(*tree)->status = 258;
}