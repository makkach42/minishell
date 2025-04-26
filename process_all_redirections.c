/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:14:16 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*find_args_after_redirection(char *redir_start)
{
	int		in_quotes;
	char	quote_type;
	int		redir_active;
	char	*args;

	in_quotes = 0;
	quote_type = 0;
	redir_active = 0;
	args = NULL;
	args = while_loop_args_aftredirs(redir_start,
			in_quotes, quote_type, redir_active);
	return (args);
}

static void	join_cmd_with_args(char **cmd_part, char *args_start,
			char *redir_start)
{
	int		redir_len;
	char	*tmp;
	char	*redir_part;

	redir_len = args_start - redir_start;
	redir_part = ft_substr(redir_start, 0, redir_len);
	tmp = *cmd_part;
	if (tmp[0] != '\0')
		*cmd_part = ft_strjoin(tmp, " ");
	else
		*cmd_part = ft_strdup("");
	free(tmp);
	tmp = *cmd_part;
	*cmd_part = ft_strjoin(tmp, args_start);
	free(tmp);
	free(redir_part);
}

void	extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
{
	char	*redir_start;
	char	*args_start;
	int		cmd_len;

	*cmd_part = NULL;
	*redir_part = NULL;
	if (!cmd_str)
		return ;
	redir_start = find_redirection_start(cmd_str);
	if (!redir_start)
	{
		*cmd_part = ft_strdup(cmd_str);
		return ;
	}
	cmd_len = redir_start - cmd_str;
	*cmd_part = ft_substr(cmd_str, 0, cmd_len);
	args_start = find_args_after_redirection(redir_start);
	if (args_start)
	{
		join_cmd_with_args(cmd_part, args_start, redir_start);
		*redir_part = ft_substr(redir_start, 0, args_start - redir_start);
	}
	else
		*redir_part = ft_strdup(redir_start);
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
		free((*tree)->command);
		(*tree)->command = cmd_part;
		(*tree)->redirections = redir_part;
	}
	else
	{
		if (cmd_part)
			free(cmd_part);
		if (redir_part)
			free(redir_part);
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
