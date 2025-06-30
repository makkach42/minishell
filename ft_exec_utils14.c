/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils14.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:54:54 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/30 14:16:18 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_execute_com(t_tree *tree, t_hdoc_data *h_data)
{
	if (variable_search_inlnkedlst(&tree) == 1)
		variable_expantion_inlnkedlst(&tree, h_data->env);
	quote_remove_lst(&tree);
	ambiguous_set(&tree);
	if (ambiguous_syntax_error(&tree, h_data->env) == 1)
		(write(2, "ambiguous redirect\n", 19));
	tree->status = ft_cmd_redir(tree, h_data);
}

void	ft_execute_redir(t_tree *tree, t_env **h, char **e)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (variable_search_inlnkedlst(&tree) == 1)
		variable_expantion_inlnkedlst(&tree, h);
	ambiguous_set(&tree);
	quote_remove_lst(&tree);
	if (ambiguous_syntax_error(&tree, h) == 1)
		(write(2, "ambiguous redirect\n", 19));
	ft_exec_redir(tree, h, e);
	exit (0);
}

void	ft_var_helper(t_tree **tree, char **e, int *check, t_hdoc_data *h_data)
{
	reset_vars(tree, h_data->env, h_data);
	(*tree)->status = ft_variable(*tree, h_data, e, check);
}
int	ft_variable(t_tree *tree, t_hdoc_data *h_data, char **e, int *check)
{
	if (!tree->command || !tree->command_arr || (
			!tree->command_arr[0]))
		return (0);
	if (cmd_check(tree) == 1 && tree->redirections == NULL)
		ft_word_handle(tree, h_data->env, e, check);
	else if (cmd_check(tree) == 1 && tree->redirections != NULL)
		ft_word_redir(tree, h_data->env, e, check);
	else if (cmd_check(tree) == 0)
		tree->status = ft_cmd_exec(tree, h_data);
	else
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, tree->command_arr[0]);
		ft_putstr_fd(2, ": command not found\n");
	}
	return (tree->status);
}

void	ft_hdoc_free(char **str, char **limiter, int fd)
{
	free (*str);
	free (*limiter);
	close (fd);
}
