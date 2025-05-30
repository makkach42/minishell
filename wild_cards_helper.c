/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:05:52 by makkach           #+#    #+#             */
/*   Updated: 2025/05/16 09:13:56 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	count_pattern_matches(const char *pattern, char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;
	int				match_count;

	match_count = 0;
	dir = opendir(dir_path);
	if (dir == NULL)
		return (0);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name))
			match_count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

char	*str_duplicate(const char *s)
{
	size_t		len;
	const char	*tmp;
	char		*dup;
	char		*ptr;

	len = 0;
	tmp = s;
	while (*tmp++)
		len++;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ptr = dup;
	while (*s)
		*ptr++ = *s++;
	*ptr = '\0';
	return (dup);
}

char	**get_matches(const char *pattern, char *dir_path, int *match_count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	char			**new_matches;
	int				capacity;
	int				new_capacity;
	int				i;

	*match_count = 0;
	capacity = 10;
	dir = opendir(dir_path);
	if (dir == NULL)
		return (NULL);
	matches = malloc(sizeof(char *) * capacity);
	if (!matches)
	{
		closedir(dir);
		return (NULL);
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
			if (*match_count >= capacity)
			{
				new_capacity = capacity * 2;
				new_matches = malloc(sizeof(char *) * new_capacity);
				if (!new_matches)
				{
					i = 0;
					while (i < *match_count)
					{
						free(matches[i]);
						i++;
					}
					free(matches);
					closedir(dir);
					return (NULL);
				}
				i = 0;
				while (i < *match_count)
				{
					new_matches[i] = matches[i];
					i++;
				}
				free(matches);
				matches = new_matches;
				capacity = new_capacity;
			}
			matches[*match_count] = str_duplicate(entry->d_name);
			if (matches[*match_count])
				(*match_count)++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (*match_count > 0)
		sort_matches(matches, *match_count);
	return (matches);
}

void	wild_cards_handle_cmdarr(char ***cmd_arr, char *dir_path)
{
	int		i;
	int		j;
	int		k;
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
	i = 0;
	new_size = 0;
	while (i < original_size)
	{
		if (if_has_wildcards((*cmd_arr)[i]))
		{
			match_count = count_pattern_matches((*cmd_arr)[i], dir_path);
			if (match_count > 0)
				new_size += match_count;
			else
				new_size += 1;
		}
		else
			new_size++;
		i++;
	}
	new_cmd_arr = malloc(sizeof(char *) * (new_size + 1));
	if (!new_cmd_arr)
		return ;
	i = 0;
	j = 0;
	while (i < original_size)
	{
		if (if_has_wildcards((*cmd_arr)[i]))
		{
			expanded_matches = get_matches((*cmd_arr)[i],
					dir_path, &match_count);
			if (expanded_matches && match_count > 0)
			{
				k = 0;
				while (k < match_count)
				{
					new_cmd_arr[j] = expanded_matches[k];
					j++;
					k++;
				}
				free(expanded_matches);
			}
			else
			{
				if (!*expanded_matches)
					free(expanded_matches);
				new_cmd_arr[j] = ft_strdup((*cmd_arr)[i]);
				j++;
			}
		}
		else
		{
			new_cmd_arr[j] = ft_strdup((*cmd_arr)[i]);
			j++;
		}
		i++;
	}
	new_cmd_arr[j] = NULL;
	i = 0;
	while (i < original_size)
	{
		free((*cmd_arr)[i]);
		i++;
	}
	free(*cmd_arr);
	*cmd_arr = new_cmd_arr;
}
