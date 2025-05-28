/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:53:49 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 10:58:28 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_search_inlnkedlst(t_tree **tree)
{
	t_list_fd	*tmplst;
	int			i;

	i = 0;
	if ((*tree) && (*tree)->fd_list)
	{
		tmplst = (*tree)->fd_list;
		while (tmplst)
		{
			if (variable_search_instr(tmplst->name
				) && ft_strcmp(tmplst->redir, "<<"))
				i = 1;
			tmplst = tmplst->next;
		}
	}
	if ((*tree) && (*tree)->left)
		i += variable_search_inlnkedlst(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		i += variable_search_inlnkedlst(&(*tree)->right);
	return (i);
}

void	expand_in_lnkdlst(t_list_fd **tmp, t_env **env, int *i, int *in_quotes)
{
	char	*tmp_char;

	if ((*tmp)->name[*i] == '$' && ((
				*in_quotes && (*tmp)->name[*i + 1
				] && ((*tmp)->name[*i + 1
					] == '"' || (*tmp)->name[*i + 1
					] == '\'')) || (
				!*in_quotes && !(*tmp)->name[*i + 1])))
		i++;
	else
	{
		process_array_variable(&(*tmp)->name, 0, i, env);
		tmp_char = (*tmp)->name;
		(*tmp)->name = ft_strtrim((*tmp)->name, " ");
		free(tmp_char);
		if ((*tmp)->name && countwords((*tmp)->name, 32) != 1)
			(*tmp)->name_split = ft_split((*tmp)->name, 32);
		*in_quotes = 0;
		*i = -1;
	}
}

void	quote_filtering_and_expantion(t_list_fd **tmp, t_env **env)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	while ((*tmp)->name && (*tmp)->name[i])
	{
		if (!in_quotes && ((*tmp)->name[i] == '"' || (*tmp)->name[i] == '\''))
		{
			in_quotes = 1;
			quote_type = (*tmp)->name[i];
		}
		else if ((*tmp)->name[i] == quote_type)
			in_quotes = 0;
		if ((*tmp)->name[i] == '$' && (
				!in_quotes || (in_quotes && quote_type == '"')))
			expand_in_lnkdlst(tmp, env, &i, &in_quotes);
		i++;
	}
}

void	variable_expantion_inlnkedlst(t_tree **tree, t_env **env)
{
	t_list_fd	*tmp;

	if ((*tree)->left)
		variable_expantion_inlnkedlst(&(*tree)->left, env);
	if ((*tree)->right)
		variable_expantion_inlnkedlst(&(*tree)->right, env);
	if ((*tree)->fd_list)
	{
		tmp = (*tree)->fd_list;
		while (tmp)
		{
			if (variable_search_instr(tmp->name))
				break ;
			tmp = tmp->next;
		}
		if (tmp)
			quote_filtering_and_expantion(&tmp, env);
	}
}

int	variable_search(t_tree **tree)
{
	int	found_in_current;
	int	found_in_left;
	int	found_in_right;
	int	j;

	found_in_current = 0;
	found_in_left = 0;
	found_in_right = 0;
	j = 0;
	if ((*tree) && (*tree)->command_arr)
	{
		while ((*tree)->command_arr[j])
		{
			if (variable_search_instr((*tree)->command_arr[j]))
				found_in_current = 1;
			j++;
		}
	}
	if ((*tree) && (*tree)->left)
		found_in_left = variable_search(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		found_in_right = variable_search(&(*tree)->right);
	return (found_in_current || found_in_left || found_in_right);
}
