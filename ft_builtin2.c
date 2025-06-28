/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:57:35 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/28 15:55:56 by aakroud          ###   ########.fr       */
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
		write(2, ": numeric argument required", 28);
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
// void	ft_skip_space(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] != 32 && str[i] != 9)
// 			return (&str[i]);
// 		i++;
// 	}
	
// }

int	ft_exit(char **s, t_hdoc_data *h_data, int status)
{
	int	m;

	m = 0;
	if (isatty(0) && isatty(1))
		ft_putstr_fd(1, "exit\n");
	// ft_skip_space();
	if (s[1] == NULL)
		ft_exit_empty(status, h_data->stat);
	else
	{
		if (ft_check_string(s[1]) != 0)
			return (ft_exit_error(1, s));
		else if (s[2] != NULL)
			return (ft_exit_error(2, s));
		else
		{
			m = ft_modulo(s[1]);
			ft_exit_m(m, h_data->env);
		}
	}
	return (0);
}
