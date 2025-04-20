/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 11:50:09 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\v' || c == '\f' || c == '\r');
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

// void	extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
// {
// 	int		i;
// 	int		cmd_len;
// 	int		in_quotes;
// 	int		paren_count;
// 	char	quote_type;
// 	char	*redir_start;

// 	*cmd_part = NULL;
// 	*redir_part = NULL;
// 	if (!cmd_str)
// 		return ;
// 	i = 0;
// 	in_quotes = 0;
// 	paren_count = 0;
// 	quote_type = 0;
// 	redir_start = NULL;
// 	while (cmd_str[i])
// 	{
// 		if ((cmd_str[i] == '\'' || cmd_str[i] == '\"') && !in_quotes)
// 		{
// 			in_quotes = 1;
// 			quote_type = cmd_str[i];
// 		}
// 		else if (in_quotes && cmd_str[i] == quote_type)
// 			in_quotes = 0;
// 		if (!in_quotes)
// 		{
// 			if (cmd_str[i] == '(')
// 				paren_count++;
// 			else if (cmd_str[i] == ')')
// 				paren_count--;
// 			if ((cmd_str[i] == '>' || cmd_str[i] == '<')
// 				&& paren_count == 0 && !redir_start)
// 			{
// 				redir_start = &cmd_str[i];
// 				break ;
// 			}
// 		}
// 		i++;
// 	}
// 	if (!redir_start)
// 	{
// 		*cmd_part = ft_strdup(cmd_str);
// 		return ;
// 	}
// 	cmd_len = redir_start - cmd_str;
// 	*cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
// 	*redir_part = ft_strdup(redir_start);
// }
void extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
{
    int     i;
    int     cmd_len;
    int     in_quotes;
    int     paren_count;
    char    quote_type;
    char    *redir_start;
    char    *args_start;

    *cmd_part = NULL;
    *redir_part = NULL;
    if (!cmd_str)
        return;
    i = 0;
    in_quotes = 0;
    paren_count = 0;
    quote_type = 0;
    redir_start = NULL;
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
            if ((cmd_str[i] == '>' || cmd_str[i] == '<')
                && paren_count == 0 && !redir_start)
            {
                redir_start = &cmd_str[i];
                break;
            }
        }
        i++;
    }
    if (!redir_start)
    {
        *cmd_part = ft_strdup(cmd_str);
        return;
    }
    cmd_len = redir_start - cmd_str;
    *cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
    args_start = NULL;
    i = 0;
    in_quotes = 0;
    quote_type = 0;
    int redir_active = 0; 
    
    while (redir_start[i])
    {
        if ((redir_start[i] == '\'' || redir_start[i] == '\"') && !in_quotes)
        {
            in_quotes = 1;
            quote_type = redir_start[i];
        }
        else if (in_quotes && redir_start[i] == quote_type)
            in_quotes = 0;
        
        if (!in_quotes)
        {
            if (redir_start[i] == '>' || redir_start[i] == '<')
            {
                redir_active = 1;
            }
            else if (redir_active && redir_start[i] == ' ' && i > 0 && 
                    redir_start[i-1] != '>' && redir_start[i-1] != '<')
            {
                int j = i + 1;
                while (redir_start[j] && redir_start[j] == ' ')
                    j++;
                if (redir_start[j] && redir_start[j] != '>' && redir_start[j] != '<')
                {
                    args_start = &redir_start[j];
                    break;
                }
                else if (redir_start[j] == '>' || redir_start[j] == '<')
                {
                    i = j - 1; 
                }
                else
                {
                    break;
                }
            }
            else if (redir_active && redir_start[i] != ' ')
            {
                redir_active = 2; 
            }
        }
        i++;
    }
    if (args_start)
    {
        int redir_len = args_start - redir_start;
        *redir_part = ft_substr_leak(redir_start, 0, redir_len, __LINE__);
        char *tmp = *cmd_part;
        if (tmp[0] != '\0')
            *cmd_part = ft_strjoin_leak(tmp, " ", __LINE__);
        else
            *cmd_part = ft_strdup("");
        t_free(tmp, __LINE__, "parsing.c");
        tmp = *cmd_part;
        *cmd_part = ft_strjoin_leak(tmp, args_start, __LINE__);
        t_free(tmp, __LINE__, "parsing.c");
    }
    else
    {
        *redir_part = ft_strdup(redir_start);
    }
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
