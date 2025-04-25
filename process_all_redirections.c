/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 14:56:39 by makkach          ###   ########.fr       */
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

void	handle_args_found(char **cmd_part, char **redir_part, char *redir_start,
					int args_pos)
{
	int		redir_len;
	char	*tmp;
	char	*args_start;

	args_start = &redir_start[args_pos];
	redir_len = args_start - redir_start;
	*redir_part = ft_substr_leak(redir_start, 0, redir_len, __LINE__);
	tmp = *cmd_part;
	if (tmp[0] != '\0')
		*cmd_part = ft_strjoin_leak(tmp, " ", __LINE__);
	else
		*cmd_part = ft_strdup("");
	t_free(tmp, __LINE__, "parsing.c");
	tmp = *cmd_part;
	*cmd_part = ft_strjoin_leak(tmp, args_start, __LINE__);
	t_free(tmp, __LINE__, "parsing.c");
}

void	extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
{
	char	*redir_start;
	int		cmd_len;
	int		args_pos;

	*cmd_part = NULL;
	*redir_part = NULL;
	if (!cmd_str)
		return ;
	redir_start = find_first_redirection(cmd_str);
	if (!redir_start)
	{
		*cmd_part = ft_strdup(cmd_str);
		return ;
	}
	cmd_len = redir_start - cmd_str;
	*cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
	args_pos = find_args_start_pos(redir_start);
	if (args_pos != -1)
		handle_args_found(cmd_part, redir_part, redir_start, args_pos);
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
