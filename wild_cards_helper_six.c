/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_six.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:10:40 by makkach           #+#    #+#             */
/*   Updated: 2025/06/02 11:32:43 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_entry(char **str, char *str2)
{
	char	*tmp_char;

	tmp_char = (*str);
	(*str) = ft_strjoin((*str), str2);
	free(tmp_char);
	tmp_char = (*str);
	(*str) = ft_strjoin((*str), " ");
	free(tmp_char);
}

void	join_to_node_name(t_list_fd **node, char **str)
{
	char	*tmp_char;

	free((*node)->name);
	tmp_char = (*str);
	(*str) = ft_strtrim((*str), " ");
	free(tmp_char);
	(*node)->name = (*str);
	(*node)->name_split = ft_split((*str), 32);
}

void	process_wildcard_node_fd(t_list_fd *node, char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*str;

	str = NULL;
	dir = opendir(dir_path);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (*entry->d_name == '.' && *node->name != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(node->name, entry->d_name))
			join_entry(&str, entry->d_name);
		entry = readdir(dir);
	}
	if (str)
		join_to_node_name(&node, &str);
	closedir(dir);
}
