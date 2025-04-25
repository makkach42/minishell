/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:00:03 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 10:00:56 by makkach          ###   ########.fr       */
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
		tmp2 = ft_strjoin_leak(tmp2, tmp->data, 1441);
		t_free(tmp_char, 1442, "parsing.c");
		tmp_char = tmp2;
		tmp2 = ft_strjoin_leak(tmp2, " ", __LINE__);
		t_free(tmp_char, __LINE__, "parsing.c");
		i++;
		tmp = tmp->next;
	}
	return (tmp2);
}

void	init_tree_node(t_tree **node, char *type)
{
	*node = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
	if (!(*node))
		return ;
	(*node)->type = type;
	(*node)->command = NULL;
	(*node)->command_arr = NULL;
	(*node)->redirections = NULL;
}

void	free_node_data(t_list *node)
{
	if (node->data)
		t_free(node->data, __LINE__, "parsing.c");
	t_free(node, __LINE__, "parsing.c");
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
			t_free(right_nodes->data, __LINE__, "parsing.c");
		t_free(right_nodes, __LINE__, "parsing.c");
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
			t_free(left_cleanup->data, __LINE__, "parsing.c");
		t_free(left_cleanup, __LINE__, "parsing.c");
		left_cleanup = next_left;
	}
}
