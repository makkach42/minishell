/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_other_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:30:34 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 11:48:58 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_redirection_helper(char *cmd_str, int *i, char *redir_buf,
				int *redir_pos)
{
	char	file_quote;

	while (cmd_str[*i] && cmd_str[*i] != ' ' && cmd_str[*i] != '>'
		&& cmd_str[*i] != '<')
	{
		if ((cmd_str[*i] == '\'' || cmd_str[*i] == '\"'))
		{
			file_quote = cmd_str[(*i)++];
			redir_buf[(*redir_pos)++] = file_quote;
			while (cmd_str[*i] && cmd_str[*i] != file_quote)
				redir_buf[(*redir_pos)++] = cmd_str[(*i)++];
			if (cmd_str[*i])
				redir_buf[(*redir_pos)++] = cmd_str[*i];
		}
		else
			redir_buf[(*redir_pos)++] = cmd_str[*i];
		(*i)++;
	}
}

int	check_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32)
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}
