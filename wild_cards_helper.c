/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:05:52 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 15:42:21 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	count_pattern_matches(const char *pattern, char *dir_path)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	int				match_count;

// 	match_count = 0;
// 	dir = opendir(dir_path);
// 	if (dir == NULL)
// 		return (0);
// 	entry = readdir(dir);
// 	while (entry != NULL)
// 	{
// 		if (entry->d_name[0] == '.' && pattern[0] != '.')
// 		{
// 			entry = readdir(dir);
// 			continue ;
// 		}
// 		if (match_pattern(pattern, entry->d_name))
// 			match_count++;
// 		entry = readdir(dir);
// 	}
// 	closedir(dir);
// 	return (match_count);
// }

// char	*str_duplicate(const char *s)
// {
// 	size_t		len;
// 	const char	*tmp;
// 	char		*dup;
// 	char		*ptr;

// 	len = 0;
// 	tmp = s;
// 	while (*tmp++)
// 		len++;
// 	dup = malloc(len + 1);
// 	if (!dup)
// 		return (NULL);
// 	ptr = dup;
// 	while (*s)
// 		*ptr++ = *s++;
// 	*ptr = '\0';
// 	return (dup);
// }

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

void	if_expanded_matches(char ***new_cmd_arr,
		char **expanded_matches, int match_count, int *j)
{
	int	k;

	k = 0;
	while (k < match_count)
		(*new_cmd_arr)[(*j)++] = expanded_matches[k++];
	free(expanded_matches);
}

void	wild_cards_handle_cmdarr(char ***cmd_arr, char *dir_path)
{
	int		i;
	int		j;
	int		original_size;
	int		new_size;
	char	**expanded_matches;
	int		match_count;
	char	**new_cmd_arr;

	if (!cmd_arr || !*cmd_arr || !dir_path)
		return ;
	original_size = 0;
	while ((*cmd_arr)[original_size])
		original_size++;
	new_size = new_size_calcs(original_size, cmd_arr, dir_path, &match_count);
	new_cmd_arr = malloc(sizeof(char *) * (new_size + 1));
	if (!new_cmd_arr)
		return ;
	i = 0;
	j = 0;
	while (i < original_size)
	{
		if (if_has_wildcards((*cmd_arr)[i]))
		{
			expanded_matches = get_matches((
						*cmd_arr)[i], dir_path, &match_count);
			if (expanded_matches && match_count > 0)
				if_expanded_matches(&new_cmd_arr,
					expanded_matches, match_count, &j);
			else
			{
				new_cmd_arr[j++] = ft_strdup((*cmd_arr)[i]);
				if (expanded_matches)
					free(expanded_matches);
			}
		}
		else
			new_cmd_arr[j++] = ft_strdup((*cmd_arr)[i]);
		i++;
	}
	new_cmd_arr[j] = NULL;
	i = 0;
	while (i < original_size)
		free((*cmd_arr)[i++]);
	free(*cmd_arr);
	*cmd_arr = new_cmd_arr;
}
