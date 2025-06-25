/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:18:26 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/23 14:23:18 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_list_changer(t_env *h, t_env *start)
{
	char	*tmp_key;
	char	*tmp_value;
	int		active;
	int		hidden;

	active = 0;
	hidden = 0;
	tmp_key = NULL;
	tmp_value = NULL;
	tmp_key = h->key;
	h->key = start->key;
	start->key = tmp_key;
	tmp_value = h->value;
	h->value = start->value;
	start->value = tmp_value;
	active = h->active;
	h->active = start->active;
	start->active = active;
	hidden = h->h;
	h->h = start->h;
	start->h = hidden;
}

t_env	*ft_sort_list(t_env *h)
{
	t_env	*head;
	t_env	*start;

	head = h;
	start = NULL;
	while (h != NULL && h->next != NULL)
	{
		start = h->next;
		while (start != NULL)
		{
			if (ft_strcmp(h->key, start->key) > 0)
				ft_list_changer(h, start);
			else if (ft_strcmp(h->key, start->key) == 0
				&& ft_strcmp(h->value, start->value) > 0)
				ft_list_changer(h, start);
			start = start->next;
		}
		h = h->next;
	}
	return (head);
}

int	ft_equal_check(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

void	ft_remove_sign(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '+' && str[i + 1] == '\0')
			str[i] = '\0';
		i++;
	}
}

int	ft_equal_count(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '=')
			count++;
		i++;
	}
	return (count);
}
