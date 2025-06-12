/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_vars_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 07:46:27 by makkach           #+#    #+#             */
/*   Updated: 2025/06/12 15:42:56 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_dollar(t_list **tmp, int *in_quotes, t_env **env, int *j)
{
	if ((*tmp)->data[*j] == '$' && ((
				*in_quotes && (*tmp)->data[*j + 1] && (
					(*tmp)->data[*j + 1
					] == '"' || (*tmp)->data[
						*j + 1] == '\'')) || (
				!*in_quotes && !(*tmp)->data[*j + 1])))
		(*j)++;
	else
	{
		process_array_variable(&(*tmp)->data, 0, j, env);
		*j = -1;
		*in_quotes = 0;
	}
}

void	reset_var_expand(t_list	*tmp, t_env **env)
{
	int		in_quotes;
	int		j;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	j = 0;
	while (tmp->data && tmp->data[j])
	{
		if (!in_quotes && (tmp->data[j
				] == '"' || tmp->data[j] == '\''))
		{
			in_quotes = 1;
			quote_type = tmp->data[j];
		}
		else if (in_quotes && tmp->data[j] == quote_type)
			in_quotes = 0;
		if (tmp->data[j] == '$' && (!in_quotes || (
					in_quotes && quote_type == '"')))
			expand_dollar(&tmp, &in_quotes, env, &j);
		j++;
	}
}

static void	expanding(t_tree **tree, int i, t_env **env)
{
	char	*old_cmd;
	char	*new_str;
	t_list	*head;
	t_list	*tmp;

	old_cmd = ft_strdup((*tree)->command_arr[i]);
	head = list_init(old_cmd);
	tmp = head;
	while (tmp)
	{
		reset_var_expand(tmp, env);
		tmp = tmp->next;
	}
	tmp = head;
	if (tmp && tmp->data)
		new_str = ft_strdup(tmp->data);
	else
		new_str = ft_strdup("");
	free((*tree)->command_arr[i]);
	(*tree)->command_arr[i] = new_str;
	free_list(&head);
}

void	reset_var_expand_var(t_tree **tree, t_env **env)
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
			expanding(tree, i, env);
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
