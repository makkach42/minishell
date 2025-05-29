/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers_helper_two.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:07:51 by makkach           #+#    #+#             */
/*   Updated: 2025/05/29 08:59:22 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	plus_handle_word_extractor(char **str, char **word)
// {
// 	char	*tmp_char;

// 	tmp_char = *str;
// 	*str = ft_substr(*str, ft_strlen(*word),
// 			ft_strlen(*str) - (ft_strlen(*word)));
// 	free(tmp_char);
// 	tmp_char = *str;
// 	*str = ft_strtrim(*str, " ");
// 	free(tmp_char);
// }

void	new_operator_word_extractor(char **word, char **str)
{
	char	*tmp_char;

	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	tmp_char = *str;
	*str = ft_substr(*str, ft_strlen(*word),
			ft_strlen(*str) - ft_strlen(*word));
	free(tmp_char);
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp_char);
}

void	equal_handle(char **str, char **word)
{
	char	*tmp_char;

	tmp_char = *word;
	*word = ft_substr(*str, 0, 1);
	free(tmp_char);
	tmp_char = *str;
	*str = ft_substr(*str, 1, ft_strlen(*str) - 1);
	free(tmp_char);
}
