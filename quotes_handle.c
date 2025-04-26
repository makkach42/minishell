/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:52:23 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 09:42:19 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_filtered_length(char *old_str)
{
	int	i;
	int	len;
	int	final_len;
	int	in_single_quotes;
	int	in_double_quotes;

	len = ft_strlen(old_str);
	final_len = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	i = 0;
	while (i < len)
	{
		if (old_str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (old_str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
			final_len++;
		i++;
	}
	return (final_len);
}

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
	int		len;
	char	*new_str;
	int		in_single_quotes;
	int		in_double_quotes;

	new_str = (char *)malloc(sizeof(char) * (
				final_len + 1));
	if (!new_str)
		return (NULL);
	len = ft_strlen(old_str);
	create_filtered_string_inits(&j, &in_single_quotes, &in_double_quotes, &i);
	while (i < len)
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
			free((*tree)->command_arr[k]);
			(*tree)->command_arr[k] = new_str;
}

void	process_command_array(t_tree **tree)
{
	int	k;

	k = 0;
	while ((*tree)->command_arr[k])
	{
		process_command_string(tree, k);
			k++;
		}
	}

void	quote_remove_two(t_tree **tree)
{
	if ((*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree)->command_arr)
		process_command_array(tree);
}
