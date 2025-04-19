/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 13:46:20 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_in_string(char *cmd)
{
	int i;
	int	in_quotes;
	char quote_type;

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
			break;
		i++;
	}
	if (cmd[i] == '\0')
		return (0);
	return (1);
}

void extract_redirections(char *cmd_str, char **cmd_part, char **redir_part) {
    int i;
    int cmd_len;
    int in_quotes;
    int paren_count;
    char quote_type;
    char *redir_start;
    
    *cmd_part = NULL;
    *redir_part = NULL;
    
    if (!cmd_str)
        return;
    
    i = 0;
    in_quotes = 0;
    paren_count = 0;
    quote_type = 0;
    redir_start = NULL;
    while (cmd_str[i]) {
        if ((cmd_str[i] == '\'' || cmd_str[i] == '\"') && !in_quotes) {
            in_quotes = 1;
            quote_type = cmd_str[i];
        } else if (in_quotes && cmd_str[i] == quote_type) {
            in_quotes = 0;
        }
        if (!in_quotes) {
            if (cmd_str[i] == '(') {
                paren_count++;
            } else if (cmd_str[i] == ')') {
                paren_count--;
            }
            if ((cmd_str[i] == '>' || cmd_str[i] == '<') && paren_count == 0 && !redir_start) {
                redir_start = &cmd_str[i];
                break;
            }
        }
        
        i++;
    }
    if (!redir_start) {
        *cmd_part = ft_strdup(cmd_str);
        return;
    }
    cmd_len = redir_start - cmd_str;
    *cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
    *redir_part = ft_strdup(redir_start);
}

void process_all_redirections(t_tree **tree) {
    if (!tree || !*tree)
        return;
    if ((*tree)->left)
        process_all_redirections(&(*tree)->left);
    if ((*tree)->right)
        process_all_redirections(&(*tree)->right);
    if (*tree && (*tree)->command) {
        char *cmd = (*tree)->command;
        char *redir_out = ft_strchr(cmd, '>');
        char *redir_in = ft_strchr(cmd, '<');
        if ((redir_out || redir_in) && check_if_in_string(cmd)) {
            char *cmd_part = NULL;
            char *redir_part = NULL;
            extract_redirections(cmd, &cmd_part, &redir_part);
            if (cmd_part && redir_part) {
                t_free((*tree)->command, __LINE__, "parsing.c");
                (*tree)->command = cmd_part;
                (*tree)->redirections = redir_part;
            } else {
                if (cmd_part)
                    t_free(cmd_part, __LINE__, "parsing.c");
                if (redir_part)
                    t_free(redir_part, __LINE__, "parsing.c");
            }
        }
    }
}