/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin9.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:42:37 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/25 11:51:10 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_expo_equal(char *s, t_env *h, int act, int *status)
{
	char	**v;
	t_env	*f;
	char	*tmp;
	t_env	*new;

	v = NULL;
	f = NULL;
	tmp = NULL;
	new = NULL;
	if (ft_equal_count(s) > 1)
		v = ft_equal_str(s);
	else
		v = ft_split(s, '=');
	if (v == NULL)
		return (1);
	f = ft_check(h, v[0]);
	if (f != NULL && f->h == 0)
		ft_expo_equal_helper(v, act, f);
	else
	{
		if (ft_parse(v[0]) == 1)
		{
			ft_expo_error2(status, s);
			ft_free_v(v, NULL);
		}
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

int	ft_expo_plus_free(t_env *f, char **v)
{
	char	*tmp;

	tmp = NULL;
	tmp = f->value;
	f->value = ft_strjoin(f->value, v[1]);
	if (!f->value)
		return (1);
	f->active = 0;
	f->h = 0;
	ft_free_v(v, tmp);
}

int	ft_expo_plus(char *s, int *status, t_env *h, int act)
{
	t_env	*new;
	char	**v;
	t_env	*f;
	char	*tmp;
	
	new = NULL;
	v = NULL;
	f = NULL;
	tmp = NULL;
	if (ft_equal_check(s) == 1)
		ft_expo_error2(status, s);
	else
	{
		if (ft_equal_count(s) > 1)
			v = ft_equal_str(s);
		else
			v = ft_split(s, '=');
		if (v == NULL)
			return (1);
		ft_remove_sign(v[0]);
		f = ft_check(h, v[0]);
		if (f != NULL && f->h == 0)
		{
			if (ft_expo_plus_free(f, v))
				return (1);
		}
		else
		{
			if (ft_parse(v[0]) == 1)
			{
				ft_expo_error2(status, s);
				ft_free_v(v, NULL);
			}
			else
			{
				if (ft_act_helper(act, v, &new))
					return (1);
				ft_lstadd_back(&h, new);
			}
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

int    ft_export(char  **s, t_env *h, t_tree *tree)
{
	int     i;
	int     status;

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

void	ft_expo_error2(int *status, char *s)
{
	write(2, "minishell: export: not an identifier: ", 39);
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	*status = 1;
}

void	ft_p(char *str, int i, int lent_first, char **p)
{
	p[0] = ft_strncpy(p[0], str, lent_first);
	p[1] = ft_strcpy(p[1], &str[i]);
}

char	**ft_equal_str(char *str)
{
	int		i;
	char	**p;
	int		lent_first;

	i = 0;
	lent_first = 0;
	p = malloc(sizeof(char *) * 2);
	if (p == NULL)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '=')
		{
			p[0] = ft_substr(str, 0, i);
			if (p[0] == NULL)
				return (free(p), NULL);
			lent_first = i;
			i++;
			p[1] = ft_substr(str, i, ft_strlen(&str[i]));
			if (p[1] == NULL)
				return (free(p[0]), free(p), NULL);
			break ;
		}
		i++;
	}
	return (ft_p(str, i, lent_first, p), p);
}

char	*ft_str_back(char *s)
{
	int		lent;
	int		flag;
	int		i;
	char	*new;

	flag = 0;
	i = 0;
	if (!s)
		return (NULL);
	lent = ft_strlen(s);
	while (lent > 0)
	{
		if (s[lent] == '/')
			break ;
		lent--;
	}
	new = ft_substr(s, 0, lent);
	if (!new)
		return (NULL);
	return (new);
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

void	ft_f_node(t_env *node)
{
	free (node->key);
	free (node->value);
	free (node);
	node = NULL;
}
