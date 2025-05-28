/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:16:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 14:22:49 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	total_len_calculation(int *i, int match_count,
		char **matches, int *total_len)
{
	*i = 0;
	*total_len = 0;
	while (*i < match_count)
	{
		*total_len += ft_strlen(matches[*i]);
		if (*i < match_count - 1)
			(*total_len)++;
		(*i)++;
	}
}

char	*join_matches(char **matches, int match_count)
{
	int		i;
	int		total_len;
	char	*result;
	char	*temp;

	if (match_count <= 0)
		return (NULL);
	total_len_calculation(&i, match_count, matches, &total_len);
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

void	pattern_match_inits(int *i, int *j, int *star_idx, int *str_idx)
{
	*i = 0;
	*j = 0;
	*star_idx = -1;
	*str_idx = -1;
}

int	skip_stars(int *i, const char *pattern)
{
	while (pattern[(*i) + 1] == '*')
		(*i)++;
	if (!pattern[(*i) + 1])
		return (1);
	return (0);
}

void	update_vars(int *star_idx, int *i, int *str_idx, int *j)
{
	*star_idx = (*i)++;
	*str_idx = *j;
}

void	back_track(int *i, int *j, int *star_idx, int *str_idx)
{
	*i = (*star_idx) + 1;
	*j = ++(*str_idx);
}

void	update_vars_two(int *i, int *j)
{
	(*i)++;
	(*j)++;
}

int	match_pattern(const char *pattern, const char *string)
{
	int		i;
	int		j;
	int		star_idx;
	int		str_idx;

	pattern_match_inits(&i, &j, &star_idx, &str_idx);
	while (string[j])
	{
		if (pattern[i] == '*')
		{
			if (skip_stars(&i, pattern) == 1)
				return (1);
			update_vars(&star_idx, &i, &str_idx, &j);
		}
		else if (pattern[i] == string[j])
			update_vars_two(&i, &j);
		else if (star_idx >= 0)
			back_track(&i, &j, &star_idx, &str_idx);
		else
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
