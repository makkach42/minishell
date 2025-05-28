/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_vars_helper_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 07:49:29 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 18:03:23 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_list_size(char ***cmd2, int *i, t_list *head, int list_size)
{
	char	**cmd;
	int		j;
	t_list	*tmp;
	char	*new_str;

	cmd = malloc(sizeof(char *) * (list_size + 1));
	j = 0;
	tmp = head;
	while (tmp)
	{
		new_str = ft_strdup(tmp->data);
		cmd[j] = new_str;
		j++;
		tmp = tmp->next;
	}
	cmd[j] = NULL;
	cmd2[*i] = cmd;
	free_list(&head);
	(*i)++;
}

static int	chararrcount(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

char	***cmd2_fill(t_tree **tree)
{
	int		i;
	int		list_size;
	char	***cmd2;
	char	*old_cmd;
	t_list	*head;

	i = chararrcount((*tree)->command_arr);
	cmd2 = malloc(sizeof(char **) * (i + 1));
	i = 0;
	while ((*tree)->command_arr[i])
	{
		old_cmd = ft_strdup((*tree)->command_arr[i]);
		head = list_init(old_cmd);
		list_size = lst_size(&head);
		if (list_size == 0)
		{
			process_no_list_size(head, list_size, cmd2, &i);
			free(old_cmd);
		}
		else
			process_list_size(cmd2, &i, head, list_size);
	}
	cmd2[i] = NULL;
	return (cmd2);
}

void	free_twod_char(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	count_triple_ptr(char ***cmd2)
{
	int	m;
	int	i;
	int	j;

	m = 0;
	i = 0;
	while (cmd2[i])
	{
		j = 0;
		while (cmd2[i][j])
		{
			m++;
			j++;
		}
		i++;
	}
	return (m);
}
