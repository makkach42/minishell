/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers_helper.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:34:49 by makkach           #+#    #+#             */
/*   Updated: 2025/07/04 20:34:58 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_word_word_extractor(char **word, char **str, int i)
{
	char	*tmp_char;

	tmp_char = *word;
	*word = ft_substr(*str, 0, i);
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_substr(*str, i, ft_strlen(*str) - (i));
	if (!*str)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
}

void	new_word_handle(char **str, char **word)
{
	int		i;
	char	*tmp_char;

	i = 0;
	tmp_char = NULL;
	while ((*str)[i])
	{
		if (((*str)[i] == '"') || ((*str)[i] == '\''
			) || ((*str)[i] == '$') || (
				is_operator((*str)[i])) || (
				(*str)[i] == '(') || (
				(*str)[i] == '='))
			break ;
		i++;
	}
	new_word_word_extractor(word, str, i);
}

void	new_parenthasis_word_extractor(char **word, int i, char **str)
{
	char	*tmp_char;

	tmp_char = *word;
	*word = ft_substr(*str, 0, i + 1);
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_substr(*str, i + 1, ft_strlen(*str) - (i + 1));
	if (!*str)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
}

void	new_variable_word_extractor(char **str, char **word, int i)
{
	char	*tmp_char;

	tmp_char = *word;
	*word = ft_substr(*str, 0, i);
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_substr(*str, i, ft_strlen(*str) - (i));
	if (!*str)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
}
