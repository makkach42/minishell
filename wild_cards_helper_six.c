/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_six.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:10:40 by makkach           #+#    #+#             */
/*   Updated: 2025/06/24 18:13:16 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_entry(char **str, char *str2)
{
	char	*tmp_char;

	tmp_char = (*str);
	(*str) = ft_strjoin((*str), str2);
	if (!(*str))
		return ;
	free(tmp_char);
	tmp_char = (*str);
	(*str) = ft_strjoin((*str), " ");
	if (!(*str))
		return ;
	free(tmp_char);
}

void	join_to_node_name(t_list_fd **node, char **str)
{
	char	*tmp_char;

	free((*node)->name);
	tmp_char = (*str);
	(*str) = ft_strtrim((*str), " ");
	if (!(*str))
		return ;
	free(tmp_char);
	(*node)->name = (*str);
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

int	variable_search_inlnkedlst(t_tree **tree)
{
	t_list_fd	*tmplst;
	int			i;

	i = 0;
	if ((*tree) && (*tree)->fd_list)
	{
		tmplst = (*tree)->fd_list;
		while (tmplst)
		{
			if (variable_search_instr(tmplst->name
				) && ft_strcmp(tmplst->redir, "<<"))
				i = 1;
			tmplst = tmplst->next;
		}
	}
	if ((*tree) && (*tree)->left)
		i += variable_search_inlnkedlst(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		i += variable_search_inlnkedlst(&(*tree)->right);
	return (i);
}
