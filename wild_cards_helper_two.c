/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:16:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 15:15:19 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_matches(char **matches, int match_count)
{
	int		i;
	size_t	total_len;
	char	*result;
	char	*temp;

	if (match_count <= 0)
		return (NULL);
	total_len = 0;
	i = 0;
	while (i < match_count)
	{
		total_len += ft_strlen(matches[i]);
		if (i < match_count - 1)
			total_len++;
		i++;
	}
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	temp = result;
	i = 0;
	ft_strcpy(temp, matches[i]);
	temp += ft_strlen(matches[i]);
	i++;
	while (i < match_count)
	{
		*temp++ = ' ';
		ft_strcpy(temp, matches[i]);
		temp += ft_strlen(matches[i]);
		i++;
	}
	*temp = '\0';
	return (result);
}

int	match_pattern(const char *pattern, const char *string)
{
	int		i;
	int		j;
	int		star_idx;
	int		str_idx;

	i = 0;
	j = 0;
	star_idx = -1;
	str_idx = -1;
	while (string[j])
	{
		if (pattern[i] == '*')
		{
			while (pattern[i + 1] == '*')
				i++;
			if (!pattern[i + 1])
				return (1);
			star_idx = i++;
			str_idx = j;
			continue ;
		}
		if (pattern[i] == string[j])
		{
			i++;
			j++;
			continue ;
		}
		if (star_idx >= 0)
		{
			i = star_idx + 1;
			j = ++str_idx;
			continue ;
		}
		return (0);
	}
	while (pattern[i] == '*')
		i++;
	return (!pattern[i]);
}

void	sort_matches(char **arr, int count)
{
	int		i;
	int		j;
	char	*key;

	i = 1;
	while (i < count)
	{
		key = arr[i];
		j = i - 1;
		while (j >= 0 && ft_strcmp(arr[j], key) > 0)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
		i++;
	}
}

// int	copy_and_resize_matches(char ***matches, int match_count, int new_cap)
// {
// 	char	**new_matches;
// 	int		i;

// 	new_matches = malloc(sizeof(char *) * new_cap);
// 	if (!new_matches)
// 	{
// 		perror("malloc failed");
// 		return (0);
// 	}
// 	i = 0;
// 	while (i < match_count)
// 	{
// 		new_matches[i] = (*matches)[i];
// 		i++;
// 	}
// 	free(*matches);
// 	*matches = new_matches;
// 	return (1);
// }

int	copy_and_resize_matches(char ***matches, int match_count, int new_cap)
{
	char	**new_matches;
	int		i;

	new_matches = malloc(sizeof(char *) * new_cap);
	if (!new_matches)
	{
		return (0);
	}
	i = 0;
	while (i < match_count)
	{
		new_matches[i] = (*matches)[i];
		i++;
	}
	free(*matches);
	*matches = new_matches;
	return (1);
}

int	if_has_wildcards(char *str)
{
	int	i;
	int	in_quotes;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			in_quotes = !in_quotes;
		if (str[i] == '*' && !in_quotes)
			return (1);
		i++;
	}
	return (0);
}
