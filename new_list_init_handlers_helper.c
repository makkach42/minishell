/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers_helper.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:34:49 by makkach           #+#    #+#             */
/*   Updated: 2025/05/27 12:11:46 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_word_word_extractor(char *tmp_char, char *word, char *str, int i)
{
	tmp_char = word;
	word = ft_substr(str, 0, i);
	free(tmp_char);
	tmp_char = word;
	word = ft_strtrim(word, " ");
	free(tmp_char);
	tmp_char = str;
	str = ft_substr(str, i, ft_strlen(str) - (i));
	free(tmp_char);
	tmp_char = str;
	str = ft_strtrim(str, " ");
	free(tmp_char);
}

char	*new_word_handle(char *str)
{
	int		i;
	char	*tmp_char;
	char	*word;

	i = 0;
	word = NULL;
	tmp_char = NULL;
	while (str[i])
	{
		if ((str[i] == '"') || (str[i] == '\''
			) || (str[i] == '$') || (
				is_operator(str[i])) || (
				str[i] == '(') || (
				str[i] == '='))
			break ;
		i++;
	}
	new_word_word_extractor(tmp_char, word, str, i);
	return (word);
}

void	new_quote_word_extractor(char *tmp_char, char *word, char *str, int i)
{
	tmp_char = word;
	word = ft_substr(str, 0, i + 1);
	free(tmp_char);
	tmp_char = word;
	word = ft_strtrim(word, " ");
	free(tmp_char);
	tmp_char = str;
	str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
	free(tmp_char);
	tmp_char = str;
	str = ft_strtrim(str, " ");
	free(tmp_char);
}

void	new_parenthasis_word_extractor(char *word,
		char *tmp_char, int i, char *str)
{
	tmp_char = word;
	word = ft_substr(str, 0, i + 1);
	free(tmp_char);
	tmp_char = word;
	word = ft_strtrim(word, " ");
	free(tmp_char);
	tmp_char = str;
	str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
	free(tmp_char);
	tmp_char = str;
	str = ft_strtrim(str, " ");
	free(tmp_char);
}

void	new_variable_word_extractor(char *str, char *word, int i)
{
	char	*tmp_char;

	tmp_char = word;
	word = ft_substr(str, 0, i);
	free(tmp_char);
	tmp_char = word;
	word = ft_strtrim(word, " ");
	free(tmp_char);
	tmp_char = str;
	str = ft_substr(str, i, ft_strlen(str) - (i));
	free(tmp_char);
	tmp_char = str;
	str = ft_strtrim(str, " ");
	free(tmp_char);
}
