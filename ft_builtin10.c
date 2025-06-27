/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin10.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:08:37 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/27 11:32:31 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_expo_plus_free(t_env *f, char **v)
{
	char	*tmp;

	tmp = NULL;
	if (!v)
		return (1);
	tmp = f->value;
	f->value = ft_strjoin(f->value, v[1]);
	if (!f->value)
		return (1);
	f->active = 0;
	f->h = 0;
	ft_free_v(v, tmp);
	return (0);
}

int	ft_expo_plus(char *s, int *status, t_env *h, int act)
{
	t_env	*new;
	char	**v;
	t_env	*f;

	new = NULL;
	v = NULL;
	f = NULL;
	if (ft_equal_check(s) == 1)
		ft_expo_error2(status, s);
	else
	{
		ft_v_expo(s, &v);
		ft_remove_sign(v[0]);
		f = ft_check(h, v[0]);
		if (f != NULL && f->h == 0)
			return (ft_expo_plus_free(f, v), 1);
		else
		{
			if (ft_parse(v[0]) == 1)
				ft_free_error(v, status, s);
			else if (!ft_act_helper(act, v, &new))
				ft_lstadd_back(&h, new);
		}
	}
	return (0);
}

int	ft_expo_helper(char *s, t_tree *tree, int *status, t_env *h)
{
	int	act;

	act = 1;
	if ((check_empty(s) == 1 && tree->var == 0))
		ft_expo_error(status, s);
	else if (check_empty(s) != 1)
	{
		if (ft_equal_check(s) == 0)
			act = 0;
		if (s[0] == '=')
			ft_expo_error(status, s);
		else if (!ft_strchr(s, '+') || !ft_equal(s))
		{
			if (ft_expo_equal(s, h, act, status))
				return (1);
		}
		else
		{
			if (ft_expo_plus(s, status, h, act))
				return (1);
		}
	}
	return (0);
}

int	ft_export(char **s, t_env *h, t_tree *tree)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (s[i] != NULL && (check_empty(s[i]) == 1 && tree->var == 1))
		i++;
	if (s[i] == NULL)
		return (ft_exp_print(h));
	while (s[i] != NULL)
	{
		if (s[i] != NULL)
		{
			if (ft_expo_helper(s[i], tree, &status, h))
				return (1);
		}
		i++;
	}
	return (status);
}

void	ft_free_v(char **v, char *tmp)
{
	free (v[1]);
	free (v[0]);
	free (v);
	free (tmp);
}
