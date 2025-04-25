/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:36:23 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 14:57:28 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_args_break_point(char *str, int i, int redir_active)
{
	return (redir_active && str[i] == ' ' && i > 0 && str[
			i - 1] != '>' && str[i - 1] != '<');
}

int	skip_spaces(char *str, int start)
{
	int	j;

	j = start + 1;
	while (str[j] && str[j] == ' ')
		j++;
	return (j);
}

int	check_quote(char c, int *in_quotes, char *quote_type)
{
	if ((c == '\'' || c == '\"') && !(*in_quotes))
	{
		*in_quotes = 1;
		*quote_type = c;
		return (1);
	}
	else if (*in_quotes && c == *quote_type)
	{
		*in_quotes = 0;
		return (1);
	}
	return (0);
}

char	*find_first_redirection(char *cmd_str)
{
	int		i;
	int		in_quotes;
	int		paren_count;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	paren_count = 0;
	quote_type = 0;
	while (cmd_str[i])
	{
		if (!check_quote(cmd_str[i], &in_quotes, &quote_type) && !in_quotes)
		{
			if (cmd_str[i] == '(')
				paren_count++;
			else if (cmd_str[i] == ')')
				paren_count--;
			if ((cmd_str[i] == '>' || cmd_str[i] == '<') && paren_count == 0)
				return (&cmd_str[i]);
		}
		i++;
	}
	return (NULL);
}

int	find_args_start_pos(char *redir_start)
{
	int		i;
	int		in_quotes;
	char	quote_type;
	int		redir_active;
	int		pos;

	init_args_search(&i, &in_quotes, &quote_type, &redir_active);
	while (redir_start[++i])
	{
		if (!check_quote(redir_start[i], &in_quotes, &quote_type) && !in_quotes)
		{
			if (handle_redirection_char(redir_start[i], &redir_active))
				;
			else
			{
				pos = find_args_in_chunk(redir_start, i, redir_active);
				if (pos >= 0)
					return (pos);
				else if (pos == -2)
					break ;
				update_redir_active(redir_start[i], &redir_active);
			}
		}
	}
	return (-1);
}
