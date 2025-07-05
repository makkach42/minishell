/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:49:54 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/03 18:35:26 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_expo_equal_free(char **v, t_env *f, int check)
{
	char	*tmp;

	tmp = NULL;
	if (check == 1)
	{
		tmp = f->value;
		f->value = ft_strdup("");
		f->active = 0;
		f->h = 0;
		ft_free_v(v, tmp);
	}
	else if (check == 2)
	{
		tmp = f->value;
		f->value = ft_strdup(v[1]);
		f->active = 0;
		f->h = 0;
		ft_free_v(v, tmp);
	}
	else if (check == 3)
		ft_free_v(v, tmp);
}

int	ft_expo_act(char **v, t_env **new)
{
	if (v[1] == NULL)
	{
		*new = ft_lstnew(v[0], "");
		if (!*new)
			return (1);
		free (v[0]);
		free (v);
		(*new)->active = 0;
		(*new)->h = 0;
	}
	else
	{
		*new = ft_lstnew(v[0], v[1]);
		if (!*new)
			return (1);
		free (v[0]);
		free (v[1]);
		free (v);
		(*new)->active = 0;
		(*new)->h = 0;
	}
	return (0);
}

int	ft_expo_nonact(t_env **new, char **v)
{
	*new = ft_lstnew(v[0], NULL);
	if (!*new)
		return (1);
	free (v[0]);
	free (v);
	(*new)->active = 1;
	(*new)->h = 0;
	return (0);
}

void	ft_expo_equal_helper(char **v, int act, t_env *f)
{
	if (v[1] == NULL && act == 0)
		ft_expo_equal_free(v, f, 1);
	else if (v[1] != NULL)
		ft_expo_equal_free(v, f, 2);
	else if (!v[1] && act == 1)
		ft_expo_equal_free(v, f, 3);
}

int	ft_act_helper(int act, char **v, t_env **new)
{
	if (act == 0)
	{
		if (ft_expo_act(v, new))
			return (1);
	}
	else
	{
		if (ft_expo_nonact(new, v))
			return (1);
	}
	return (0);
}
