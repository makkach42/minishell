/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_four.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:21:25 by makkach           #+#    #+#             */
/*   Updated: 2025/06/20 16:26:10 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pattern_matches(const char *pattern, char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;
	int				match_count;

	match_count = 0;
	dir = opendir(dir_path);
	if (dir == NULL)
		return (0);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name))
			match_count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

char	*str_duplicate(const char *s)
{
	size_t		len;
	char		*dup;
	char		*ptr;

	len = 0;
	while (s[len])
		len++;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ptr = dup;
	while (*s)
		*ptr++ = *s++;
	*ptr = '\0';
	return (dup);
}

int	has_wild_cards_fdlst(t_tree **tree)
{
	int			i;
	t_list_fd	*tmp;

	i = 0;
	if ((*tree) && (*tree)->fd_list)
	{
		tmp = (*tree)->fd_list;
		while (tmp)
		{
			if (if_has_wildcards(tmp->name))
				i = 1;
			tmp = tmp->next;
		}
	}
	if ((*tree) && (*tree)->left)
		i = has_wild_cards_fdlst(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		i = has_wild_cards_fdlst(&(*tree)->right);
	return (i);
}

static void	wild_cards_handle_fd_list(t_list_fd **fd_list, char *dir_path)
{
	t_list_fd	*node;
	t_list_fd	**wildcard_nodes;
	int			wildcard_count;
	int			i;

	if (!fd_list || !*fd_list || !dir_path)
		return ;
	wildcard_count = 0;
	node = *fd_list;
	wildcard_nodes = malloc(sizeof(t_list_fd *) * (fd_list_size(fd_list) + 1));
	while (node && wildcard_count < fd_list_size(fd_list))
	{
		if (if_has_wildcards(node->name))
		{
			wildcard_nodes[wildcard_count] = node;
			wildcard_count++;
		}
		node = node->next;
	}
	i = -1;
	while (++i < wildcard_count)
		process_wildcard_node_fd(wildcard_nodes[i], dir_path);
	free(wildcard_nodes);
}

void	handle_wildcards_in_fdlst(t_tree **tree)
{
	t_list_fd	*tmp;
	char		*dir_name;

	if ((*tree)->fd_list)
	{
		tmp = (*tree)->fd_list;
		while (tmp)
		{
			if (if_has_wildcards(tmp->name))
			{
				dir_name = getcwd(NULL, 0);
				if (!dir_name)
					return ;
				wild_cards_handle_fd_list(&(*tree)->fd_list, dir_name);
				free(dir_name);
			}
			tmp = tmp->next;
		}
	}
	if ((*tree)->left)
		has_wild_cards_fdlst(&(*tree)->left);
	if ((*tree)->right)
		has_wild_cards_fdlst(&(*tree)->right);
}
