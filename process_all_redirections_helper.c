/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:10:32 by makkach           #+#    #+#             */
/*   Updated: 2025/04/30 11:52:45 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_command_char(char c, int in_quotes)
{
	return ((c != '>' && c != '<') || in_quotes);
}

int	skip_spaces_in_str(char *str, int start)
{
	while (str[start] && str[start] == ' ')
		start++;
	return (start);
}

void	init_extraction_vars(int *i, int *in_quotes, char *quote_type,
				int *paren_count)
{
	*i = 0;
	*in_quotes = 0;
	*quote_type = 0;
	*paren_count = 0;
}

void	handle_quotes_and_parens(char c, int *in_quotes, char *quote_type,
					int *paren_count)
{
	if ((c == '\'' || c == '\"') && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = c;
	}
	else if (*in_quotes && c == *quote_type)
		*in_quotes = 0;
	if (!*in_quotes)
	{
		if (c == '(')
			(*paren_count)++;
		else if (c == ')')
			(*paren_count)--;
	}
}

int	process_redirection(char *cmd_str, int *i, t_dynbuf *redir_buf)
{
	if (!dyn_buf_add_char(redir_buf, cmd_str[*i]))
		return (0);
	if (cmd_str[*i + 1] && cmd_str[*i] == cmd_str[*i + 1])
	{
		if (!dyn_buf_add_char(redir_buf, cmd_str[++(*i)]))
			return (0);
	}
	if (!dyn_buf_add_char(redir_buf, ' '))
		return (0);
	(*i)++;
	while (cmd_str[*i] && cmd_str[*i] == ' ')
		(*i)++;
	if (!process_redirection_helper(cmd_str, i, redir_buf))
		return (0);
	if (!dyn_buf_add_char(redir_buf, ' '))
		return (0);
	(*i)--;
	return (1);
}
