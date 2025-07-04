/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_handle_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:53:49 by makkach           #+#    #+#             */
/*   Updated: 2025/07/04 11:10:27 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_in_lnkdlst(t_tmp_tree **tmp,
			t_hdoc_data *h_data, int *i, int *in_quotes)
{
	char	*tmp_char;
	int		n;

	if ((*tmp)->tmp_fd->name[*i] == '$' && ((
				*in_quotes && (*tmp)->tmp_fd->name[*i + 1
				] && ((*tmp)->tmp_fd->name[*i + 1
					] == '"' || (*tmp)->tmp_fd->name[*i + 1
					] == '\'')) || (
				!*in_quotes && !(*tmp)->tmp_fd->name[*i + 1])))
		i++;
	else
	{
		n = *i;
		process_array_variable(&(*tmp)->tmp_fd->name, 0, i, h_data->env);
		if (*i == -1)
			if_question_mark_three(tmp, n, h_data);
		tmp_char = (*tmp)->tmp_fd->name;
		(*tmp)->tmp_fd->name = ft_strtrim((*tmp)->tmp_fd->name, " ");
		if (!(*tmp)->tmp_fd->name)
			return ;
		free(tmp_char);
		*in_quotes = 0;
		*i = -1;
	}
}

void	quote_filtering_and_expantion(t_tmp_tree **tmp, t_hdoc_data *h_data)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	while ((*tmp)->tmp_fd->name && (*tmp)->tmp_fd->name[i])
	{
		if (!in_quotes && ((*tmp)->tmp_fd->name[
					i] == '"' || (*tmp)->tmp_fd->name[i] == '\''))
		{
			in_quotes = 1;
			quote_type = (*tmp)->tmp_fd->name[i];
		}
		else if ((*tmp)->tmp_fd->name[i] == quote_type)
			in_quotes = 0;
		if ((*tmp)->tmp_fd->name[i] == '$' && (
				!in_quotes || (in_quotes && quote_type == '"')))
			expand_in_lnkdlst(tmp, h_data, &i, &in_quotes);
		i++;
	}
}

// void	variable_expantion_para(t_tree **tree, t_env **env)
// {
// 	t_list_fd	*tmp;

// 	if ((*tree)->left)
// 		variable_expantion_para(&(*tree)->left, env);
// 	if ((*tree)->right)
// 		variable_expantion_para(&(*tree)->right, env);
// 	if ((*tree)->fd_list)
// 	{
// 		tmp = (*tree)->fd_list;
// 		while (tmp)
// 		{
// 			if (variable_search_instr(tmp->name))
// 				break ;
// 			tmp = tmp->next;
// 		}
// 		if (tmp)
// 			quote_filtering_and_expantion(&tmp, env);
// 	}
// }

void	variable_expantion_inlnkedlst(t_tree **tree, t_hdoc_data *h_data)
{
	t_tmp_tree	*tmp;

	tmp = malloc(sizeof(t_tmp_tree));
	tmp->tree = *tree;
	if ((*tree)->fd_list)
	{
		tmp->tmp_fd = (*tree)->fd_list;
		while (tmp->tmp_fd)
		{
			if (variable_search_instr(tmp->tmp_fd->name))
				break ;
			tmp->tmp_fd = tmp->tmp_fd->next;
		}
		if (tmp->tmp_fd)
			quote_filtering_and_expantion(&tmp, h_data);
	}
	free(tmp);
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
