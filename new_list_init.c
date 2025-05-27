/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:04:49 by makkach           #+#    #+#             */
/*   Updated: 2025/05/27 12:10:21 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*equale_handle(char *str)
{
	char	*tmp_char;
	char	*word;

	word = NULL;
	tmp_char = word;
	word = ft_substr(str, 0, 1);
	free(tmp_char);
	tmp_char = str;
	str = ft_substr(str, 1, ft_strlen(str) - 1);
	free(tmp_char);
	return (word);
}

void	new_slice_and_dice_one(char *str, char **word)
{
	if (*str == '"' || *str == '\'')
		*word = new_quote_handle(str);
	else if (*str == '(')
		*word = new_parenthasis_handle(str);
	else if (*str == '$')
		*word = new_variable_handle(str);
	else if (*str == '+')
		*word = plus_handle(str);
	else if (is_operator(*str))
		*word = new_operator_handle(str);
	else
		*word = new_word_handle(str);
}

void	new_slice_and_dice_two(char *str, char **word)
{
	if (*str == '"' || *str == '\'')
		*word = new_quote_handle(str);
	else if (*str == '(')
		*word = new_parenthasis_handle(str);
	else if (*str == '$')
		*word = new_variable_handle(str);
	else if (is_operator(*str))
		*word = new_operator_handle(str);
	else if (*str == '=')
		*word = equale_handle(str);
	else
		*word = new_word_handle(str);
}

void	new_list_node_init(t_list *new_node, char *word)
{
	new_node = malloc(sizeof(t_list));
	new_node->data = ft_strdup(word);
	new_node->token = NULL;
	new_node->prev = NULL;
	new_node->next = NULL;
}

t_list	*new_list_init(char *str)
{
	char	*word;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if (!str)
		return (NULL);
	head = NULL;
	new_node = NULL;
	new_slice_and_dice_one(str, &word);
	new_list_node_init(head, word);
	tmp = head;
	while (str && *str)
	{
		new_slice_and_dice_two(str, &word);
		new_list_node_init(new_node, word);
		new_node->prev = tmp;
		tmp->next = new_node;
		tmp = new_node;
	}
	if (str)
		free(str);
	if (word)
		free(word);
	return (head);
}
