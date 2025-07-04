/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:45:05 by makkach           #+#    #+#             */
/*   Updated: 2025/07/04 20:36:05 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_parenthasis_handle(char **str, char **word)
{
	int		i;
	int		open_par;
	int		closed_par;

	i = 0;
	open_par = 0;
	closed_par = 0;
	while (*str[i])
	{
		if (*str[i] == '(')
			open_par++;
		else if (*str[i] == ')')
			closed_par++;
		if (open_par == closed_par && open_par != 0)
			break ;
		i++;
	}
	new_parenthasis_word_extractor(word, i, str);
}

void	new_variable_handle(char **str, char **word)
{
	int		i;

	i = 1;
	while ((*str)[i])
	{
		if (!((*str)[i] >= 'a' && (*str)[i] <= 'z') && !(
				(*str)[i] >= 'A' && (*str)[i] <= 'Z') && !(
				(*str)[i] >= '0' && (*str)[i] <= '9') && !(
				(*str)[i] == '+'))
			break ;
		i++;
	}
	new_variable_word_extractor(str, word, i);
}

void	new_operator_handle(char **str, char **word)
{
	if (*str[0] == '&' && ((*str[1] && *str[1] != '&') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '*' && ((*str[1] && *str[1] != '*') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '|' && ((*str[1] && *str[1] != '|') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '>' && ((*str[1] && *str[1] != '>') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '<' && ((*str[1] && *str[1] != '<') || !*str[1]))
		*word = ft_substr(*str, 0, 1);
	else if (*str[0] == '&' && *str[1] && *str[1] == '&')
		*word = ft_substr(*str, 0, 2);
	else if (*str[0] == '|' && *str[1] && *str[1] == '|')
		*word = ft_substr(*str, 0, 2);
	else if (*str[0] == '>' && *str[1] && *str[1] == '>')
		*word = ft_substr(*str, 0, 2);
	else if (*str[0] == '<' && *str[1] && *str[1] == '<')
		*word = ft_substr(*str, 0, 2);
	if (!*word)
		return ;
	new_operator_word_extractor(word, str);
}
