/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_init_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:27:49 by makkach           #+#    #+#             */
/*   Updated: 2025/04/24 09:31:04 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes(char **word, char **str)
{
	char	*tmp_char;

	*word = str_extractor(*str);
	if (!word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim_leak(*word, " ", 1092, "list_init");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = str_remover(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim_leak(*str, " ", 1102, "list_init");
		free(tmp_char);
	}
}

void	handle_operator(char **word, char **str)
{
	char	*tmp_char;

	*word = extract_operator(*str);
	if (!word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim_leak(*word, " ", 1113, "list_init");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = remove_operator(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim_leak(*str, " ", 1126, "list_init");
		free(tmp_char);
	}
}

void	variable_handle(char **word, char **str)
{
	char	*tmp_char;

	*word = extract_variable(*str);
	if (!*word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim_leak(*word, " ", 1137, "list_init");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = first_word_remover(*str, *word);
	free(tmp_char);
	if (str)
	{
		tmp_char = *str;
		*str = ft_strtrim_leak(*str, " ", 1150, "list_init");
		free(tmp_char);
	}
}

void	handle_parenthases(char **word, char **str)
{
	char	*tmp_char;

	*word = extract_parenthesis(*str);
	if (!*word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim_leak(*word, " ", 1161, "list_init");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = parenthesis_remover(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim_leak(*str, " ", 1174, "list_init");
		free(tmp_char);
	}
}

void	handle_else(char **word, char **str)
{
	char	*tmp_char;

	*word = word_extractor(*str);
	if (!*word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim_leak(*word, " ", 1185, "list_init");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = first_word_remover(*str, *word);
	free(tmp_char);
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim_leak(*str, " ", 1197, "list_init");
		free(tmp_char);
	}
}
