/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:17:14 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/29 10:41:47 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_name_helper(char *name, int i)
{
	char	*tmp;
	char	*add;

	add = NULL;
	tmp = NULL;
	tmp = name;
	add = ft_itoa(i);
	if (!add)
		return (free(name), NULL);
	name = ft_strjoin(name, add);
	if (!name)
		return (NULL);
	free (tmp);
	free (add);
	return (name);
}

char	*ft_name_check(char *name)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (name == NULL)
		return (NULL);
	tmp = name;
	name = ft_strdup("e");
	if (!name)
		return (NULL);
	free (tmp);
	while (access(name, F_OK) == 0)
	{
		name = ft_name_helper(name, i);
		i++;
	}
	return (name);
}

void	ft_hdoc_expand_inits(int *i, int *in_quotes,
	int *flag, char *quote_type)
{
	*i = 0;
	*in_quotes = 0;
	*flag = 0;
	*quote_type = 0;
}

int	ft_space_count(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == 32 || str[i] == 9)
			count++;
		i++;
	}
	return (count);
}

void	ft_hdoc(char *limiter, t_list_fd *tmp, t_env **env, int status)
{
	char	*str;
	char	*temp;

	str = readline("> ");
	while (str != NULL && ft_strcmp(limiter, str) != 0)
	{
		if (!tmp->in_quotes)
			ft_hdoc_expand(&str, env, status);
		write(tmp->fd, str, ft_strlen(str));
		write(tmp->fd, "\n", 1);
		temp = str;
		str = readline("> ");
		free(temp);
	}
	ft_hdoc_free(&str, &limiter, tmp->fd);
}
