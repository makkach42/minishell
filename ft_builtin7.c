/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:55:35 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/25 11:41:59 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_unset_error(char *s)
{
	write(2, "unset: ", 8);
	write(2, s, ft_strlen(s));
	write(2, ": invalid parameter name\n", 26);
	return (1);
}

void	ft_unset_helper(t_env **h, t_env *node)
{
	t_env	*start;

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

void	ft_unset_first(t_env **h, t_env *node)
{
	*h = (*h)->next;
	node->next = NULL;
	ft_f_node(node);
}

void	ft_unset_main(t_env **h, t_env *node)
{
	if (*h == node)
		ft_unset_first(h, node);
	else
		ft_unset_helper(h, node);
}

int	ft_unset(t_env **h, char **s)
{
	t_env	*node;
	int		flag;
	int		i;

	flag = 0;
	i = 1;
	node = NULL;
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
				ft_unset_main(h, node);
		}
		i++;
	}
	return (0);
}
