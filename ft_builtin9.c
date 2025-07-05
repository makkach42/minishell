/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin9.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:42:37 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/03 18:44:27 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_error(char **v, int *status, char *s)
{
	ft_expo_error2(status, s);
	ft_free_v(v, NULL);
}

void	ft_v_expo(char *s, char ***v)
{
	if (ft_equal_count(s) > 1)
		*v = ft_equal_str(s);
	else
		*v = ft_split(s, '=');
}

int	ft_expo_equal(char *s, t_env *h, int act, int *status)
{
	char	**v;
	t_env	*f;
	t_env	*new;

	f = NULL;
	new = NULL;
	v = NULL;
	ft_v_expo(s, &v);
	f = ft_check(h, v[0]);
	if (f != NULL && f->h == 0)
		ft_expo_equal_helper(v, act, f);
	else
	{
		if (ft_parse(v[0]) == 1)
			ft_free_error(v, status, s);
		else
		{
			if (ft_act_helper(act, v, &new))
				return (1);
			ft_lstadd_back(&h, new);
		}
	}
	return (0);
}

int	ft_equal(char *s)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (s[i])
	{
		if (s[i] == '=')
			flag = 1;
		if (s[i] == '+' && flag == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_pwd(t_env *h)
{
	char	*path;
	char	*temp;
	t_env	*n;
	char	*pwd_path;

	n = ft_check(h, "1PWD");
	pwd_path = ft_strdup(n->value);
	path = getcwd(NULL, 0);
	if (path)
	{
		temp = pwd_path;
		pwd_path = path;
		free (temp);
	}
	else if (!path)
		path = pwd_path;
	printf("%s\n", path);
	free (pwd_path);
	return (0);
}
