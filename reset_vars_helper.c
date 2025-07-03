/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_vars_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 07:46:27 by makkach           #+#    #+#             */
/*   Updated: 2025/07/03 19:53:15 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_dollar(t_tmp_tree *tmp, int *in_quotes,
						int *j, t_hdoc_data *h_data)
{
	int		n;

	if (tmp->tmp->data[*j] == '$' && ((
				*in_quotes && tmp->tmp->data[*j + 1] && (
					tmp->tmp->data[*j + 1
					] == '"' || tmp->tmp->data[
						*j + 1] == '\'')) || (
				!*in_quotes && !tmp->tmp->data[*j + 1])))
		(*j)++;
	else
	{
		n = *j;
		process_array_variable(&tmp->tmp->data, 0, j, h_data->env);
		tmp->tree->var = 1;
		if (*j == -1)
			if_question_mark(&tmp, n, h_data);
		*j = -1;
		*in_quotes = 0;
	}
}

void	reset_var_expand(t_tmp_tree	*tmp, t_hdoc_data *h_data)
{
	int		in_quotes;
	int		j;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	j = 0;
	while (tmp->tmp->data && tmp->tmp->data[j])
	{
		if (!in_quotes && (tmp->tmp->data[j
				] == '"' || tmp->tmp->data[j] == '\''))
		{
			in_quotes = 1;
			quote_type = tmp->tmp->data[j];
		}
		else if (in_quotes && tmp->tmp->data[j] == quote_type)
			in_quotes = 0;
		if (tmp->tmp->data[j] == '$' && (!in_quotes || (
					in_quotes && quote_type == '"')))
			expand_dollar(tmp, &in_quotes, &j, h_data);
		if (j >= 0 && !tmp->tmp->data[j])
			j--;
		j++;
	}
}

static void	expanding(t_tree **tree, int i, t_hdoc_data *h_data)
{
	char		*old_cmd;
	char		*new_str;
	t_list		*head;
	t_tmp_tree	tmp;

	old_cmd = ft_strdup((*tree)->command_arr[i]);
	head = list_init(old_cmd);
	tmp.tmp = head;
	tmp.tree = (*tree);
	while (tmp.tmp)
	{
		reset_var_expand(&tmp, h_data);
		tmp.tmp = tmp.tmp->next;
	}
	tmp.tmp = head;
	if (tmp.tmp && tmp.tmp->data)
		new_str = ft_strdup(tmp.tmp->data);
	else
		new_str = ft_strdup("");
	free((*tree)->command_arr[i]);
	(*tree)->command_arr[i] = new_str;
	free_list(&head);
}

void	reset_var_expand_var(t_tree **tree, t_hdoc_data *h_data)
{
	int		i;

	i = 0;
	while ((*tree)->command_arr[i])
	{
		if (ft_strchr((*tree)->command_arr[i], '"'
			) || ft_strchr(
				(*tree)->command_arr[i], '\''
			) || ft_strchr(
				(*tree)->command_arr[i], '$'))
		{
			expanding(tree, i, h_data);
		}
		i++;
	}
}

void	process_no_list_size(t_list	*head, int list_size, char ***cmd2, int *i)
{
	char	**cmd;
	char	*new_str;
	int		j;
	t_list	*tmp;

	cmd = malloc(sizeof(char *) * (list_size + 1));
	j = 0;
	tmp = head;
	while (tmp)
	{
		new_str = ft_strdup(tmp->data);
		cmd[j] = new_str;
		j++;
		tmp = tmp->next;
	}
	cmd[j] = NULL;
	cmd2[*i] = cmd;
	free_list(&head);
	(*i)++;
}
