/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:39:32 by makkach           #+#    #+#             */
/*   Updated: 2024/10/31 11:46:02 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check(char c, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	getstart(char *str, int start, char *set)
{
	while (str[start] != '\0')
	{
		if (check(str[start], set) == 1)
			start++;
		else
			break ;
	}
	return (start);
}

static int	getend(char *str, int start, int end, char *set)
{
	while (str[end] != '\0')
		end++;
	end -= 1;
	while (end >= start)
	{
		if (check(str[end], set) == 1)
			end--;
		else
			break ;
	}
	return (end);
}

char	*ft_strtrim(char *s1, char *set)
{
	int		start;
	int		j;
	int		end;
	int		nlen;
	char	*new;

	if (!s1 || !set)
		return (0);
	start = 0;
	j = 0;
	start = getstart(s1, start, set);
	end = start;
	end = getend(s1, start, end, set);
	nlen = end - start + 1;
	new = malloc((nlen + 1) * sizeof(char));
	if (!new)
		return (0);
	while (j < nlen)
	{
		new[j] = s1[start + j];
		j++;
	}
	new[j] = '\0';
	return (new);
}

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
