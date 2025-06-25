/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:10:12 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/25 13:28:10 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_str_expo_buff(char **buff, char *s)
{
	char	*tmp;

	tmp = NULL;
	tmp = *buff;
	*buff = ft_strjoin(*buff, s);
	if (!*buff)
		return ;
	free (tmp);
}

char	*ft_str_export(t_env *h)
{
	char	*buff;

	buff = ft_strjoin("declare -x ", NULL);
	if (!buff)
		return (NULL);
	ft_str_expo_buff(&buff, h->key);
	ft_str_expo_buff(&buff, "=");
	ft_str_expo_buff(&buff, h->value);
	ft_str_expo_buff(&buff, "\n");
	return (buff);
}

int	ft_exp_print_helper(int *flag, char **buff, t_env *h)
{
	char	*str;
	char	*tmp;

	str = NULL;
	tmp = NULL;
	if (!*flag)
	{
		str = ft_str_export(h);
		if (!str)
			return (1);
		*buff = str;
		*flag = 1;
	}
	else
	{
		tmp = *buff;
		str = ft_str_export(h);
		*buff = ft_strjoin(*buff, str);
		if (!*buff)
			return (free (str), 1);
		free (str);
		free (tmp);
	}
	return (0);
}

int	ft_exp_print(t_env *h)
{
	int		flag;
	char	*buff;

	buff = NULL;
	flag = 0;
	h = ft_sort_list(h);
	while (h != NULL)
	{
		if ((h)->h == 0)
		{
			if (ft_exp_print_helper(&flag, &buff, h))
				return (1);
		}
		h = (h)->next;
	}
	return (ft_putstr_fd(1, buff), free (buff), 0);
}

void	ft_expo_error(int *status, char *s)
{
	write(2, "minishell: export: ", 20);
	write(2, s, ft_strlen(s));
	write(2, ": not a valid identifier\n", 26);
	*status = 1;
}
