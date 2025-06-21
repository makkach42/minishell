/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init_handlers_helper_two.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:07:51 by makkach           #+#    #+#             */
/*   Updated: 2025/06/21 08:04:13 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_operator_word_extractor(char **word, char **str)
{
	char	*tmp_char;

	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_substr(*str, ft_strlen(*word),
			ft_strlen(*str) - ft_strlen(*word));
	if (!*str)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!str)
		return ;
	free(tmp_char);
}

void	equal_handle(char **str, char **word)
{
	char	*tmp_char;

	tmp_char = *word;
	*word = ft_substr(*str, 0, 1);
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	if (!*word)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_substr(*str, 1, ft_strlen(*str) - 1);
	if (!*str)
		return ;
	free(tmp_char);
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	if (!*str)
		return ;
	free(tmp_char);
}

int	check_inside_parenths(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '(' && str[i + 1] && str[i + 1] == ')')
			return (1);
		i++;
	}
	return (0);
}
