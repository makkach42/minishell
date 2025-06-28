/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:08:07 by makkach           #+#    #+#             */
/*   Updated: 2025/06/28 15:22:34 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	reset_vars(t_tree **tree, t_env **env, t_hdoc_data *h_data)
{
	t_list	*head;
	int		i;

	if ((*tree) && (*tree)->left)
		reset_vars(&(*tree)->left, env, h_data);
	if ((*tree) && (*tree)->right)
		reset_vars(&(*tree)->right, env, h_data);
	if ((*tree) && (*tree)->command_arr)
	{
		i = 0;
		head = NULL;
		reset_var_expand_var(tree, h_data);
		while ((*tree)->command_arr[i])
		{
			replace_whites_spaces((*tree)->command_arr[i]);
			i++;
		}
		reset_var_cmd_split(tree);
		if (has_wild_cards_comarr(tree) == 1)
			handle_wildcards_in_cmdarr(tree);
		if (has_wild_cards_fdlst(tree) == 1)
			handle_wildcards_in_fdlst(tree);
		reset_var_remove_quotes(tree);
	}
}
