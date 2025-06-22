/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:25:20 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/22 17:40:22 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_cd_helper(t_env *h)
{
	t_env *home;

	home = ft_check(h, "HOME");
	if (home == NULL || home->value == NULL)
	{
		ft_putstr_fd(2, "minishell: cd: HOME not set\n");
		return (1);
	}
	if (chdir(home->value) == -1)
	{
		ft_putstr_fd(2, "minishell: cd: ");
		perror("");
		return (1);
	}
	return (0);
}

int	ft_chdir_helper(char **p, char *past)
{
	free (past);
	ft_free_array(p);
	perror("");
	return (1);
}

int	ft_chdir_fail(char **s, char *past)
{
	char	**p;
	int		i;

	i = 0;
	p = ft_split(s[1], '/');
	if (!p)
	{
		free (past);
		return (1);
	}
	while (p[i])
	{
		if (!ft_strcmp(p[i], ".."))
		{
			if (chdir(past) == -1)
				return (ft_chdir_helper(p, past));
			free (past);
			ft_free_array(p);
			return (0);
		}
		i++;
	}
	return (ft_chdir_helper(p, past));
}

int	ft_cd_fail(t_env *n, char **s, char *past)
{
	int		lent;
	char	*temp;

	lent = 0;
	temp = NULL;
	perror("");
	lent = ft_strlen(n->value);
	if (n->value[lent - 1] != '/')
	{
		temp = s[1];
		s[1] = ft_strjoin("/", s[1]);
		if (!s[1])
			return (free(past), 1);
		free (temp);
	}
	temp = n->value;
	n->value = ft_strjoin(n->value, s[1]);
	if (!n->value)
		return (free(past), free(temp), 1);
	return (free(past), free(temp), 0);
}

int	ft_cd(char **s, t_env *h)
{
	char	*t;
	t_env	*n;
	char	*past;

	n = ft_check(h, "1PWD");
	past = ft_str_back(n->value);
	if (s[1] == NULL)
		return (ft_cd_helper(h));
	else
	{
		if (chdir(s[1]) == -1)
			return (ft_chdir_fail(s, past));
		t = getcwd(NULL, 0);
		if (t)
			n->value = t;
		else
			return (ft_cd_fail(n, s, past));
	}
	return (0);
}

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

	tmp = NULL;
	if ((i >= 2 && flag) || (i >= 3 && !flag && ft_nline_check(s[i])))
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

int	ft_echo_helper1(char **buff, char **s, int i, int flag)
{
	char *tmp;

	tmp = NULL;
	if ((i >= 2 && flag) || (i >= 3 && !flag && ft_nline_check(s[i])))
	{
		dprintf(2, "entered in the wrong plcae\n");
		tmp = *buff;
	}
	*buff = ft_strjoin(*buff, NULL);
	if (*buff == NULL)
		return (1);
	free (tmp);
	return (0);
}

int	ft_echo_main(char **s, int i, char **buff, int flag)
{
	if (s[i + 1])
	{
		dprintf(2, "entered in wrong\n");
		if (ft_echo_helper(buff, s, i, flag))
			return (1);
	}
	else
	{
		if (ft_echo_helper1(buff, s, i, flag))
			return (1);
	}
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
	if (s[i])
		buff = s[i];
	while (s[i] != NULL)
	{
		// if (s[i + 1])
		// {
		// 	if (ft_echo_helper(&buff, s, i, flag))
		// 		return (1);
		// }
		// else
		// {
		// 	if (ft_echo_helper1(&buff, s, 1, flag))
		// 		return (1);
		// }
		if (ft_echo_main(s, i, &buff, flag))
			return (1);
		i++;
	}
	dprintf(2, "this is the allocated %p\n", buff);
	if (flag == 1)
		if (ft_echo_helper2(buff))
			return (1);
	ft_putstr_fd(1, buff);
	dprintf(2, "final buff %p\n", buff);
	free (buff);
	return (0);
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
