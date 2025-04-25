/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:09:28 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 08:48:53 by makkach          ###   ########.fr       */
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

void	tree_maker(t_list **head, t_tree **tree)
{
	t_list	*tmp;
	t_list	*last;
	t_list	*current;
	t_list	*prev_part;
	t_tree	*command;
	t_list	*to_free;
	t_list	*left_cleanup;
	t_list	*next_left;
	t_list	*right_nodes;
	t_list	*next_right;
	int		i;
	int		total_nodes;

	if (!(*head))
	{
		*tree = NULL;
		return ;
	}
	tmp = *head;
	total_nodes = 0;
	while (tmp)
	{
		last = tmp;
		tmp = tmp->next;
		total_nodes++;
	}
	tmp = last;
	i = total_nodes;
	while (tmp)
	{
		if (ft_strcmp(tmp->token, "OPERATION") == 0)
		{
			*tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
			if (!(*tree))
				return ;
			(*tree)->type = "OPERATION";
			(*tree)->command = NULL;
			(*tree)->command_arr = NULL;
			(*tree)->redirections = NULL;
			command = NULL;
			if (tmp->next)
			{
				command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
				if (!command)
				{
					t_free(*tree, __LINE__, "parsing.c");
					*tree = NULL;
					return ;
				}
				if (tmp->next->token && tmp->next->token[0] != '\0')
				{
					command->type = tmp->next->token;
				}
				else
				{
					command->type = "WORD";
				}
				command->command = side_maker(&(tmp->next), total_nodes - i, 0);
				command->left = NULL;
				command->right = NULL;
				command->command_arr = NULL;
				command->redirections = NULL;
				right_nodes = tmp->next;
				while (right_nodes)
				{
					next_right = right_nodes->next;
					if (right_nodes->data)
						t_free(right_nodes->data, __LINE__, "parsing.c");
					t_free(right_nodes, __LINE__, "parsing.c");
					right_nodes = next_right;
				}
			}
			(*tree)->right = command;
			if (tmp->prev)
			{
				prev_part = *head;
				tmp->prev->next = NULL;
				if (tmp->data)
					t_free(tmp->data, __LINE__, "parsing.c");
				t_free(tmp, __LINE__, "parsing.c");
				if (prev_part)
				{
					tree_maker(&prev_part, &(*tree)->left);
					if (prev_part)
					{
						left_cleanup = prev_part;
						while (left_cleanup)
						{
							next_left = left_cleanup->next;
							if (left_cleanup->data)
								t_free(
									left_cleanup->data, __LINE__, "parsing.c");
							t_free(left_cleanup, __LINE__, "parsing.c");
							left_cleanup = next_left;
						}
					}
				}
			}
			else
			{
				if (tmp->data)
					t_free(tmp->data, __LINE__, "parsing.c");
				t_free(tmp, __LINE__, "parsing.c");
				(*tree)->left = NULL;
			}
			*head = NULL;
			return ;
		}
		tmp = tmp->prev;
		i--;
	}
	tmp = last;
	i = total_nodes;
	while (tmp)
	{
		if (ft_strcmp(tmp->token, "PIPE") == 0)
		{
			*tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
			if (!(*tree))
				return ;
			(*tree)->type = "PIPE";
			(*tree)->command = NULL;
			(*tree)->command_arr = NULL;
			(*tree)->redirections = NULL;
			command = NULL;
			if (tmp->next)
			{
				command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
				if (!command)
				{
					t_free(*tree, __LINE__, "parsing.c");
					*tree = NULL;
					return ;
				}
				if (tmp->next->token && tmp->next->token[0] != '\0')
				{
					command->type = tmp->next->token;
				}
				else
				{
					command->type = "WORD";
				}
				command->command = side_maker(&(tmp->next), total_nodes - i, 0);
				command->left = NULL;
				command->right = NULL;
				command->command_arr = NULL;
				command->redirections = NULL;
				right_nodes = tmp->next;
				while (right_nodes)
				{
					next_right = right_nodes->next;
					if (right_nodes->data)
						t_free(right_nodes->data, __LINE__, "parsing.c");
					t_free(right_nodes, __LINE__, "parsing.c");
					right_nodes = next_right;
				}
			}
			(*tree)->right = command;
			if (tmp->prev)
			{
				prev_part = *head;
				tmp->prev->next = NULL;
				if (tmp->data)
					t_free(tmp->data, __LINE__, "parsing.c");
				t_free(tmp, __LINE__, "parsing.c");
				if (prev_part)
				{
					tree_maker(&prev_part, &(*tree)->left);
					if (prev_part)
					{
						left_cleanup = prev_part;
						while (left_cleanup)
						{
							next_left = left_cleanup->next;
							if (left_cleanup->data)
								t_free(
									left_cleanup->data, __LINE__, "parsing.c");
							t_free(left_cleanup, __LINE__, "parsing.c");
							left_cleanup = next_left;
						}
					}
				}
			}
			else
			{
				if (tmp->data)
					t_free(tmp->data, __LINE__, "parsing.c");
				t_free(tmp, __LINE__, "parsing.c");
				(*tree)->left = NULL;
			}
			*head = NULL;
			return ;
		}
		tmp = tmp->prev;
		i--;
	}
	if (*head)
	{
		*tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
		if (!(*tree))
			return ;
		if ((*head)->token && (*head)->token[0] != '\0')
		{
			(*tree)->type = (*head)->token;
		}
		else
		{
			(*tree)->type = "WORD";
		}
		(*tree)->command = side_maker(head, total_nodes - i, 0);
		(*tree)->left = NULL;
		(*tree)->right = NULL;
		(*tree)->command_arr = NULL;
		(*tree)->redirections = NULL;
		current = *head;
		while (current)
		{
			to_free = current;
			current = current->next;
			if (to_free->data)
				t_free(to_free->data, __LINE__, "parsing.c");
			t_free(to_free, __LINE__, "parsing.c");
		}
		*head = NULL;
	}
}
