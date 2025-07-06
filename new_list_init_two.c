/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:39:26 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 11:19:54 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parenthases_two(char **str, char **word)
{
	char	*tmp;
	int		i;

	i = 0;
	*word = ft_substr(*str, 0, 1);
	if (!*word)
		return ;
	tmp = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp);
	if (!*word)
		return ;
	tmp = *str;
	*str = ft_substr(*str, 1, ft_strlen(*str) - 1);
	free(tmp);
	if (!*str)
		return ;
	tmp = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp);
	if (!*str)
		return ;
}

void	cut_and_slice_two(char **word, char **str)
{
	if (**str == '\"' || **str == '\'')
		handle_quotes_two(word, str);
	else if (is_operator(**str))
		handle_operator(word, str);
	else if (**str == '$')
		variable_handle(word, str);
	else if (**str == '(' || **str == ')')
		handle_parenthases_two(str, word);
	else
		handle_else_two(str, word);
}

t_list	*new_list_init_two(char *str)
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
	cut_and_slice_two(&word, &str);
	head = node_maker(word);
	if (!head)
		return (NULL);
	tmp = head;
	while (str && *str)
	{
		cut_and_slice_two(&word, &str);
		handle_new_node(&new_node, &tmp, &word);
	}
	if (str)
		free(str);
	return (head);
}
