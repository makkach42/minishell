/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:05:14 by makkach           #+#    #+#             */
/*   Updated: 2025/06/26 11:39:27 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cmd_helper(t_cmd *com, int i, char *env, char *s)
{
	if (i == 0)
	{
		com->p = ft_split(env, ':');
		if (com->p == NULL)
			return ;
		com->t = ft_split(s, 32);
		if (com->t == NULL || com->t[0] == NULL)
			return ;
		com->str = ft_strmcpy(com->t[0]);
	}
	if (i == 1)
	{
		ft_free_array(com->t);
		ft_free_array(com->p);
		free (com->str);
	}
}

void	ft_cmd_check_error(char *s)
{
	ft_putstr_fd(2, "minishell: ");
	ft_putstr_fd(2, s);
	ft_putstr_fd(2, ": is a directory\n");
	exit (126);
}

void	ft_access_error(char *s)
{
	ft_putstr_fd(2, s);
	ft_putstr_fd(2, ": ");
	perror("");
	if (errno == 2)
		exit (127);
	else if (errno == 13 || errno == 20)
		exit (126);
	exit (1);
}

char	*ft_cmd_check_helper(char *s)
{
	int	fd;

	fd = -1;
	fd = open(s, O_DIRECTORY);
	if (fd != -1)
		ft_cmd_check_error(s);
	else if (access(s, X_OK) == 0)
		return (ft_strdup(s));
	else if (access(s, X_OK) == -1)
		ft_access_error(s);
	return (NULL);
}

char	*ft_cmd_check(char *env, char *s)
{
	t_cmd	com;

	com.i = 0;
	if (s[0] == '\0' || ft_is_spaces(s) == 0)
		return (NULL);
	if (ft_strchr(s, '/') != NULL || env == NULL || env[0] == '\0')
		return (ft_cmd_check_helper(s));
	ft_cmd_helper(&com, 0, env, s);
	while (com.p[com.i] != NULL)
	{
		com.cmd = ft_strjoin(com.p[com.i], com.str);
		if (access(com.cmd, X_OK) == 0)
			return (ft_cmd_helper(&com, 1, env, s), com.cmd);
		free (com.cmd);
		com.i++;
	}
	return (ft_cmd_helper(&com, 1, env, s), NULL);
}
