/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:53:49 by makkach           #+#    #+#             */
/*   Updated: 2025/05/09 18:17:54 by makkach          ###   ########.fr       */
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

int	variable_expantion_two(char **redirname,
	int var_pos, t_env **env, int *flag)
{
	int		name_end;
	char	*var_name;
	char	*before;
	char	*after;
	char	*value;
	char	*new_redir;
	t_env	*tmp;

	if (!(*redirname)[var_pos])
		return (-1);
	name_end = var_pos + 1;
	while ((*redirname)[name_end])
	{
		if (((*redirname)[name_end] >= 'a' && (*redirname)[name_end] <= 'z'
			) || ((*redirname)[name_end] >= 'A' && (*redirname)[name_end] <= 'Z'
			) || ((*redirname)[name_end] >= '0' && (*redirname)[name_end] <= '9'
			))
			name_end++;
		else
			break ;
	}
	var_name = ft_substr((*redirname), var_pos + 1, name_end - var_pos - 1);
	if (!var_name)
		return (-1);
	tmp = *env;
	value = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, var_name))
		{
			value = ft_strdup(tmp->value);
			if (countwords(value, 32) != 1)
				*flag = 1;
			break ;
		}
		tmp = tmp->next;
	}
	free(var_name);
	before = ft_substr((*redirname), 0, var_pos);
	if (!before)
		return (free(value), -1);
	after = ft_substr((*redirname), name_end,
			ft_strlen((*redirname)) - name_end);
	if (!after)
		return (free(value), free(before), -1);
	if (value)
		new_redir = ft_strjoin_three(before, value, after);
	else
		new_redir = ft_strdup(*redirname);
	if (!new_redir)
		return (free(before), free(value), free(after), -1);
	free((*redirname));
	(*redirname) = new_redir;
	return (free(before), free(value), free(after), 0);
}

void	variable_expantion_inlnkedlst(t_tree **tree, t_env **env)
{
	t_list_fd	*tmp;
	int			i;
	int			in_quotes;
	int			flag;
	char		quote_type;

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
		{
			i = 0;
			in_quotes = 0;
			flag = 0;
			while (tmp->name && tmp->name[i])
			{
				if (!in_quotes && (tmp->name[i] == '"' || tmp->name[i] == '\''))
				{
					in_quotes = 1;
					quote_type = tmp->name[i];
				}
				else if (tmp->name[i] == quote_type)
					in_quotes = 0;
				if (tmp->name[i] == '$' && (
						!in_quotes || (in_quotes && quote_type == '"')))
				{
					if (variable_expantion_two(&tmp->name, i, env, &flag) == -1)
						break ;
					if (tmp->name && countwords(tmp->name, 32) != 1)
						tmp->name_split = ft_split(tmp->name, 32);
					if (flag == 1)
						(*tree)->ambiguous = 1;
					if (!variable_search_inlnkedlst(tree) || !variable_search_instr(tmp->name))
						i = -1;
				}
				i++;
			}
		}
	}
}
