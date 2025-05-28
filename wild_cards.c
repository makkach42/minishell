/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:13:43 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 15:14:12 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// int	has_wild_cards_fdlst(t_tree **tree)
// {
// 	int			i;
// 	t_list_fd	*tmp;

// 	i = 0;
// 	if ((*tree)->fd_list)
// 	{
// 		tmp = (*tree)->fd_list;
// 		while (tmp)
// 		{
// 			if (if_has_wildcards(tmp->name))
// 				i = 1;
// 			tmp = tmp->next;
// 		}
// 	}
// 	if ((*tree)->left)
// 		i = has_wild_cards_fdlst(&(*tree)->left);
// 	if ((*tree)->right)
// 		i = has_wild_cards_fdlst(&(*tree)->right);
// 	return (i);
// }

// void	wild_cards_handle_fd_list(t_list_fd **fd_list, char *dir_path)
// {
// 	t_list_fd	*node;
// 	t_list_fd	**wildcard_nodes;
// 	int			wildcard_count;
// 	int			i;

// 	if (!fd_list || !*fd_list || !dir_path)
// 		return ;
// 	wildcard_count = 0;
// 	node = *fd_list;
// 	wildcard_nodes = malloc(sizeof(t_list_fd *) * (fd_list_size(fd_list) + 1));
// 	while (node && wildcard_count < fd_list_size(fd_list))
// 	{
// 		if (if_has_wildcards(node->name))
// 		{
// 			wildcard_nodes[wildcard_count] = node;
// 			wildcard_count++;
// 		}
// 		node = node->next;
// 	}
// 	i = 0;
// 	while (i < wildcard_count)
// 	{
// 		process_wildcard_node_fd(wildcard_nodes[i], dir_path);
// 		i++;
// 	}
// 	free(wildcard_nodes);
// }
int	init_matches(char ***matches, DIR **dir,
			char *dir_path, int *capacity)
{
	*capacity = 10;
	*dir = opendir(dir_path);
	if (*dir == NULL)
		return (0);
	*matches = malloc(sizeof(char *) * *capacity);
	if (!*matches)
	{
		closedir(*dir);
		return (0);
	}
	return (1);
}

void	export_case_wild_card(t_tree **tree)
{
	char	*dir_name;
	int		j;

	j = 1;
	while ((*tree)->command_arr[j])
	{
		if (if_has_wildcards((*tree)->command_arr[j]))
		{
			dir_name = getcwd(NULL, 0);
			if (!dir_name)
				return ;
			wild_cards_handle_cmdarr(&(*tree)->command_arr, dir_name);
			free(dir_name);
			break ;
		}
		j++;
	}
}

void	normale_wild_card_case(t_tree **tree)
{
	int		j;
	char	*dir_name;

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
			break ;
		}
		j++;
	}
}

void	handle_wildcards_in_cmdarr(t_tree **tree)
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->command_arr && (*tree)->command_arr[0
		] && ft_strcmp((*tree
			)->command_arr[0], "export") == 0)
		export_case_wild_card(tree);
	else if ((*tree)->command_arr)
		normale_wild_card_case(tree);
	if ((*tree)->left)
		handle_wildcards_in_cmdarr(&(*tree)->left);
	if ((*tree)->right)
		handle_wildcards_in_cmdarr(&(*tree)->right);
}

// void	handle_wildcards_in_fdlst(t_tree **tree)
// {
// 	t_list_fd	*tmp;
// 	char		*dir_name;

// 	if ((*tree)->fd_list)
// 	{
// 		tmp = (*tree)->fd_list;
// 		while (tmp)
// 		{
// 			if (if_has_wildcards(tmp->name))
// 			{
// 				dir_name = getcwd(NULL, 0);
// 				if (!dir_name)
// 					return ;
// 				wild_cards_handle_fd_list(&(*tree)->fd_list, dir_name);
// 				free(dir_name);
// 			}
// 			tmp = tmp->next;
// 		}
// 	}
// 	if ((*tree)->left)
// 		has_wild_cards_fdlst(&(*tree)->left);
// 	if ((*tree)->right)
// 		has_wild_cards_fdlst(&(*tree)->right);
// }
