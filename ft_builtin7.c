/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:55:35 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/25 13:14:09 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_pwd(t_env *h)
{
	t_env	*n;
	char	*path;
	char	*temp;
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

int	ft_unset_error(char *s)
{
	write(2, "unset: ", 8);
	write(2, s, ft_strlen(s));
	write(2, ": invalid parameter name\n", 26);
	return (1);
}

int	ft_unset(t_env **h, char **s)
{
	t_env	*node;
	t_env	*start;
	int		flag;
	int		i;

	flag = 0;
	i = 1;
	start = NULL;
	if (s == NULL)
		return (1);
	while (s[i] != NULL)
	{
		if (ft_parse(s[i]) == 1)
			return (ft_unset_error(s));
		else
		{
			node = NULL;
			node = ft_check(*h, s[i]);
			if (node != NULL)
			{
				if (*h == node)
				{
					*h = (*h)->next;
					node->next = NULL;
					ft_f_node(node);
				}
				else
				{
					start = *h;
					while (start != NULL)
					{
						if (start->next == node)
						{
							start->next = node->next;
							node->next = NULL;
							ft_f_node(node);
							break ;
						}
						start = start->next;
					}
				}
			}
		}
		i++;
	}
	return (0);
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
