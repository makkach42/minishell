/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:26:59 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/28 11:12:20 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_env	*ft_check(t_env *h, char *str)
{
	if (!h || !h->next || !str)
		return (NULL);
	while (h != NULL)
	{
		if (ft_strcmp(h->key, str) == 0)
			return (h);
		h = h->next;
	}
	return (NULL);
}

int	ft_parse(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	if (ft_isalpha(s[0]) != 0 && s[0] != '_')
	{
		return (1);
	}
	while (s[i] != '\0')
	{
		if (ft_isalpha(s[i]) != 0 && s[i] != '_' && ft_isdigit(s[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*help;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		new -> next = *lst;
		*lst = new;
		return ;
	}
	help = *lst;
	while (help -> next != NULL)
	{
		help = help -> next;
	}
	help -> next = new;
}
