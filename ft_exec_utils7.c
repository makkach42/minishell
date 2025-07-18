/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:14:27 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/09 15:07:21 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_word_redir(t_tree *tree, t_hdoc_data *h_data, char **e, int *check)
{
	int	id;

	id = 0;
	display_terminal_control_chars();
	if (!*check)
	{
		id = fork();
		if (id == 0)
			ft_execute_redir(tree, h_data, e);
		ft_signal_ign();
	}
	else
		ft_execute_redir(tree, h_data, e);
	close (tree->fd_list->fd1);
	waitpid(id, &(tree->status), 0);
	ft_word_handle_signal(tree, check);
}

void	ft_check_exit(int *check, int status)
{
	if (*check)
		exit (status);
}

int	ft_cmd_word_check(t_tree *tree)
{
	if (!ft_strcmp("COMMAND", tree->type) || !ft_strcmp("WORD", tree->type)
		|| !ft_strcmp("REDIRECTION", tree->type))
		return (0);
	return (1);
}

void	ft_cmd_word_helper(t_tree *tree, t_hdoc_data *h_data,
							int *check, char **e)
{
	if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections == NULL)
		|| (ft_strcmp("WORD", tree->type) == 0 && cmd_check(tree) == 0))
	{
		reset_vars(&tree, h_data->env, h_data);
		tree->status = ft_cmd_exec(tree, h_data);
		ft_check_exit(check, tree->status);
	}
	else if ((ft_strcmp("COMMAND", tree->type) == 0 && tree->redirections)
		|| (cmd_check(tree) == 0 && ft_strcmp("REDIRECTION", tree->type) == 0))
		(ft_execute_com(tree, h_data), ft_check_exit(check, tree->status));
	else if (tree->redirections == NULL && ft_strcmp("WORD", tree->type) == 0
		&& cmd_check(tree) == 1)
		(ft_word_expand(tree, h_data),
			ft_word_handle(tree, h_data->env, e, check));
	else if ((tree->redirections != NULL && ft_strcmp("WORD", tree->type) == 0)
		|| (ft_strcmp("REDIRECTION", tree->type) == 0 && cmd_check(tree) == 1))
	{
		reset_vars(&tree, h_data->env, h_data);
		variable_expantion_inlnkedlst(&tree, h_data);
		if (has_wild_cards_fdlst(&tree) == 1)
			handle_wildcards_in_fdlst(&tree);
		ft_word_redir(tree, h_data, e, check);
	}
}

void	ft_cmd_word(t_tree *tree, t_hdoc_data *h_data, int *check, char **e)
{
	if (!ft_strcmp("WORD", tree->type) && !cmd_check(tree)
		&& !tree->redirections)
	{
		reset_vars(&tree, h_data->env, h_data);
		tree->status = ft_cmd_exec(tree, h_data);
		ft_check_exit(check, tree->status);
	}
	else if (!ft_strcmp("WORD", tree->type) && !cmd_check(tree)
		&& tree->redirections)
	{
		ft_execute_com(tree, h_data);
		ft_check_exit(check, tree->status);
	}
	else
		ft_cmd_word_helper(tree, h_data, check, e);
}
