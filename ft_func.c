/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:20:17 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/26 11:21:48 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putstr_fd(int fd, char *s)
{
	int	i;

	i = 0;
	if (s == NULL || fd < 0)
		return ;
	write(fd, s, ft_strlen(s));
}

int	ft_lstsize(t_env *lst)
{
	int	count;

	count = 0;
	while (lst != NULL)
	{
		count++;
		lst = lst -> next;
	}
	return (count);
}

static	int	count_len(int n)
{
	int			count;
	long		p;

	p = n;
	count = 0;
	if (p == 0)
	{
		return (1);
	}
	if (p < 0)
	{
		p = -p;
		count++;
	}
	while (p > 0)
	{
		p /= 10;
		count++;
	}
	return (count);
}

static	char	*ft_change(long int y, char *s)
{
	int	i;

	i = count_len(y) - 1;
	if (y < 0)
	{
		y = -y;
		s[0] = '-';
	}
	while (y > 0)
	{
		s[i] = y % 10 + 48;
		y /= 10;
		i--;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	int			i;
	char		*str;
	long int	p;

	p = n;
	i = count_len(n);
	str = (char *)malloc(i + 1);
	if (str == NULL)
		return (NULL);
	str[i] = '\0';
	i -= 1;
	if (p == 0)
	{
		str[0] = '0';
		return (str);
	}
	str = ft_change(p, str);
	return (str);
}
