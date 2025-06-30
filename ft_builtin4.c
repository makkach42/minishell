/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:27:00 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/30 17:10:27 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_cd(char **s, t_env *h)
{
	char		*t;
	t_env		*n;
	char		*past;
	static char	*tmp;

	n = ft_check(h, "1PWD");
	past = ft_str_back(n->value);
	if (s[1] == NULL)
		return (ft_cd_helper(h));
	else
	{
		if (chdir(s[1]) == -1)
		{
			dprintf(2, "nooooo\n");
			return (ft_chdir_fail(s, past));
		}
		t = getcwd(NULL, 0);
		if (t)
		{
			free (tmp);
			free (past);
			n->value = t;
			tmp = n->value;
		}
		else
		{
			dprintf(2, "entered here\n");
			return (ft_cd_fail(n, s, past));
		}
	}
	return (0);
}

void	ft_f_node(t_env *node)
{
	free (node->key);
	free (node->value);
	free (node);
	node = NULL;
}
