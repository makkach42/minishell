/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 18:49:56 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_filtered_length(char *old_str)
{
	int	i;
	int	len;
	int	final_len;
	int	in_single_quotes;
	int	in_double_quotes;

	len = ft_strlen(old_str);
	final_len = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	i = 0;
	while (i < len)
	{
		if (old_str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (old_str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
			final_len++;
		i++;
	}
	return (final_len);
}

int	check_if_in_string(char *cmd)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	while (cmd[i])
	{
		if (!in_quotes && (cmd[i] == '\'' || cmd[i] == '"'))
		{
			in_quotes = 1;
			quote_type = cmd[i];
		}
		else if (in_quotes && cmd[i] == quote_type)
		{
			in_quotes = 0;
			quote_type = '\0';
		}
		if ((cmd[i] == '>' || cmd[i] == '<') && !in_quotes)
			break ;
		i++;
	}
	if (cmd[i] == '\0')
		return (0);
	return (1);
}

int is_command_char(char c, int in_quotes)
{
    return (c != '>' && c != '<') || in_quotes;
}

int skip_spaces_in_str(char *str, int start)
{
    while (str[start] && str[start] == ' ')
        start++;
    return start;
}

void extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
{
    int     i;
    int     in_quotes;
    char    quote_type;
    char    *command_buf;
    char    *redir_buf;
    int     cmd_pos;
    int     redir_pos;
    int     paren_count;

    *cmd_part = NULL;
    *redir_part = NULL;
    if (!cmd_str)
        return;

    command_buf = malloc(ft_strlen(cmd_str) + 1);
    redir_buf = malloc(ft_strlen(cmd_str) + 1);
    if (!command_buf || !redir_buf)
    {
        if (command_buf)
            t_free(command_buf, __LINE__, "parsing.c");
        if (redir_buf)
            t_free(redir_buf, __LINE__, "parsing.c");
        return;
    }
    i = 0;
    cmd_pos = 0;
    redir_pos = 0;
    in_quotes = 0;
    paren_count = 0;
    quote_type = 0;
    while (cmd_str[i])
    {
        if ((cmd_str[i] == '\'' || cmd_str[i] == '\"') && !in_quotes)
        {
            in_quotes = 1;
            quote_type = cmd_str[i];
        }
        else if (in_quotes && cmd_str[i] == quote_type)
            in_quotes = 0;
        if (!in_quotes)
        {
            if (cmd_str[i] == '(')
                paren_count++;
            else if (cmd_str[i] == ')')
                paren_count--;
        }
        if ((cmd_str[i] == '>' || cmd_str[i] == '<') && !in_quotes && paren_count == 0)
        {
            redir_buf[redir_pos++] = cmd_str[i];
            if (cmd_str[i+1] && cmd_str[i] == cmd_str[i+1])
            {
                redir_buf[redir_pos++] = cmd_str[++i];
            }
            redir_buf[redir_pos++] = ' ';
            i++;
            while (cmd_str[i] && cmd_str[i] == ' ')
                i++;
            while (cmd_str[i] && cmd_str[i] != ' ' && cmd_str[i] != '>' && cmd_str[i] != '<')
            {
                if ((cmd_str[i] == '\'' || cmd_str[i] == '\"'))
                {
                    char file_quote = cmd_str[i++];
                    redir_buf[redir_pos++] = file_quote;
                    
                    while (cmd_str[i] && cmd_str[i] != file_quote)
                        redir_buf[redir_pos++] = cmd_str[i++];
                    
                    if (cmd_str[i])
                        redir_buf[redir_pos++] = cmd_str[i];
                }
                else
                {
                    redir_buf[redir_pos++] = cmd_str[i];
                }
                i++;
            }
            redir_buf[redir_pos++] = ' ';
            i--;
        }
        else if (cmd_str[i] == ' ')
        {
            command_buf[cmd_pos++] = cmd_str[i];
        }
        else if (is_command_char(cmd_str[i], in_quotes || paren_count > 0))
        {
            command_buf[cmd_pos++] = cmd_str[i];
        }
        i++;
    }
    command_buf[cmd_pos] = '\0';
    redir_buf[redir_pos] = '\0';
    *cmd_part = ft_strtrim(command_buf, " ");
    t_free(command_buf, __LINE__, "parsing.c");
    
    if (redir_pos > 0)
    {
        *redir_part = ft_strtrim(redir_buf, " ");
    }
    else
    {
        *redir_part = ft_strdup("");
    }
    t_free(redir_buf, __LINE__, "parsing.c");
}

void	process_all_redirections_helper(t_tree **tree, char **cmd)
{
	char	*cmd_part;
	char	*redir_part;

	cmd_part = NULL;
	redir_part = NULL;
	extract_redirections(*cmd, &cmd_part, &redir_part);
	if (cmd_part && redir_part)
	{
		t_free((*tree)->command, __LINE__, "parsing.c");
		(*tree)->command = cmd_part;
		(*tree)->redirections = redir_part;
	}
	else
	{
		if (cmd_part)
			t_free(cmd_part, __LINE__, "parsing.c");
		if (redir_part)
			t_free(redir_part, __LINE__, "parsing.c");
	}
}

void	process_all_redirections(t_tree **tree)
{
	char	*cmd;
	char	*redir_out;
	char	*redir_in;

	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		process_all_redirections(&(*tree)->left);
	if ((*tree)->right)
		process_all_redirections(&(*tree)->right);
	if (*tree && (*tree)->command)
	{
		cmd = (*tree)->command;
		redir_out = ft_strchr(cmd, '>');
		redir_in = ft_strchr(cmd, '<');
		if ((redir_out || redir_in) && check_if_in_string(cmd))
		{
			process_all_redirections_helper(tree, &cmd);
		}
	}
}