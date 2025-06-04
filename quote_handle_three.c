/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handle_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:29:57 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 08:21:07 by makkach          ###   ########.fr       */
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
