/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:52:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/09 17:49:03 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_filtered_string_inits(int *j, int *in_single_quotes,
		int *in_double_quotes, int *i)
{
	*j = 0;
	*in_single_quotes = 0;
	*in_double_quotes = 0;
	*i = 0;
}

char	*create_filtered_string(char *old_str, int final_len)
{
	int		i;
	int		j;
	char	*new_str;
	int		in_single_quotes;
	int		in_double_quotes;

	new_str = (char *)malloc(sizeof(char) * (
				final_len + 1));
	if (!new_str)
		return (NULL);
	create_filtered_string_inits(&j, &in_single_quotes, &in_double_quotes, &i);
	while (i < (int)ft_strlen(old_str))
	{
		if (old_str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (old_str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
			new_str[j++] = old_str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	process_command_string(t_tree **tree, int k)
{
	char	*old_str;
	char	*new_str;
	int		final_len;

	old_str = (*tree)->command_arr[k];
	final_len = count_filtered_length(old_str);
	new_str = create_filtered_string(old_str, final_len);
	if (!new_str)
		return ;
	printf("*************%s\n", new_str);
	free((*tree)->command_arr[k]);
	(*tree)->command_arr[k] = new_str;
}

void	process_command_array(t_tree **tree)
{
	int	k;

	k = 0;
	while ((*tree)->command_arr[k])
	{
		if (variable_search_instr((*tree)->command_arr[k]))
		{
			k++;
			continue ;
		}
		process_command_string(tree, k);
		k++;
	}
}

void	process_command_array_two(t_tree **tree)
{
	int	k;

	k = 0;
	while ((*tree)->command_arr[k])
	{
		if (variable_search_instr((*tree)->command_arr[k]))
			process_command_string(tree, k);
		k++;
	}
}

void	quote_remove(t_tree **tree)
{
	if ((*tree) && (*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
		process_command_array(tree);
}

void	quote_remove_two(t_tree **tree)
{
	if ((*tree) && (*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
		process_command_array_two(tree);
}

void	process_lst(t_list_fd **node)
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

void	process_lnked_lst(t_tree **tree)
{
	t_list_fd	*tmp;

	tmp = (*tree)->fd_list;
	while (tmp)
	{
		process_lst(&tmp);
		tmp = tmp->next;
	}
}

void	quote_remove_lst(t_tree **tree)
{
	if ((*tree)->left)
		quote_remove_lst(&(*tree)->left);
	if ((*tree)->right)
		quote_remove_lst(&(*tree)->right);
	if ((*tree)->fd_list)
		process_lnked_lst(tree);
}
