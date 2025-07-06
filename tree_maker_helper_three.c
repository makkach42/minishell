/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker_helper_three.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:12:02 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 11:12:50 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_operation_node(t_list **head, t_tree **tree, t_list *tmp,
							int *node_info)
{
	t_list	*prev_part;

	init_tree_node(tree, tmp->token);
	if (!(*tree))
		return ;
	handle_operation_command(tree, tmp, node_info[0], node_info[1]);
	if (!(*tree))
		return ;
	if_prev(tmp, &prev_part, tree, head);
	*head = NULL;
}

void	process_pipe_node(t_list **head, t_tree **tree, t_list *tmp,
						int *node_info)
{
	t_list	*prev_part;

	init_tree_node(tree, "PIPE");
	if (!(*tree))
		return ;
	handle_operation_command(tree, tmp, node_info[0], node_info[1]);
	if (!(*tree))
		return ;
	if_prev(tmp, &prev_part, tree, head);
	*head = NULL;
}

int	parenthasis_recognizer_two(char *str)
{
	if (!ft_strcmp("(", str))
		return (1);
	else if (!ft_strcmp(")", str))
		return (2);
	return (0);
}

void	tokenizer_two(t_list *tmp)
{
	if (command_recognizer(tmp->data) == 1)
		tmp->token = "COMMAND";
	else if (variable_recognizer(tmp->data) == 1)
		tmp->token = "VARIABLE";
	else if (parenthasis_recognizer_two(tmp->data) == 1)
		tmp->token = "OPEN_PAR";
	else if (parenthasis_recognizer_two(tmp->data) == 2)
		tmp->token = "CLOSED_PAR";
	else if (word_recognizer(tmp->data) == 1)
		tmp->token = "WORD";
	else if (pipe_recognizer(tmp->data) == 1)
		tmp->token = "PIPE";
	else if (pipe_recognizer(tmp->data) == 2)
		tmp->token = "OPERATION_||";
	else if (redirection_recognizer(tmp->data) == 1)
		tmp->token = "REDIRECTION";
	else if (operation_recognizer(tmp->data) == 1)
		tmp->token = "OPERATION_&&";
	else
		tmp->token = "WORD";
}

void	lexer_two(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (tmp)
	{
		tokenizer_two(tmp);
		tmp = tmp->next;
	}
}
