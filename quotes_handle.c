/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:52:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 08:36:36 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	str_extractor_helper(char *str, int *i, int *flag, char **word)
{
	int		in_quotes;
	char	quote_type;

	*flag = 0;
	in_quotes = 0;
	quote_type = 0;
	while (str[*i])
	{
		if (!in_quotes && (str[*i] == '"' || str[*i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[*i];
		}
		else if (in_quotes && (str[*i] == quote_type))
			in_quotes = 0;
		if (!in_quotes && (str[*i] && (
					str[*i] == 32 || str[*i
					] == '\0' || is_operator(
						str[*i]))))
			break ;
		(*i)++;
	}
	*word = ft_substr(str, 0, *i);
}

static void	process_lst(t_list_fd **node)
{
	char		*old_str;
	char		*new_str;
	int			final_len;

	old_str = (*node)->name;
	final_len = count_filtered_length(old_str);
	new_str = create_filtered_string(old_str, final_len);
	if (!new_str)
		return ;
	free((*node)->name);
	(*node)->name = new_str;
}

static void	process_lnked_lst(t_tree **tree)
{
	t_list_fd	*tmp;
	int			i;

	i = 0;
	tmp = (*tree)->fd_list;
	while (tmp)
	{
		process_lst(&tmp);
		while (tmp->name[i])
		{
			if (tmp->name[i] == 10)
				tmp->name[i] = '"';
			else if (tmp->name[i] == 11)
				tmp->name[i] = '\'';
			i++;
		}
		tmp = tmp->next;
	}
}

void	fill_twod_char_with_triple(char **arr, char ***cmd2)
{
	int	i;
	int	j;
	int	m;

	i = 0;
	j = 0;
	m = 0;
	while (cmd2[i])
	{
		j = 0;
		while (cmd2[i][j])
		{
			arr[m] = ft_strdup(cmd2[i][j]);
			m++;
			j++;
		}
		i++;
	}
	arr[m] = NULL;
}
// void	process_lnked_lst_two(t_tree **tree)
// {
// 	t_list_fd	*tmp;

// 	tmp = (*tree)->fd_list;
// 	while (tmp)
// 	{
// 		if (variable_search_instr(tmp->name))
// 			process_lst(&tmp);
// 		tmp = tmp->next;
// 	}
// }

void	quote_remove_lst(t_tree **tree)
{
	if ((*tree)->left)
		quote_remove_lst(&(*tree)->left);
	if ((*tree)->right)
		quote_remove_lst(&(*tree)->right);
	if ((*tree)->fd_list)
		process_lnked_lst(tree);
}

// void	quote_remove_lst_two(t_tree **tree)
// {
// 	if ((*tree)->left)
// 		quote_remove_lst(&(*tree)->left);
// 	if ((*tree)->right)
// 		quote_remove_lst(&(*tree)->right);
// 	if ((*tree)->fd_list)
// 		process_lnked_lst_two(tree);
// }
