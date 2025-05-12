/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:13:43 by makkach           #+#    #+#             */
/*   Updated: 2025/05/12 10:57:37 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_has_wildcards(char *str)
{
	int	i;
	int	in_quotes;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			in_quotes = !in_quotes;
		if (str[i] == '*' && !in_quotes)
			return (1);
		i++;
	}
	return (0);
}

int	has_wild_cards_comarr(t_tree **tree)
{
	int	i;
	int	j;

	i = 0;
	if ((*tree)->command_arr)
	{
		j = 0;
		while ((*tree)->command_arr[j])
		{
			if (if_has_wildcards((*tree)->command_arr[j]))
				i = 1;
			j++;
		}
	}
	if ((*tree)->left)
		i = has_wild_cards_comarr(&(*tree)->left);
	if ((*tree)->right)
		i = has_wild_cards_comarr(&(*tree)->right);
	return (i);
}

int	has_wild_cards_fdlst(t_tree **tree)
{
	int			i;
	t_list_fd	*tmp;

	i = 0;
	if ((*tree)->fd_list)
	{
		tmp = (*tree)->fd_list;
		while (tmp)
		{
			if (if_has_wildcards(tmp->name))
				i = 1;
			tmp = tmp->next;
		}
	}
	if ((*tree)->left)
		i = has_wild_cards_fdlst(&(*tree)->left);
	if ((*tree)->right)
		i = has_wild_cards_fdlst(&(*tree)->right);
	return (i);
}

int	match_pattern(const char *pattern, const char *string)
{
	int		i;
	int		j;
	int		star_idx;
	int		str_idx;

	i = 0;
	j = 0;
	star_idx = -1;
	str_idx = -1;
	while (string[j])
	{
		if (pattern[i] == '*')
		{
			while (pattern[i + 1] == '*')
				i++;
			if (!pattern[i + 1])
				return (1);
			star_idx = i++;
			str_idx = j;
			continue ;
		}
		if (pattern[i] == string[j])
		{
			i++;
			j++;
			continue ;
		}
		if (star_idx >= 0)
		{
			i = star_idx + 1;
			j = ++str_idx;
			continue ;
		}
		return (0);
	}
	while (pattern[i] == '*')
		i++;
	return (!pattern[i]);
}

char	*str_duplicate(const char *s)
{
	size_t		len;
	const char	*tmp;
	char		*dup;
	char		*ptr;

	len = 0;
	tmp = s;
	while (*tmp++)
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

void	sort_matches(char **arr, int count)
{
	int		i;
	int		j;
	char	*key;

	i = 1;
	while (i < count)
	{
		key = arr[i];
		j = i - 1;
		while (j >= 0 && ft_strcmp(arr[j], key) > 0)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
		i++;
	}
}

int	copy_and_resize_matches(char ***matches, int match_count, int new_cap)
{
	char	**new_matches;
	int		i;

	new_matches = malloc(sizeof(char *) * new_cap);
	if (!new_matches)
	{
		perror("malloc failed");
		return (0);
	}
	i = 0;
	while (i < match_count)
	{
		new_matches[i] = (*matches)[i];
		i++;
	}
	free(*matches);
	*matches = new_matches;
	return (1);
}

char	*join_matches(char **matches, int match_count)
{
	int		i;
	size_t	total_len;
	char	*result;
	char	*temp;

	if (match_count <= 0)
		return (NULL);
	total_len = 0;
	i = 0;
	while (i < match_count)
	{
		total_len += ft_strlen(matches[i]);
		if (i < match_count - 1)
			total_len++;
		i++;
	}
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	temp = result;
	i = 0;
	ft_strcpy(temp, matches[i]);
	temp += ft_strlen(matches[i]);
	i++;
	while (i < match_count)
	{
		*temp++ = ' ';
		ft_strcpy(temp, matches[i]);
		temp += ft_strlen(matches[i]);
		i++;
	}
	*temp = '\0';
	return (result);
}

void	process_wildcard_node_fd(t_list_fd *node, char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*pattern;
	char			**matches;
	int				match_count;
	int				capacity;
	int				i;
	int				old_capacity;
	char			*joined_result;

	pattern = node->name;
	match_count = 0;
	capacity = 10;
	dir = opendir(dir_path);
	if (dir == NULL)
		return ;
	matches = malloc(sizeof(char *) * capacity);
	if (!matches)
	{
		closedir(dir);
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name))
		{
			if (match_count >= capacity)
			{
				old_capacity = capacity;
				if (!copy_and_resize_matches(&matches,
						match_count, capacity * 2))
				{
					entry = readdir(dir);
					continue ;
				}
				capacity = old_capacity * 2;
			}
			matches[match_count] = str_duplicate(entry->d_name);
			if (!matches[match_count])
			{
				entry = readdir(dir);
				continue ;
			}
			match_count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (match_count > 0)
	{
		sort_matches(matches, match_count);
		joined_result = join_matches(matches, match_count);
		if (joined_result)
		{
			free(node->name);
			node->name = joined_result;
			i = 0;
			while (i < match_count)
			{
				free(matches[i]);
				i++;
			}
		}
	}
	free(matches);
}

int	fd_list_size(t_list_fd **head)
{
	t_list_fd	*tmp;
	int			size;

	tmp = *head;
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}

void	wild_cards_handle_fd_list(t_list_fd **fd_list, char *dir_path)
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
	i = 0;
	while (i < wildcard_count)
	{
		process_wildcard_node_fd(wildcard_nodes[i], dir_path);
		i++;
	}
	free(wildcard_nodes);
}

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

char	**get_matches(const char *pattern, char *dir_path, int *match_count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	char			**new_matches;
	int				capacity;
	int				new_capacity;
	int				i;

	*match_count = 0;
	capacity = 10;
	dir = opendir(dir_path);
	if (dir == NULL)
		return (NULL);
	matches = malloc(sizeof(char *) * capacity);
	if (!matches)
	{
		closedir(dir);
		return (NULL);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name))
		{
			if (*match_count >= capacity)
			{
				new_capacity = capacity * 2;
				new_matches = malloc(sizeof(char *) * new_capacity);
				if (!new_matches)
				{
					i = 0;
					while (i < *match_count)
					{
						free(matches[i]);
						i++;
					}
					free(matches);
					closedir(dir);
					return (NULL);
				}
				i = 0;
				while (i < *match_count)
				{
					new_matches[i] = matches[i];
					i++;
				}
				free(matches);
				matches = new_matches;
				capacity = new_capacity;
			}
			matches[*match_count] = str_duplicate(entry->d_name);
			if (matches[*match_count])
				(*match_count)++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (*match_count > 0)
		sort_matches(matches, *match_count);
	return (matches);
}

void	wild_cards_handle_cmdarr(char ***cmd_arr, char *dir_path)
{
	int		i;
	int		j;
	int		k;
	int		original_size;
	int		new_size;
	char	**expanded_matches;
	int		match_count;
	char	**new_cmd_arr;

	if (!cmd_arr || !*cmd_arr || !dir_path)
		return ;
	original_size = 0;
	while ((*cmd_arr)[original_size])
		original_size++;
	i = 0;
	new_size = 0;
	while (i < original_size)
	{
		if (if_has_wildcards((*cmd_arr)[i]))
		{
			match_count = count_pattern_matches((*cmd_arr)[i], dir_path);
			if (match_count > 0)
				new_size += match_count;
			else
				new_size += 1;
		}
		else
			new_size++;
		i++;
	}
	new_cmd_arr = malloc(sizeof(char *) * (new_size + 1));
	if (!new_cmd_arr)
		return ;
	i = 0;
	j = 0;
	while (i < original_size)
	{
		if (if_has_wildcards((*cmd_arr)[i]))
		{
			expanded_matches = get_matches((*cmd_arr)[i],
					dir_path, &match_count);
			if (expanded_matches && match_count > 0)
			{
				k = 0;
				while (k < match_count)
				{
					new_cmd_arr[j] = expanded_matches[k];
					j++;
					k++;
				}
				free(expanded_matches);
			}
			else
			{
				if (!*expanded_matches)
					free(expanded_matches);
				new_cmd_arr[j] = ft_strdup((*cmd_arr)[i]);
				j++;
			}
		}
		else
		{
			new_cmd_arr[j] = ft_strdup((*cmd_arr)[i]);
			j++;
		}
		i++;
	}
	new_cmd_arr[j] = NULL;
	i = 0;
	while (i < original_size)
	{
		free((*cmd_arr)[i]);
		i++;
	}
	free(*cmd_arr);
	*cmd_arr = new_cmd_arr;
}

void	handle_wildcards_in_cmdarr(t_tree **tree)
{
	char	*dir_name;
	int		j;

	if ((*tree)->command_arr)
	{
		j = 0;
		while ((*tree)->command_arr[j])
		{
			if (if_has_wildcards((*tree)->command_arr[j]))
			{
				dir_name = getcwd(NULL, 0);
				if (!dir_name)
					return ;
				wild_cards_handle_cmdarr(&(*tree)->command_arr, dir_name);
				free(dir_name);
			}
			j++;
		}
	}
	if ((*tree)->left)
		has_wild_cards_comarr(&(*tree)->left);
	if ((*tree)->right)
		has_wild_cards_comarr(&(*tree)->right);
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
