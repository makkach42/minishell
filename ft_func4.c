/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:38:41 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/26 11:39:19 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	delimiter(char c, char del)
{
	return (c == del);
}

void	ft_free_array(char **p)
{
	int	i;

	i = 0;
	if (!p)
		return ;
	while (p[i] != NULL)
	{
		free (p[i]);
		i++;
	}
	free (p);
}

static int	ft_count_strings(char	const	*s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && delimiter(s[i], c) == 1)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != '\0' && delimiter(s[i], c) == 0)
			i++;
	}
	return (count);
}
