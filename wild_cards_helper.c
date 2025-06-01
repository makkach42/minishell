/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:05:52 by makkach           #+#    #+#             */
/*   Updated: 2025/06/01 12:05:15 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_matching_entry(char ***matches, int *match_count,
		int *capacity, char *entry_name)
{
	if (*match_count >= *capacity)
	{
		if (!copy_and_resize_matches(matches, *match_count, *capacity * 2))
			return (0);
		*capacity *= 2;
	}
	(*matches)[*match_count] = str_duplicate(entry_name);
	if (!(*matches)[*match_count])
		return (0);
	(*match_count)++;
	return (1);
}

static char	**cleanup_on_error(char ***matches, int match_count, DIR *dir)
{
	int	i;

	i = 0;
	while (i < match_count)
		free((*matches)[i++]);
	free(*matches);
	closedir(dir);
	return (NULL);
}

static char	**process_directory(DIR *dir, const char *pattern,
		char ***matches, int *match_count)
{
	struct dirent	*entry;
	int				capacity;

	capacity = 10;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name))
		{
			if (!handle_matching_entry(matches, match_count, &capacity,
					entry->d_name))
				return (cleanup_on_error(matches, *match_count, dir));
		}
		entry = readdir(dir);
	}
	return (*matches);
}

char	**get_matches(const char *pattern, char *dir_path, int *match_count)
{
	DIR		*dir;
	char	**matches;
	int		capacity;

	*match_count = 0;
	if (!init_matches(&matches, &dir, dir_path, &capacity))
		return (NULL);
	matches = process_directory(dir, pattern, &matches, match_count);
	closedir(dir);
	if (*match_count > 0)
		sort_matches(matches, *match_count);
	else
	{
		free(matches);
		return (NULL);
	}
	return (matches);
}

void	wild_cards_handle_cmdarr(char ***cmd_arr, char *dir_path)
{
	t_idx	m;
	int		new_size;
	int		match_count;
	char	**new_cmd_arr;

	if (!cmd_arr || !*cmd_arr || !dir_path)
		return ;
	m.original_size = count_cmd_arr_size(*cmd_arr);
	new_size = new_size_calcs(m.original_size, cmd_arr, dir_path, &match_count);
	new_cmd_arr = init_new_cmd_arr(new_size);
	if (!new_cmd_arr)
		return ;
	m.i = -1;
	m.j = 0;
	while (++m.i < m.original_size)
	{
		if (if_has_wildcards((*cmd_arr)[m.i]))
			process_wildcard_element(&new_cmd_arr,
				(*cmd_arr)[m.i], dir_path, &m.j);
		else
			process_regular_element(&new_cmd_arr, (*cmd_arr)[m.i], &m.j);
	}
	new_cmd_arr[m.j] = NULL;
	free_original_cmd_arr(cmd_arr, m.original_size);
	*cmd_arr = new_cmd_arr;
}

void	process_wildcard_node_fd(t_list_fd *node, char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*pattern;
	char			**matches;
	int				match_count;
	int				capacity;
	int				i;
	int				old_capacity;
	char			*joined_result;

	pattern = node->name;
	match_count = 0;
	capacity = 10;
	dir = opendir(dir_path);
	if (dir == NULL)
		return ;
	matches = malloc(sizeof(char *) * capacity);
	if (!matches)
	{
		closedir(dir);
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name))
		{
			if (match_count >= capacity)
			{
				old_capacity = capacity;
				if (!copy_and_resize_matches(&matches,
						match_count, capacity * 2))
				{
					entry = readdir(dir);
					continue ;
				}
				capacity = old_capacity * 2;
			}
			matches[match_count] = str_duplicate(entry->d_name);
			if (!matches[match_count])
			{
				entry = readdir(dir);
				continue ;
			}
			match_count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (match_count > 0)
	{
		sort_matches(matches, match_count);
		joined_result = join_matches(matches, match_count);
		if (joined_result)
		{
			free(node->name);
			node->name = joined_result;
			i = 0;
			while (i < match_count)
			{
				free(matches[i]);
				i++;
			}
		}
	}
	free(matches);
}
