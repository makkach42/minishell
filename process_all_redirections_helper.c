/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:07:57 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:13:34 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*while_loop_args_aftredirs(char *redir_start,
	int in_quotes, char quote_type, int redir_active)
{
	int		i;
	char	*args;

	i = 0;
	while (redir_start[i])
	{
		update_quote_state(redir_start[i], &in_quotes, &quote_type);
		if (!in_quotes)
		{
			if (redir_start[i] == '>' || redir_start[i] == '<')
				redir_active = 1;
			else if ((redir_active == 1) && (redir_start[i] == ' '
				) && (i > 0) && (redir_start[i - 1] != '>'
				) && (redir_start[i - 1] != '<'))
			{
				i = handle_space_after_redir(redir_start, i, &args);
				if (args)
					return (args);
			}
			else
				i = update_redir_state(redir_start, i, &redir_active);
		}
		i++;
	}
	return (NULL);
}

char	*find_redirection_start(char *cmd_str)
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
		update_quote_state(cmd_str[i], &in_quotes, &quote_type);
		if (!in_quotes)
		{
			if (cmd_str[i] == '(')
				paren_count++;
			else if (cmd_str[i] == ')')
				paren_count--;
			if ((cmd_str[i] == '>' || cmd_str[i] == '<')
				&& paren_count == 0)
				return (&cmd_str[i]);
		}
		i++;
	}
	return (NULL);
}

char	*check_for_valid_args(char *redir_start, int j)
{
	while (redir_start[j] && redir_start[j] == ' ')
		j++;
	if (redir_start[j] && redir_start[j] != '>' && redir_start[j] != '<')
		return (&redir_start[j]);
	return (NULL);
}

int	handle_space_after_redir(char *redir_start, int i, char **args)
{
	int	j;

	j = i + 1;
	*args = check_for_valid_args(redir_start, j);
	if (*args)
		return (i);
	while (redir_start[j] && redir_start[j] == ' ')
		j++;
	if (redir_start[j] == '>' || redir_start[j] == '<')
		return (j - 1);
	return (i);
}

int	update_redir_state(char *redir_start, int i, int *redir_active)
{
	if (redir_start[i] == '>' || redir_start[i] == '<')
		*redir_active = 1;
	else if ((*redir_active == 1) && redir_start[i] != ' ')
		*redir_active = 2;
	return (i);
}
