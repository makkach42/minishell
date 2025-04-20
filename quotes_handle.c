/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:52:23 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 13:37:46 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_remove_two(t_tree **tree)
{
	int		i;
	int		j;
	int		k;
	char	*old_str;
	char	*new_str;
	int		len;
	int		final_len;
	int		in_single_quotes;
	int		in_double_quotes;

	if ((*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree)->command_arr)
	{
		k = 0;
		while ((*tree)->command_arr[k])
		{
			old_str = (*tree)->command_arr[k];
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
			new_str = (char *)t_malloc(sizeof(char) * (
						final_len + 1), 2341, "parsing.c");
			if (!new_str)
			{
				k++;
				continue ;
			}
			j = 0;
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
				{
					new_str[j] = old_str[i];
					j++;
				}
				i++;
			}
			new_str[j] = '\0';
			t_free((*tree)->command_arr[k], 2370, "parsing.c");
			(*tree)->command_arr[k] = new_str;
			k++;
		}
	}
}
