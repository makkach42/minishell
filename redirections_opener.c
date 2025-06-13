/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:47:08 by makkach           #+#    #+#             */
/*   Updated: 2025/06/13 11:51:11 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	extract_redir_token(char **old_redirs, char **target_redir)
{
	int		i;
	char	*tmp_char;

	i = 0;
	while ((*old_redirs)[i] && ((*old_redirs)[i] == '>' || (
			*old_redirs)[i] == '<' || (*old_redirs)[i] == 32))
		i++;
	*target_redir = ft_substr(*old_redirs, 0, i);
	if (!*target_redir)
		return ;
	tmp_char = *target_redir;
	*target_redir = ft_strtrim(*target_redir, " ");
	if (!*target_redir)
		return ;
	free(tmp_char);
	tmp_char = *old_redirs;
	*old_redirs = ft_substr(*old_redirs, i, (int)ft_strlen(*old_redirs) - i);
	if (!*old_redirs)
		return ;
	free(tmp_char);
}

static void	extract_filename(char **old_redirs, char **target_name)
{
	int		i;
	char	*tmp_char;

	i = 0;
	while ((*old_redirs)[i] && (*old_redirs)[i] == 32)
		i++;
	quote_check_filename(old_redirs, &i);
	*target_name = ft_substr(*old_redirs, 0, i);
	if (!*target_name)
		return ;
	tmp_char = *old_redirs;
	*old_redirs = ft_substr(*old_redirs, i, (int)ft_strlen(*old_redirs) - i);
	if (!*old_redirs)
		return ;
	free(tmp_char);
}

static t_list_fd	*create_redir_node(char **old_redirs, char *command)
{
	t_list_fd	*node;

	node = malloc(sizeof(t_list_fd));
	if (!node)
		return (NULL);
	init_list_fd_node(node);
	extract_redir_token(old_redirs, &node->redir);
	// printf("-------------%s\n", node->redir);
	extract_filename(old_redirs, &node->name);
	// printf("-------------%s\n", node->name);
	if (command)
		node->command = ft_strdup(command);
	else
		node->command = ft_strdup("");
	return (node);
}

static void	process_tree_redirections(t_tree *tree, char *old_redirs)
{
	t_list_fd	*head;
	t_list_fd	*new_node;
	t_list_fd	*tmp;

	head = create_redir_node(&old_redirs, tree->command);
	if (!head)
		return ;
	tmp = head;
	while (old_redirs && !check_empty(old_redirs))
	{
		new_node = create_redir_node(&old_redirs, tree->command);
		if (!new_node)
			break ;
		tmp->next = new_node;
		tmp = new_node;
	}
	if (old_redirs)
		free(old_redirs);
	tree->fd_list = head;
}

void	redirections_list_maker(t_tree **tree)
{
	char	*old_redirs;

	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		redirections_list_maker(&(*tree)->left);
	if ((*tree)->right)
		redirections_list_maker(&(*tree)->right);
	if ((*tree)->redirections)
	{
		old_redirs = ft_strdup((*tree)->redirections);
		process_tree_redirections(*tree, old_redirs);
	}
}
