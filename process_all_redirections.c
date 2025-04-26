/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:29 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:05:30 by makkach          ###   ########.fr       */
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

static void	update_quote_state(char c, int *in_quotes, char *quote_type)
{
	if ((c == '\'' || c == '\"') && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = c;
	}
	else if (*in_quotes && c == *quote_type)
		*in_quotes = 0;
}

static char	*find_redirection_start(char *cmd_str)
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

static char	*check_for_valid_args(char *redir_start, int j)
{
	while (redir_start[j] && redir_start[j] == ' ')
		j++;
	if (redir_start[j] && redir_start[j] != '>' && redir_start[j] != '<')
		return (&redir_start[j]);
	return (NULL);
}

static int	handle_space_after_redir(char *redir_start, int i, char **args)
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

static int	update_redir_state(char *redir_start, int i, int *redir_active)
{
	if (redir_start[i] == '>' || redir_start[i] == '<')
		*redir_active = 1;
	else if ((*redir_active == 1) && redir_start[i] != ' ')
		*redir_active = 2;
	return (i);
}

static char	*while_loop_args_aftredirs(char *redir_start,
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
