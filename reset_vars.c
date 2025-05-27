/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:08:07 by makkach           #+#    #+#             */
/*   Updated: 2025/05/27 17:25:50 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_dollar(t_list **tmp, int *in_quotes, t_env **env, int *j)
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

void	expanding(t_tree **tree, int i, t_env **env)
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
	new_str = ft_strdup(tmp->data);
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

void	process_list_size(char ***cmd2, int *i, t_list *head, int list_size)
{
	char	**cmd;
	int		j;
	t_list	*tmp;
	char	*new_str;

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

int	chararrcount(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

char	***cmd2_fill(t_tree **tree)
{
	int		i;
	int		list_size;
	char	***cmd2;
	char	*old_cmd;
	t_list	*head;

	i = chararrcount((*tree)->command_arr);
	cmd2 = malloc(sizeof(char **) * (i + 1));
	i = 0;
	while ((*tree)->command_arr[i])
	{
		old_cmd = ft_strdup((*tree)->command_arr[i]);
		head = list_init(old_cmd);
		list_size = lst_size(&head);
		if (list_size == 0)
		{
			process_no_list_size(head, list_size, cmd2, &i);
			free(old_cmd);
		}
		else
			process_list_size(cmd2, &i, head, list_size);
	}
	cmd2[i] = NULL;
	return (cmd2);
}

void	free_twod_char(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	count_triple_ptr(char ***cmd2)
{
	int	m;
	int	i;
	int	j;

	m = 0;
	i = 0;
	j = 0;
	while (cmd2[i])
	{
		j = 0;
		while (cmd2[i][j])
		{
			m++;
			j++;
		}
		i++;
	}
	return (m);
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

void	free_three_d(char ***cmd2)
{
	int	i;
	int	j;

	i = -1;
	while (cmd2[++i])
	{
		if (cmd2[i])
		{
			j = -1;
			while (cmd2[i][++j])
			{
				if (cmd2[i][j])
				{
					free(cmd2[i][j]);
					cmd2[i][j] = NULL;
				}
			}
			free(cmd2[i]);
			cmd2[i] = NULL;
		}
	}
	(free(cmd2), cmd2 = NULL);
}

void	reset_var_cmd_split(t_tree **tree)
{
	int		i;
	char	***cmd2;

	cmd2 = cmd2_fill(tree);
	free_twod_char((*tree)->command_arr);
	i = count_triple_ptr(cmd2);
	(*tree)->command_arr = malloc(sizeof(char *) * (i + 1));
	fill_twod_char_with_triple((*tree)->command_arr, cmd2);
	free_three_d(cmd2);
}

void	remove_quotes_from_var(char	**arr)
{
	int		i;
	char	*old_cmd;
	int		final_len;
	char	*new_str;

	i = 0;
	while (arr[i])
	{
		if (ft_strchr(arr[i], '"'
			) || ft_strchr(
				arr[i], '\''))
		{
			old_cmd = arr[i];
			final_len = count_filtered_length(old_cmd);
			new_str = create_filtered_string(old_cmd, final_len);
			if (!new_str)
				return ;
			free(arr[i]);
			arr[i] = new_str;
		}
		i++;
	}
}

void	reset_var_remove_quotes(t_tree **tree)
{
	int	i;
	int	j;

	remove_quotes_from_var((*tree)->command_arr);
	i = 0;
	j = 0;
	while ((*tree)->command_arr[i])
	{
		j = 0;
		while ((*tree)->command_arr[i][j])
		{
			if ((*tree)->command_arr[i][j] == 10)
				(*tree)->command_arr[i][j] = '"';
			else if ((*tree)->command_arr[i][j] == 11)
				(*tree)->command_arr[i][j] = '\'';
			j++;
		}
		i++;
	}
}

void	reset_vars(t_tree **tree, t_env **env)
{
	t_list	*head;

	if ((*tree) && (*tree)->left)
		reset_vars(&(*tree)->left, env);
	if ((*tree) && (*tree)->right)
		reset_vars(&(*tree)->right, env);
	if ((*tree) && (*tree)->command_arr)
	{
		head = NULL;
		reset_var_expand_var(tree, env);
		reset_var_cmd_split(tree);
		reset_var_remove_quotes(tree);
	}
}
