/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handle_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:25:20 by makkach           #+#    #+#             */
/*   Updated: 2025/05/20 10:40:22 by makkach          ###   ########.fr       */
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

char	*create_filtered_string(char *old_str, int final_len, int l, int k)
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
	if (l != k)
	{
		while (i < (int)ft_strlen(old_str))
		{
			if (old_str[i] == '\'' && !in_double_quotes)// && (i < l || i > k)
				in_single_quotes = !in_single_quotes;
			else if (old_str[i] == '\"' && !in_single_quotes)// && (i < l || i > k)
				in_double_quotes = !in_double_quotes;
			else
				new_str[j++] = old_str[i];
			i++;
		}
	}
	else
	{
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
	final_len = count_filtered_length(old_str, &(*tree)->var, 0, 0);
	new_str = create_filtered_string(old_str, final_len, 0, 0);
	if (!new_str)
		return ;
	free((*tree)->command_arr[k]);
	(*tree)->command_arr[k] = new_str;
}

int	expandableornot(char *str)
{
	int		i;
	int		in_quotes;
	char	quotetype;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quotetype = str[i];
		}
		else if (in_quotes && str[i] == quotetype)
			in_quotes = 0;
		if (str[i] == '$')
			break ;
		i++;
	}
	if (quotetype == '"')
		return (1);
	return (0);
}

void	process_command_array(t_tree **tree)
{
	int	k;

	k = 0;
	while ((*tree)->command_arr[k])
	{
		// if (variable_search_instr((*tree)->command_arr[k]
		// 	) && !expandableornot((*tree)->command_arr[k]))
		// {
		// 	k++;
		// 	continue ;
		// }
		process_command_string(tree, k);
		k++;
	}
}
