/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:06:02 by makkach           #+#    #+#             */
/*   Updated: 2025/05/04 16:07:42 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*node_maker(char *word)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	node->data = word;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	cut_and_slice(char **word, char **str)
{
	if (**str == '\"' || **str == '\'')
		handle_quotes(word, str);
	else if (is_operator(**str))
		handle_operator(word, str);
	else if (**str == '$')
		variable_handle(word, str);
	else if (**str == '(')
		handle_parenthases(word, str);
	else
		handle_else(word, str);
}

void	handle_new_node(t_list **new_node, t_list **tmp, char **word)
{
	*new_node = malloc(sizeof(t_list));
	if (!*new_node)
	{
		free(*word);
		return ;
	}
	(*new_node)->data = *word;
	(*new_node)->next = NULL;
	(*new_node)->prev = *tmp;
	(*tmp)->next = *new_node;
	*tmp = *new_node;
}

t_list	*list_init(char *str)
{
	char	*word;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	word = NULL;
	head = NULL;
	new_node = NULL;
	tmp = NULL;
	if (!str || !*str)
		return (NULL);
	cut_and_slice(&word, &str);
	head = node_maker(word);
	if (!head)
		return (NULL);
	tmp = head;
	while (str && *str)
	{
		cut_and_slice(&word, &str);
		handle_new_node(&new_node, &tmp, &word);
	}
	if (str)
		free(str);
	return (head);
}
