/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers_helper_two.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:07:51 by makkach           #+#    #+#             */
/*   Updated: 2025/05/27 12:08:52 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	plus_handle_word_extractor(char *tmp_char, char *str, char *word)
{
	tmp_char = str;
	str = ft_substr(str, ft_strlen(word),
			ft_strlen(str) - (ft_strlen(word)));
	free(tmp_char);
	tmp_char = str;
	str = ft_strtrim(str, " ");
	free(tmp_char);
}

void	new_operator_word_extractor(char *word, char *tmp_char, char *str)
{
	tmp_char = word;
	word = ft_strtrim(word, " ");
	free(tmp_char);
	tmp_char = str;
	str = ft_substr(str, ft_strlen(word), ft_strlen(str) - ft_strlen(word));
	free(tmp_char);
	tmp_char = str;
	str = ft_strtrim(str, " ");
	free(tmp_char);
}
