/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:57:35 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/29 19:16:02 by makkach          ###   ########.fr       */
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

void	ft_exit_m(int m, t_env **h)
{
	if (m < 0)
	{
		free_env(h);
		exit (m + 256);
	}
	else
	{
		free_env(h);
		exit (m);
	}
}

void	ft_exit_empty(int status, int stat)
{
	if (stat != -1)
		exit (stat);
	exit (status);
}

char	*ft_skip_space(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != 32 && str[i] != 9)
			return (&str[i]);
		i++;
	}
	if (str[i] == '\0')
		return (NULL);
	return (str);
}
