/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener_helper.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:38:31 by makkach           #+#    #+#             */
/*   Updated: 2025/07/04 11:11:33 by makkach          ###   ########.fr       */
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

int	if_question_mark_three(t_tmp_tree **tmp, int n, t_hdoc_data *h_data)
{
	char	*before;
	char	*after;
	char	*num;
	char	*new_str;

	before = ft_substr((*tmp)->tmp_fd->name, 0, n);
	if (!before)
		return (1);
	n++;
	after = ft_substr((*tmp)->tmp_fd->name, n + 1,
			ft_strlen((*tmp)->tmp_fd->name) - (n + 1));
	if (!after)
		return (free(before), 1);
	if (h_data->stat != -1)
		num = ft_itoa(h_data->stat);
	else
		num = ft_itoa((*tmp)->tree->status);
	if (!num)
		return (free(before), free(after), 1);
	new_str = ft_strjoin_three(before, num, after);
	if (!new_str)
		return (free(before), free(after), free(num), 1);
	free((*tmp)->tmp_fd->name);
	(*tmp)->tmp_fd->name = new_str;
	return (free(before), free(after), free(num), 0);
}
