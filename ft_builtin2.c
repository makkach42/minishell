/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:57:35 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/05 10:00:52 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_modulo(char *str)
{
	long	n;
	int		m;

	n = ft_atoi(str);
	if (ft_strcmp(str, "9223372036854775807") != 0 && n == LONG_MAX)
	{
		write (2, "minishell: exit: ", 18);
		write(2, str, ft_strlen(str));
		write(2, ": numeric argument required\n", 29);
		exit (255);
	}
	m = n % 256;
	return (m);
}

int	ft_exit_error(int check, char **s)
{
	if (check == 1)
	{
		write(2, "minishell: exit: ", 18);
		write(2, s[1], ft_strlen(s[1]));
		write(2, ": numeric argument required\n", 29);
		exit (255);
	}
	else if (check == 2)
		write(2, "minishell: exit: too many arguments\n", 37);
	return (1);
}

void	ft_exit_m(int m, t_env **h, char *tmp)
{
	if (m < 0)
	{
		free_env(h);
		free (tmp);
		exit (m + 256);
	}
	else
	{
		free_env(h);
		free (tmp);
		exit (m);
	}
}

int	ft_lent_char(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != 32 && str[i] != 9)
			count++;
		i++;
	}
	return (count);
}

char	*ft_skip_space(char *str)
{
	int		i;
	int		j;
	char	*dup;

	i = 0;
	j = 0;
	dup = NULL;
	if (!str)
		return (NULL);
	dup = malloc(ft_lent_char(str) + 1);
	if (!dup)
		return (NULL);
	while (str[i])
	{
		if (str[i] != 32 && str[i] != 9)
			dup[j++] = str[i];
		i++;
	}
	dup[j] = '\0';
	return (dup);
}
