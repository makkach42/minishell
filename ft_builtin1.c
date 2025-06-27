/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:26:07 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/27 11:33:54 by aakroud          ###   ########.fr       */
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
	if (str[i] == '\0')
		return (1);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}
