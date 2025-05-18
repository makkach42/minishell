/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:29 by makkach           #+#    #+#             */
/*   Updated: 2025/05/18 11:11:28 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_filtered_length(char *old_str, int *var_flag)
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
		{
			if (old_str[i] == '$' && in_single_quotes && !in_double_quotes && var_flag)
				*var_flag = 2;
			final_len++;
		}
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

void	extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
{
	t_extract	v;

	*cmd_part = NULL;
	*redir_part = NULL;
	if (!cmd_str)
		return ;
	if (!initialize_buffers(&v, cmd_str))
		return ;
	init_extraction_vars(&v.i, &v.in_quotes, &v.quote_type, &v.paren_count);
	if (!process_command_string_two(&v, cmd_str))
	{
		dyn_buf_free(&v.command_buf);
		dyn_buf_free(&v.redir_buf);
		return ;
	}
	cleanup_and_assign(&v.command_buf, &v.redir_buf, cmd_part, redir_part);
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
