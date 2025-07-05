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

void	handle_else_two(char **word, char **str)
{
	char	*tmp_char;
	int		i;

	if (!str || !*str)
		return ;
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
	{
		free(tmp_char);
		return ;
	}
	free(tmp_char);
	
	// Find the first operator, parenthesis, or dollar sign
	i = 0;
	while ((*str)[i] && (*str)[i] != '(' && (*str)[i] != ')' && 
		   (*str)[i] != '$' && !is_operator((*str)[i]))
		i++;
	
	if (i == 0)
		return ;
	
	// Extract the word before the special character
	*word = ft_substr(*str, 0, i);
	if (!*word)
		return ;
	
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	if (!*word)
	{
		free(tmp_char);
		return ;
	}
	free(tmp_char);
	
	// Check if the trimmed word is empty
	if (!**word)
	{
		free(*word);
		*word = NULL;
		return ;
	}
	
	// Update str to remove the extracted word
	tmp_char = *str;
	*str = ft_substr(*str, i, ft_strlen(*str) - i);
	if (!*str)
	{
		free(tmp_char);
		return ;
	}
	free(tmp_char);
	
	// Trim whitespace from the remaining string
	if (*str)
	{
		tmp_char = *str;
		*str = ft_strtrim(*str, " ");
		if (!*str)
		{
			free(tmp_char);
			return ;
		}
		free(tmp_char);
	}
}
