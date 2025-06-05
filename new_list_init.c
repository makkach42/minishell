/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:04:49 by makkach           #+#    #+#             */
/*   Updated: 2025/06/04 17:27:38 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	slice_and_dice_one(char **str, char **word)
{
	if (**str == '"' || **str == '\'')
		handle_quotes(word, str);
	else if (**str == '(')
		new_parenthasis_handle(str, word);
	else if (**str == '$')
		new_variable_handle(str, word);
	else if (is_operator(**str))
		new_operator_handle(str, word);
	else
		new_word_handle(str, word);
}

static void	slice_and_dice_two(char **str, char **word)
{
	if (**str == '"' || **str == '\'')
		handle_quotes(word, str);
	else if (**str == '(')
		new_parenthasis_handle(str, word);
	else if (**str == '$')
		new_variable_handle(str, word);
	else if (is_operator(**str))
		new_operator_handle(str, word);
	else if (**str == '=')
		equal_handle(str, word);
	else
		new_word_handle(str, word);
}

static void	new_node_init(t_list **node, char **word)
{
	(*node) = malloc(sizeof(t_list));
	(*node)->data = ft_strdup(*word);
	(*node)->token = NULL;
	(*node)->prev = NULL;
	(*node)->next = NULL;
}

t_list	*new_list_init(char *str)
{
	char	*word;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if (!str)
		return (NULL);
	word = NULL;
	slice_and_dice_one(&str, &word);
	new_node_init(&head, &word);
	free(word);
	tmp = head;
	while (str && *str)
	{
		word = NULL;
		slice_and_dice_two(&str, &word);
		new_node_init(&new_node, &word);
		free(word);
		new_node->prev = tmp;
		tmp->next = new_node;
		tmp = new_node;
	}
	if (str)
		free(str);
	return (head);
}
