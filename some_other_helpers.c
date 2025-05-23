/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_other_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:30:34 by makkach           #+#    #+#             */
/*   Updated: 2025/05/23 10:05:02 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redirection_helper(char *cmd_str, int *i, t_dynbuf *redir_buf)
{
	char	file_quote;

	while (cmd_str[*i] && cmd_str[*i] != ' ' && cmd_str[*i] != '>'
		&& cmd_str[*i] != '<')
	{
		if ((cmd_str[*i] == '\'' || cmd_str[*i] == '\"'))
		{
			file_quote = cmd_str[(*i)++];
			if (!dyn_buf_add_char(redir_buf, file_quote))
				return (0);
			while (cmd_str[*i] && cmd_str[*i] != file_quote)
				if (!dyn_buf_add_char(redir_buf, cmd_str[(*i)++]))
					return (0);
			if (cmd_str[*i])
				if (!dyn_buf_add_char(redir_buf, cmd_str[*i]))
					return (0);
		}
		else
			if (!dyn_buf_add_char(redir_buf, cmd_str[*i]))
				return (0);
		(*i)++;
	}
	return (1);
}

int	check_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || str[i] == 9)
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	countwords(char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (s && *s)
	{
		if (*s == c)
			in_word = 0;
		else if (!in_word)
		{
			count++;
			in_word = 1;
		}
		s++;
	}
	return (count);
}
void	ambiguous_set(t_tree **tree)
{
	t_list_fd	*tmp;
	int			i;
	int			in_word;
	int			in_quotes;
	int			count;
	char		quote_type;

	if ((*tree) && (*tree)->left)
		ambiguous_set(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		ambiguous_set(&(*tree)->right);
	if ((*tree) && (*tree)->fd_list)
	{
		tmp = (*tree)->fd_list;
		while (tmp)
		{
			i = 0;
			in_quotes = 0;
			quote_type = 0;
			in_word = 0;
			count = 0;
			while (tmp->name[i])
			{
				if (!in_quotes && (tmp->name[i] == '"' || tmp->name[i] == '\''))
				{
					in_quotes = 1;
					quote_type = tmp->name[i];
				}
				else if (in_quotes && tmp->name[i] == quote_type)
					in_quotes = 0;
				if (!in_quotes && tmp->name[i] == 32)
					in_word = 0;
				else if (!in_word)
				{
					count++;
					in_word = 1;
				}
				i++;
			}
			if (count != 1)
			{
				(*tree)->ambiguous = 1;
			}
			else if (count == 1)
			{
				i = 0;
				while (tmp->name[i] == '"' || tmp->name[i] == '\'')
					i++;
				if (!tmp->name[i] && i != 0)
					(*tree)->quotes = 1;
			}
			tmp = tmp->next;
		}
	}
}

int	new_syntax_error(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (tmp && tmp->next && tmp->next->next)
	{
		if (!ft_strcmp(tmp->token, "REDIRECTION") && !ft_strcmp(
				tmp->next->token, "WORD") && !ft_strcmp(
				tmp->next->next->token, "PARENTHASIS"))
			break ;
		tmp = tmp->next;
	}
	if (tmp && tmp->next && tmp->next->next)
		return (1);
	return (0);
}
