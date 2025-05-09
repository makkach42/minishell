/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:00:03 by makkach           #+#    #+#             */
/*   Updated: 2025/05/09 11:20:01 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*side_maker(t_list **head, int number, int j)
{
	t_list	*tmp;
	char	*tmp2;
	char	*tmp_char;
	int		i;

	tmp = *head;
	i = 0;
	tmp2 = NULL;
	while (i < number - j)
	{
		tmp_char = tmp2;
		tmp2 = ft_strjoin(tmp2, tmp->data);
		free(tmp_char);
		tmp_char = tmp2;
		tmp2 = ft_strjoin(tmp2, " ");
		free(tmp_char);
		i++;
		tmp = tmp->next;
	}
	return (tmp2);
}

void	init_tree_node(t_tree **node, char *type)
{
	*node = malloc(sizeof(t_tree));
	if (!(*node))
		return ;
	(*node)->type = type;
	(*node)->command = NULL;
	(*node)->command_arr = NULL;
	(*node)->redirections = NULL;
	(*node)->fd_list = NULL;
	(*node)->ambiguous = 0;
	(*node)->quotes = 0;
	(*node)->var = 0;
}

void	free_node_data(t_list *node)
{
	if (node->data)
		free(node->data);
	free(node);
}

void	cleanup_right_nodes(t_list *nodes)
{
	t_list	*right_nodes;
	t_list	*next_right;

	right_nodes = nodes;
	while (right_nodes)
	{
		next_right = right_nodes->next;
		if (right_nodes->data)
			free(right_nodes->data);
		free(right_nodes);
		right_nodes = next_right;
	}
}

void	cleanup_left_nodes(t_list *nodes)
{
	t_list	*left_cleanup;
	t_list	*next_left;

	left_cleanup = nodes;
	while (left_cleanup)
	{
		next_left = left_cleanup->next;
		if (left_cleanup->data)
			free(left_cleanup->data);
		free(left_cleanup);
		left_cleanup = next_left;
	}
}
