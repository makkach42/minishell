/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:13:43 by makkach           #+#    #+#             */
/*   Updated: 2025/06/21 17:53:32 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_wild_cards_comarr(t_tree **tree)
{
	int	i;
	int	j;

	i = 0;
	if ((*tree) && (*tree)->command_arr)
	{
		j = 0;
		while ((*tree)->command_arr[j])
		{
			if (if_has_wildcards((*tree)->command_arr[j]))
				i = 1;
			j++;
		}
	}
	if ((*tree) && (*tree)->left)
		i = has_wild_cards_comarr(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		i = has_wild_cards_comarr(&(*tree)->right);
	return (i);
}

int	init_matches(char ***matches, DIR **dir,
			char *dir_path, int *capacity)
{
	*capacity = 10;
	*dir = opendir(dir_path);
	if (*dir == NULL)
		return (0);
	*matches = malloc(sizeof(char *) * *capacity);
	if (!*matches)
	{
		closedir(*dir);
		return (0);
	}
	return (1);
}

static void	export_case_wild_card(t_tree **tree)
{
	char	*dir_name;
	int		j;

	j = 1;
	while ((*tree)->command_arr[j])
	{
		if (if_has_wildcards((*tree)->command_arr[j]))
		{
			dir_name = getcwd(NULL, 0);
			if (!dir_name)
				return ;
			wild_cards_handle_cmdarr(&(*tree)->command_arr, dir_name);
			free(dir_name);
			break ;
		}
		j++;
	}
}

static void	normale_wild_card_case(t_tree **tree)
{
	int		j;
	char	*dir_name;

	j = 0;
	while ((*tree)->command_arr[j])
	{
		if (if_has_wildcards((*tree)->command_arr[j]))
		{
			dir_name = getcwd(NULL, 0);
			if (!dir_name)
				return ;
			wild_cards_handle_cmdarr(&(*tree)->command_arr, dir_name);
			free(dir_name);
			break ;
		}
		j++;
	}
}

void	handle_wildcards_in_cmdarr(t_tree **tree)
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->command_arr && (*tree)->command_arr[0
		] && ft_strcmp((*tree
			)->command_arr[0], "export") == 0)
		export_case_wild_card(tree);
	else if ((*tree)->command_arr)
		normale_wild_card_case(tree);
	if ((*tree)->left)
		handle_wildcards_in_cmdarr(&(*tree)->left);
	if ((*tree)->right)
		handle_wildcards_in_cmdarr(&(*tree)->right);
}
