/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:16:23 by makkach           #+#    #+#             */
/*   Updated: 2025/06/21 17:54:08 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	total_len_calculation(int *i, int match_count,
// 		char **matches, int *total_len)
// {
// 	*i = 0;
// 	*total_len = 0;
// 	while (*i < match_count)
// 	{
// 		*total_len += ft_strlen(matches[*i]);
// 		if (*i < match_count - 1)
// 			(*total_len)++;
// 		(*i)++;
// 	}
// }

// char	*join_matches(char **matches, int match_count)
// {
// 	int		i;
// 	int		total_len;
// 	char	*result;
// 	char	*temp;

// 	if (match_count <= 0)
// 		return (NULL);
// 	total_len_calculation(&i, match_count, matches, &total_len);
// 	result = malloc(total_len + 1);
// 	if (!result)
// 		return (NULL);
// 	temp = result;
// 	i = 0;
// 	ft_strcpy(temp, matches[i]);
// 	temp += ft_strlen(matches[i]);
// 	i++;
// 	while (i < match_count)
// 	{
// 		*temp++ = ' ';
// 		ft_strcpy(temp, matches[i]);
// 		temp += ft_strlen(matches[i]);
// 		i++;
// 	}
// 	*temp = '\0';
// 	return (result);
// }

// int	match_pattern(const char *pattern, const char *string)
// {
// 	int		i;
// 	int		j;
// 	int		star_idx;
// 	int		str_idx;
// 	int		in_quotes;
// 	int		quote_idx;
// 	char	quote_type;

// 	in_quotes = 0;
// 	quote_idx = 0;
// 	pattern_match_inits(&i, &j, &star_idx, &str_idx);
// 	printf("*********************%s\n", string);
// 	while (string[j])
// 	{
// 		if ((!in_quotes && (pattern[i] == '"' || pattern[i] == '\'')) || ((pattern[i] == '"' || pattern[i] == '\'') && i == quote_idx))
// 		{
// 			in_quotes = 1;
// 			quote_type = pattern[i];
// 			quote_idx = i;
// 			i++;
// 		}
// 		else if (in_quotes && pattern[i] == quote_type)
// 		{
// 			if (i != quote_idx)
// 				in_quotes = 0;
// 			i++;
// 		}
// 		if (!in_quotes && pattern[i] == quote_type && i == quote_idx)
// 			i++;
// 		if (pattern[i] == '*' && !in_quotes)
// 		{
// 			if (skip_stars(&i, pattern) == 1)
// 				return (1);
// 			update_vars(&star_idx, &i, &str_idx, &j, in_quotes);
// 		}
// 		else if (pattern[i] == string[j])
// 		{
// 			printf("-------------%c\n", pattern[i]);
// 			printf("+++++++++++++%c\n\n", string[j]);
// 			update_vars_two(&i, &j);
// 			if (in_quotes && pattern[i] == quote_type)
// 			{
// 				if (i != quote_idx)
// 					in_quotes = 0;
// 				i++;
// 			}
// 		}
// 		else if (star_idx >= 0)
// 		{
// 			back_track(&i, &j, &star_idx, &str_idx);
// 		}
// 		else
// 			return (0);
// 	}
// 	while (pattern[i] == '*')
// 		i++;
// 	return (!pattern[i]);
// }

int	match_pattern(char *pattern, char *string)
{
	int			i;
	int			j;
	int			k;
	int			star_idx;
	int			str_idx;
	int			in_quotes;
	int			quote_idx;
	char		quote_type;
	char		*tmp;

	in_quotes = 0;
	quote_idx = 0;
	pattern_match_inits(&i, &j, &star_idx, &str_idx);
	while (string[j])
	{
		if (!in_quotes && (pattern[i] == '"' || pattern[i] == '\''))
		{
			in_quotes = 1;
			quote_type = pattern[i];
			quote_idx = i;
			i++;
		}
		else if (in_quotes && pattern[i] == quote_type && i != quote_idx)
		{
			in_quotes = 0;
			i++;
		}
		if (pattern[i] == '*' && !in_quotes)
		{
			if (skip_stars(&i, pattern) == 1)
				return (1);
			update_vars(&star_idx, &i, &str_idx, &j, in_quotes);
		}
		else if (pattern[i] == string[j])
		{
			update_vars_two(&i, &j);
			if (in_quotes && pattern[i] == quote_type && i != quote_idx)
			{
				in_quotes = 0;
				i++;
			}
		}
		else if (star_idx >= 0)
		{
			back_track(&i, &j, &star_idx, &str_idx);
			in_quotes = 0;
		}
		else
			return (0);
	}
	while (pattern[i] == '*')
		i++;
	return (!pattern[i]);
}

// void	sort_matches(char **arr, int count)
// {
// 	int		i;
// 	int		j;
// 	char	*key;

// 	i = 1;
// 	while (i < count)
// 	{
// 		key = arr[i];
// 		j = i - 1;
// 		while (j >= 0 && ft_strcmp(arr[j], key) > 0)
// 		{
// 			arr[j + 1] = arr[j];
// 			j--;
// 		}
// 		arr[j + 1] = key;
// 		i++;
// 	}
// }

int	if_has_wildcards(char *str)
{
	int	i;
	int	in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		if (!in_quotes && str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}
// int	if_has_wildcards(char *str)
// {
// 	int	i;
// 	int	in_quotes;

// 	i = 0;
// 	in_quotes = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '\"')
// 			in_quotes = !in_quotes;
// 		if (str[i] == '*' && !in_quotes)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }
