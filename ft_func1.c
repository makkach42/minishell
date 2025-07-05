/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:22:40 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/26 11:22:49 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_lstnew(void *key, void *value)
{
	t_env	*newnode;

	newnode = (t_env *)malloc(sizeof(t_env));
	if (newnode == NULL)
		return (NULL);
	newnode -> key = ft_strdup((char *)key);
	newnode -> value = ft_strdup((char *)value);
	newnode -> next = NULL;
	return (newnode);
}

char	*ft_strmcpy(char *src)
{
	char	*dest;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dest = malloc(ft_strlen(src) + 2);
	if (dest == NULL)
		return (NULL);
	dest[j] = '/';
	j++;
	while (src[i])
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

int	ft_is_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != 9 && s[i] != 32)
			return (1);
		i++;
	}
	return (0);
}

int	ft_file_check(char *str)
{
	int	fd;

	fd = open(str, O_RDONLY);
	return (fd);
}

int	ft_file_create(char *str, int n)
{
	int	fd;

	if (n == 1)
		fd = open(str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (n == 2)
		fd = open(str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	return (fd);
}
