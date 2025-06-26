/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils14.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:54:54 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/26 11:55:06 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_execute_com(t_tree *tree, t_env **h)
{
	if (variable_search_inlnkedlst(&tree) == 1)
		variable_expantion_inlnkedlst(&tree, h);
	quote_remove_lst(&tree);
	ambiguous_set(&tree);
	if (ambiguous_syntax_error(&tree, h) == 1)
		(write(2, "ambiguous redirect\n", 19));
	tree->status = ft_cmd_redir(tree, h);
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
	(*tree)->status = ft_variable(*tree, h_data->env, e, check);
}
