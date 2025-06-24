/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:25:20 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/24 17:04:30 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*ft_str_back(char *s)
// {
// 	int		lent;
// 	int		flag;
// 	int		i;
// 	char	*new;

// 	flag = 0;
// 	i = 0;
// 	if (!s)
// 		return (NULL);
// 	lent = ft_strlen(s);
// 	while (lent > 0)
// 	{
// 		if (s[lent] == '/')
// 			break ;
// 		lent--;
// 	}
// 	new = ft_substr(s, 0, lent);
// 	if (!new)
// 		return (NULL);
// 	return (new);
// }

int	ft_cd_helper(t_env *h)
{
	t_env	*home;

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

int	ft_cd_fail_helper(char **s, char *past)
{
	char	*temp;

	temp = NULL;
	temp = s[1];
	s[1] = ft_strjoin("/", s[1]);
	if (!s[1])
		return (free(past), 1);
	free (temp);
	return (0);
}

int	ft_cd_fail(t_env *n, char **s, char *past)
{
	int			lent;
	char		*temp;
	static char	*cd_path;

	lent = 0;
	temp = NULL;
	if (cd_path)
		free (cd_path);
	cd_path = ft_strdup(n->value);
	if (!cd_path)
		return (1);
	perror("");
	lent = ft_strlen(n->value);
	if (n->value[lent - 1] != '/')
	{
		if (ft_cd_fail_helper(s, past))
			return (1);
	}
	temp = cd_path;
	cd_path = ft_strjoin(cd_path, s[1]);
	if (!cd_path)
		return (free(temp), free(past), 1);
	n->value = cd_path;
	return (free(temp), free(past), 0);
}
