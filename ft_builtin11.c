/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin11.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:12:00 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/29 10:09:03 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_expo_error2(int *status, char *s)
{
	write(2, "minishell: export: not an identifier: ", 39);
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	*status = 1;
}

void	ft_p(char *str, int i, int lent_first, char **p)
{
	p[0] = ft_strncpy(p[0], str, lent_first);
	p[1] = ft_strcpy(p[1], &str[i]);
}

char	**ft_equal_str_helper(char **p, char *str, int i, int *lent_first)
{
	p[0] = ft_substr(str, 0, i);
	if (p[0] == NULL)
		return (free(p), NULL);
	*lent_first = i;
	i++;
	p[1] = ft_substr(str, i, ft_strlen(&str[i]));
	if (p[1] == NULL)
		return (free(p[0]), free(p), NULL);
	return (p);
}

char	**ft_equal_str(char *str)
{
	int		i;
	char	**p;
	int		lent_first;

	i = 0;
	lent_first = 0;
	p = malloc(sizeof(char *) * 2);
	if (p == NULL)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '=')
		{
			p = ft_equal_str_helper(p, str, i, &lent_first);
			if (!p)
				return (NULL);
			break ;
		}
		i++;
	}
	return (ft_p(str, i, lent_first, p), p);
}

char	*ft_str_back(char *s)
{
	int		lent;
	int		flag;
	int		i;
	char	*new;

	flag = 0;
	i = 0;
	if (!s)
		return (NULL);
	lent = ft_strlen(s);
	while (lent > 0)
	{
		if (s[lent] == '/')
			break ;
		lent--;
	}
	new = ft_substr(s, 0, lent);
	if (!new)
		return (NULL);
	return (new);
}
