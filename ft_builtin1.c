/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:26:07 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/23 13:56:27 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_str_env(t_env *h)
{
	char	*buff;
	char	*tmp;

	buff = ft_strjoin(h->key, "=");
	if (!buff)
		return (NULL);
	tmp = buff;
	buff = ft_strjoin(buff, h->value);
	if (!buff)
	{
		free (tmp);
		return (NULL);
	}
	free (tmp);
	tmp = buff;
	buff = ft_strjoin(buff, "\n");
	if (!buff)
	{
		free (tmp);
		return (NULL);
	}
	free (tmp);
	return (buff);
}

int	ft_env2(t_env *h, char **buff)
{
	char	*tmp;
	char	*str;

	tmp = NULL;
	str = NULL;
	tmp = *buff;
	str = ft_str_env(h);
	*buff = ft_strjoin(*buff, str);
	if (!*buff)
		return (free(str), 1);
	free (str);
	free (tmp);
	return (0);
}

int	ft_env_main(t_env **h, t_env **start, char **buff)
{
	if (*h == *start)
	{
		*buff = ft_str_env(*h);
		if (!*buff)
			return (1);
	}
	else
	{
		if (ft_env2(*h, buff))
			return (1);
	}
	return (0);
}

int	ft_env(t_env *h)
{
	char	*buff;
	t_env	*start;

	start = h;
	buff = NULL;
	while (h != NULL)
	{
		if (h->active == 0)
		{
			if (ft_env_main(&h, &start, &buff))
				return (1);
		}
		h = h->next;
	}
	if (buff)
	{
		ft_putstr_fd(1, buff);
		free (buff);
	}
	return (0);
}

int	ft_check_string(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '\0' && (str[i] == '+' || str[i] == '-'))
		i++;
	if (str[i] == NULL)
		return (1);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

// int	ft_modulo(char *str)
// {
// 	long	n;
// 	int		m;

// 	n = ft_atoi(str);
// 	if (ft_strcmp(str, "9223372036854775807") != 0 && n == LONG_MAX)
// 	{
// 		write (2, "minishell: exit: ", 18);
// 		write(2, str, ft_strlen(str));
// 		write(2, ": numeric argument required", 28);
// 		exit (255);
// 	}
// 	m = n % 256;
// 	return (m);
// }

// void	ft_exit_error(int check, char **s)
// {
// 	if (check == 1)
// 	{
// 		write(2, "minishell: exit: ", 18);
// 		write(2, s[1], ft_strlen(s[1]));
// 		write(2, ": numeric argument required\n", 29);
// 		exit (255);
// 	}
// 	else if (check == 2)
// 		write(2, "minishell: exit: too many arguments\n", 37);
// }

// void	ft_exit_m(int m, t_env **h)
// {
// 	if (m < 0)
// 	{
// 		free_env(h);
// 		exit (m + 256);
// 	}
// 	else
// 	{
// 		free_env(h);
// 		exit (m);
// 	}
// }

// void	ft_exit(char **s, t_env **h, int status)
// {
// 	int	m;

// 	m = 0;
// 	if (isatty(0) && isatty(1))
// 		ft_putstr_fd(1, "exit\n");
// 	if (s[1] == NULL)
// 		exit (status);
// 	else
// 	{
// 		if (ft_check_string(s[1]) != 0)
// 			ft_exit_error(1, s);
// 		else if (s[2] != NULL)
// 			ft_exit_error(2, s);
// 		else
// 		{
// 			m = ft_modulo(s[1]);
// 			ft_exit_m(m, h);
// 		}
// 	}
// }
