/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_four.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:21:25 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 15:21:59 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
