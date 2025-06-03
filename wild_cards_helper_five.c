/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_five.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:07:47 by makkach           #+#    #+#             */
/*   Updated: 2025/06/02 11:43:28 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	new_size_calcs(int original_size, char ***cmd_arr,
	char *dir_path, int *match_count)
{
	int	i;
	int	new_size;

	i = 0;
	new_size = 0;
	while (i < original_size)
	{
		if (if_has_wildcards((*cmd_arr)[i]))
		{
			(*match_count) = count_pattern_matches((*cmd_arr)[i], dir_path);
			if ((*match_count) > 0)
				new_size += (*match_count);
			else
				new_size += 1;
		}
		else
			new_size++;
		i++;
	}
	return (new_size);
}

static void	if_expanded_matches(char ***new_cmd_arr,
		char **expanded_matches, int match_count, int *j)
{
	int	k;

	k = 0;
	while (k < match_count)
		(*new_cmd_arr)[(*j)++] = expanded_matches[k++];
	free(expanded_matches);
}

int	count_cmd_arr_size(char **cmd_arr)
{
	int	size;

	size = 0;
	if (!cmd_arr)
		return (0);
	while (cmd_arr[size])
		size++;
	return (size);
}

char	**init_new_cmd_arr(int new_size)
{
	char	**new_cmd_arr;

	new_cmd_arr = malloc(sizeof(char *) * (new_size + 1));
	if (!new_cmd_arr)
		return (NULL);
	return (new_cmd_arr);
}

void	process_wildcard_element(char ***new_cmd_arr, char *element,
		char *dir_path, int *j)
{
	char	**expanded_matches;
	int		match_count;

	expanded_matches = get_matches(element, dir_path, &match_count);
	if (expanded_matches && match_count > 0)
		if_expanded_matches(new_cmd_arr, expanded_matches, match_count, j);
	else
	{
		(*new_cmd_arr)[(*j)++] = ft_strdup(element);
		if (expanded_matches)
			free(expanded_matches);
	}
}
