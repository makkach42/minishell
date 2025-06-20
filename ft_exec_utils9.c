/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils9.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:26:36 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/19 15:44:59 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*env_fill_pwd(t_env *head)
{
	t_env	*tmp;

	tmp = ft_check(head, "OLDPWD");
	if (tmp)
	{
		tmp->active = 1;
		(free(tmp->value), tmp->value = NULL);
	}
	ft_lstadd_back(&head, ft_lstnew("1PWD", NULL));
	tmp = ft_check(head, "1PWD");
	if (tmp)
	{
		tmp->active = 1;
		tmp->h = 1;
	}
}

void	ft_env_helper(t_env *new_node, t_env **tmp)
{
	new_node->next = NULL;
	(*tmp)->next = new_node;
	*tmp = new_node;
	(*tmp)->active = 0;
	(*tmp)->h = 0;
}

void	ft_env_helper2(t_env **head, int *i, int *j, char **argev)
{
	env_fill_helper(head, i, j, argev);
	(*head)->next = NULL;
	(*head)->h = 0;
	(*head)->active = 0;
}

t_env	*env_fill(char **argev)
{
	int		i;
	int		j;
	t_env	*head;
	t_env	*new_node;
	t_env	*tmp;

	i = 0;
	j = 0;
	if (!argev || !*argev)
		env_fill_empty(&head, &i, &j);
	else
	{
		ft_env_helper2(&head, &i, &j, argev);
		tmp = head;
		i++;
		while (argev[i])
		{
			env_fill_helper(&new_node, &i, &j, argev);
			ft_env_helper(new_node, &tmp);
			i++;
		}
		env_fill_pwd(head);
	}
	return (head);
}

void	ft_close_handle(t_list_fd *head)
{
	if (!head)
		return ;
	while (head)
	{
		close (head->fd1);
		head = head->next;
	}
}
