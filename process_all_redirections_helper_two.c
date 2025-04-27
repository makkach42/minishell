/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper_two.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:10:58 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 10:26:07 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_regular_char(char c, char *command_buf, int *cmd_pos,
				int in_quotes_or_paren)
{
	if (c == ' ')
		command_buf[(*cmd_pos)++] = c;
	else if (is_command_char(c, in_quotes_or_paren))
		command_buf[(*cmd_pos)++] = c;
}

void	cleanup_and_assign(char *command_buf, char *redir_buf,
				char **cmd_part, char **redir_part)
{
	*cmd_part = ft_strtrim(command_buf, " ");
	t_free(command_buf, __LINE__, "parsing.c");
	if (ft_strlen(redir_buf) > 0)
		*redir_part = ft_strtrim(redir_buf, " ");
	else
		*redir_part = ft_strdup("");
	t_free(redir_buf, __LINE__, "parsing.c");
}

int	initialize_buffers(t_extract *v, char *cmd_str)
{
	v->command_buf = malloc(ft_strlen(cmd_str) + 1);
	v->redir_buf = malloc(ft_strlen(cmd_str) + 1);
	if (!v->command_buf || !v->redir_buf)
	{
		if (v->command_buf)
			t_free(v->command_buf, __LINE__, "parsing.c");
		if (v->redir_buf)
			t_free(v->redir_buf, __LINE__, "parsing.c");
		return (0);
	}
	return (1);
}

void	process_command_char(t_extract *v, char *cmd_str)
{
	if ((cmd_str[v->i] == '>' || cmd_str[v->i] == '<') && !v->in_quotes
		&& v->paren_count == 0)
		process_redirection(cmd_str, &v->i, v->redir_buf, &v->redir_pos);
	else
		process_regular_char(cmd_str[v->i], v->command_buf, &v->cmd_pos,
			v->in_quotes || v->paren_count > 0);
}

void	process_command_string_two(t_extract *v, char *cmd_str)
{
	while (cmd_str[v->i])
	{
		handle_quotes_and_parens(cmd_str[v->i], &v->in_quotes, &v->quote_type,
			&v->paren_count);
		process_command_char(v, cmd_str);
		v->i++;
	}
	v->command_buf[v->cmd_pos] = '\0';
	v->redir_buf[v->redir_pos] = '\0';
}
