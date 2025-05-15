/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_everything.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:16:58 by makkach           #+#    #+#             */
/*   Updated: 2025/05/15 11:53:06 by makkach          ###   ########.fr       */
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

void	free_list_fd(t_list_fd **head)
{
	t_list_fd	*current;
	t_list_fd	*next;
	int			i;

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
		{
			i = -1;
			while (current->name_split[++i])
				free(current->name_split[i]);
			(free(current->name_split), current->name_split = NULL);
		}
		if (current->fd > 0)
			(close(current->fd), current->fd = -1);
		free(current);
		current = next;
	}
	*head = NULL;
}

void free_tree(t_tree *tree)
{
    int i;
    int j;

    if (!tree)
        return;
    if (tree->left)
        free_tree(tree->left);
    if (tree->right)
        free_tree(tree->right);
    if (tree->command)
        (free(tree->command), tree->command = NULL);
    if (tree->redirections)
    {
        free(tree->redirections);
        tree->redirections = NULL;
    }
    if (tree->command_arr)
    {
        i = -1;
        while (tree->command_arr[++i])
            (free(tree->command_arr[i]), tree->command_arr[i] = NULL);
        (free(tree->command_arr), tree->command_arr = NULL);
    }
    if (tree->command_arr_expanded)
    {
        i = -1;
        while (tree->command_arr_expanded[++i])
        {
            if (tree->command_arr_expanded[i])
            {
                j = -1;
                while (tree->command_arr_expanded[i][++j])
                {
                    if (tree->command_arr_expanded[i][j])
                        (free(tree->command_arr_expanded[i][j]), tree->command_arr_expanded[i][j] = NULL);
                }
                (free(tree->command_arr_expanded[i]), tree->command_arr_expanded[i] = NULL);
            }
        }
        (free(tree->command_arr_expanded), tree->command_arr_expanded = NULL);
    }

    if (tree->split)
    {
        i = -1;
        while (tree->split[++i])
            (free(tree->split[i]), tree->split[i] = NULL);
        free(tree->split);
		tree->split = NULL;
    }
    if (tree->fd_list)
        free_list_fd(&(tree)->fd_list);
    free(tree);
}

void	free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = *env;
	while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
	*env = NULL;
}

void	lasfree(t_tree **tree)
{
	if (tree)
		free_tree(*tree);
}
