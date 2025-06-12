/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_init_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:27:49 by makkach           #+#    #+#             */
/*   Updated: 2025/06/12 17:15:12 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes(char **word, char **str)
{
	char	*tmp_char;

	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
	*word = str_extractor(*str);
	if (!word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = str_remover(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim(*str, " ");
		free(tmp_char);
	}
}

void	handle_operator(char **word, char **str)
{
	char	*tmp_char;

	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
	*word = extract_operator(*str);
	if (!word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = remove_operator(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim(*str, " ");
		free(tmp_char);
	}
}

void	variable_handle(char **word, char **str)
{
	char	*tmp_char;

	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
	*word = extract_variable(*str);
	if (!*word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = first_word_remover(*str, *word);
	free(tmp_char);
	if (str)
	{
		tmp_char = *str;
		*str = ft_strtrim(*str, " ");
		free(tmp_char);
	}
}

void	handle_parenthases(char **word, char **str)
{
	char	*tmp_char;

	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
	*word = extract_parenthesis(*str);
	if (!*word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = parenthesis_remover(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim(*str, " ");
		free(tmp_char);
	}
}

void	handle_else(char **word, char **str)
{
	char	*tmp_char;

	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
	*word = word_extractor(*str);
	if (!*word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = first_word_remover(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim(*str, " ");
		free(tmp_char);
	}
}
