/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_everything.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:16:58 by makkach           #+#    #+#             */
/*   Updated: 2025/06/27 11:19:13 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_list **head)
{
	t_list	*tmp;
	t_list	*tmp2;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->data)
			free(tmp->data);
		free(tmp);
		tmp = tmp2;
	}
	*head = NULL;
}

void	name_split(t_list_fd **current)
{
	int	i;

	i = -1;
	while ((*current)->name_split[++i])
		free((*current)->name_split[i]);
	(free((*current)->name_split), (*current)->name_split = NULL);
}

void	free_list_fd(t_list_fd **head)
{
	t_list_fd	*current;
	t_list_fd	*next;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		next = current->next;
		if (current->command)
			(free(current->command), current->command = NULL);
		if (current->name)
			(free(current->name), current->name = NULL);
		if (current->redir)
			(free(current->redir), current->redir = NULL);
		if (current->name_split)
			name_split(&current);
		if (current->fd > 0)
			(close(current->fd), current->fd = -1);
		free(current);
		current = next;
	}
	*head = NULL;
}

void	free_tree(t_tree *tree)
{
	int	i;

	if (!tree)
		return ;
	if (tree->left)
		free_tree(tree->left);
	if (tree->right)
		free_tree(tree->right);
	if (tree->command)
		(free(tree->command), tree->command = NULL);
	if (tree->redirections)
		(free(tree->redirections), tree->redirections = NULL);
	if (tree->command_arr)
	{
		i = -1;
		while (tree->command_arr[++i])
			(free(tree->command_arr[i]), tree->command_arr[i] = NULL);
		(free(tree->command_arr), tree->command_arr = NULL);
	}
	if (tree->fd_list)
		free_list_fd(&(tree)->fd_list);
	free(tree);
}
