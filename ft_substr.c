/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:40:37 by makkach           #+#    #+#             */
/*   Updated: 2025/04/15 16:06:40 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
