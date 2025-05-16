/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:07:49 by makkach           #+#    #+#             */
/*   Updated: 2025/05/16 09:24:04 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_words_beside(char *str)
{
	int	i;
	int	open_par;
	int	closed_par;

	i = 0;
	open_par = 0;
	closed_par = 0;
	if (str[i] != '(')
		return (0);
	while (str[i])
	{
		if (str[i] == '(')
			open_par++;
		else if (str[i] == ')')
			closed_par++;
		if (open_par == closed_par && str[i + 1])
			return (0);
		i++;
	}
	return (1);
}

static char	*makestrs(const char *s, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static int	mktwod(char **split, const char *s, char c, int count)
{
	int	start;
	int	i;
	int	j;

	i = 0;
	j = 0;
	start = 0;
	while (s[i] && j < count)
	{
		if (s[i] == c)
		{
			if (i > start)
			{
				split[j] = makestrs(s, start, i);
				if (!split[j++])
					return (0);
			}
			start = i + 1;
		}
		i++;
	}
	if (i > start && j < count)
		split[j++] = makestrs(s, start, i);
	split[j] = NULL;
	return (1);
}

static void	ft_free(char **split, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	**ft_split(char *s, char c)
{
	int		count;
	char	**split;

	if (!s)
		return (NULL);
	count = countwords(s, c);
	split = malloc((count + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	if (!mktwod(split, s, c, count))
	{
		ft_free(split, count);
		return (NULL);
	}
	return (split);
}
