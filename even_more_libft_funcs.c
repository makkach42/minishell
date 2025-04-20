/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   even_more_libft_funcs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:26:40 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 09:57:22 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*cases(char *s1, char *s2)
{
	int	len3;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len3 = ft_strlen(s1) + ft_strlen(s2);
	return (t_malloc((len3 + 1) * sizeof(char), 26, "ft_strjoin.c"));
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*s3;
	int		i;
	int		j;

	s3 = cases(s1, s2);
	if (!s3)
		return (0);
	i = 0;
	while (s1 && s1[i] != '\0')
	{
		s3[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j] != '\0')
	{
		s3[i + j] = s2[j];
		j++;
	}
	s3[i + j] = '\0';
	return (s3);
}

static char	*casereturns(char *s, size_t len, unsigned int start)
{
	char	*sub;

	if ((s[0] == '\0' && len == 0) || (start >= ft_strlen(s)))
	{
		sub = t_malloc(1, 21, "ft_substr.c");
		if (!sub)
			return (0);
		sub[0] = '\0';
		return (sub);
	}
	return (0);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*sub;

	i = 0;
	if (!s)
		return (0);
	if (s[0] == '\0' && len == 0)
		return (casereturns(s, len, start));
	if (start >= ft_strlen(s))
		return (casereturns(s, len, start));
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	sub = t_malloc((len + 1) * sizeof(char), 44, "ft_substr.c");
	if (!sub)
		return (0);
	while (i < len && s[start + i] != '\0')
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

