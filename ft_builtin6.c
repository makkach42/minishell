/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:10:12 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/25 10:44:14 by aakroud          ###   ########.fr       */
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
	ft_str_expo_buff(&buff,"\n");
	return (buff);
}

int	ft_exp_print(t_env *h)
{
	char	*str;
	int		flag;
	char	*buff;
	char	*tmp;

	str = NULL;
	buff = NULL;
	tmp = NULL;
	flag = 0;
	h = ft_sort_list(h);
	while (h != NULL)
	{
		if ((h)->h == 0)
		{
			if (!flag)
			{
				str = ft_str_export(h);
				if (!str)
					return (1);
				buff = str;
				flag = 1;
			}
			else
			{
				tmp = buff;
				str = ft_str_export(h);
				buff = ft_strjoin(buff, str);
				if (!buff)
					return (free (str), 1);
				free (str);
				free (tmp);
			}
		}
		h = (h)->next;
	}
	if (buff)
	{
		ft_putstr_fd(1, buff);
		free (buff);
	}
	return (0);
}

void	ft_expo_error(int *status, char *s)
{
	write(2, "minishell: export: ", 20);
	write(2, s, ft_strlen(s));
	write(2, ": not a valid identifier\n", 26);
	*status = 1;
}

void	ft_expo_error2(int *status, char *s)
{
	write(2, "minishell: export: not an identifier: ", 39);
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	*status = 1;
}

void	ft_free_v(char **v, char *tmp)
{
	free (v[1]);
	free (v[0]);
	free (v);
	free (tmp);
}

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
			ft_expo_error2(&status, s);
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
			if (ft_expo_plus(s, status, h, act));
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

