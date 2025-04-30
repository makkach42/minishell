/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper_two.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:10:58 by makkach           #+#    #+#             */
/*   Updated: 2025/04/30 11:29:07 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_regular_char(char c, t_dynbuf *command_buf, int in_quotes_or_paren)
{
    if (c == ' ' || is_command_char(c, in_quotes_or_paren))
        return dyn_buf_add_char(command_buf, c);
    return (1);
}

void cleanup_and_assign(t_dynbuf *command_buf, t_dynbuf *redir_buf,
                char **cmd_part, char **redir_part)
{
    dyn_buf_finalize(command_buf);
    dyn_buf_finalize(redir_buf);
    *cmd_part = ft_strtrim(command_buf->data, " ");
    if (redir_buf->pos > 0)
        *redir_part = ft_strtrim(redir_buf->data, " ");
    else
        *redir_part = ft_strdup("");
    dyn_buf_free(command_buf);
    dyn_buf_free(redir_buf);
}

int	initialize_buffers(t_extract *v, char *cmd_str)
{
    size_t	initial_size;
	
	initial_size = ft_strlen(cmd_str) + 1;
    if (!dyn_buf_init(&v->command_buf, initial_size))
        return (0);
    if (!dyn_buf_init(&v->redir_buf, initial_size))
    {
        dyn_buf_free(&v->command_buf);
        return (0);
    }
    return (1);
}

int process_command_char(t_extract *v, char *cmd_str)
{
    if ((cmd_str[v->i] == '>' || cmd_str[v->i] == '<') && !v->in_quotes
        && v->paren_count == 0)
        return process_redirection(cmd_str, &v->i, &v->redir_buf);
    else
        return process_regular_char(cmd_str[v->i], &v->command_buf,
            v->in_quotes || v->paren_count > 0);
}

int	process_command_string_two(t_extract *v, char *cmd_str)
{
    while (cmd_str[v->i])
    {
        handle_quotes_and_parens(cmd_str[v->i], &v->in_quotes, &v->quote_type,
            &v->paren_count);
        if (!process_command_char(v, cmd_str))
            return (0);
        v->i++;
    }
    dyn_buf_finalize(&v->command_buf);
    dyn_buf_finalize(&v->redir_buf);
    
    return (1);
}
