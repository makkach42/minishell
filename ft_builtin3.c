/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:25:28 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/22 18:35:17 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_nline_check(char *str)
{
	int	i;

	if (str == NULL)
		return (1);
	if (str[0] == '-' && str[1] == 'n')
	{
		i = 1;
		while (str[i] != '\0' && str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (0);
	}
	return (1);
}

int	ft_echo_helper(char **buff, char **s, int i, int flag)
{
	char	*tmp;

	tmp = *buff;
	if (!*buff)
	{
		*buff = ft_strjoin(*buff, s[i]);
		if (*buff == NULL)
			return (1);
		free (tmp);
	}
	if (s[i + 1])
	{
		tmp = *buff;
		*buff = ft_strjoin(*buff, " ");
		if (*buff == NULL)
			return (1);
		free (tmp);
		tmp = *buff;
		*buff = ft_strjoin(*buff, s[i + 1]);
		if (*buff == NULL)
			return (1);
		free (tmp);
	}
	return (0);
}

int	ft_echo_helper2(char **buff)
{
	char	*tmp;

	tmp = *buff;
	*buff = ft_strjoin(*buff, "\n");
	if (*buff == NULL)
		return (1);
	free (tmp);
	return (0);
}

int	ft_echo(char **s)
{
	int		flag;
	int		i;
	char	*buff;

	i = 1;
	flag = 1;
	buff = NULL;
	if (!ft_nline_check(s[i]))
	{
		while (ft_nline_check(s[i]) == 0)
			i++;
		flag = 0;
	}
	while (s[i] != NULL)
	{
		if (ft_echo_helper(&buff, s, i, flag))
			return (1);
		i++;
	}
	if (flag == 1)
	{
		if (ft_echo_helper2(&buff))
			return (1);
	}
	return (ft_putstr_fd(1, buff), free(buff), 0);
}

int	ft_check_act(t_env *h)
{
	while (h)
	{
		if (h->active == 0)
			return (0);
		h = h->next;
	}
	return (1);
}
