/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_everything.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:16:58 by makkach           #+#    #+#             */
/*   Updated: 2025/04/20 09:27:02 by makkach          ###   ########.fr       */
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
			t_free(tmp->data, 1388, "parsing.c");
		t_free(tmp, 1389, "parsing.c");
		tmp = tmp2;
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
		(t_free(tree->command, __LINE__, "parsing.c"), tree->command = NULL);
	if (tree->redirections)
	{
		t_free(tree->redirections, __LINE__, "parsing.c");
		tree->redirections = NULL;
	}
	if (tree->command_arr)
	{
		i = -1;
		while (tree->command_arr[++i])
			(t_free(tree->command_arr[i], __LINE__,
					"parsing.c"), tree->command_arr[i] = NULL);
		t_free(tree->command_arr, __LINE__, "parsing.c");
		tree->command_arr = NULL;
	}
	t_free(tree, __LINE__, "parsing.c");
}

void	free_list_fd(t_list_fd **head)
{
	t_list_fd	*current;
	t_list_fd	*next;

	if (!head || !*head)
		return ;
	current = *head;
	next = NULL;
	while (current)
	{
		next = current->next;
		if (current->command)
			(t_free(current->command, 26, "parsing.c"),
				current->command = NULL);
		if (current->name)
			(t_free(current->name, 28, "parsing.c"), current->name = NULL);
		if (current->redir)
			(t_free(current->redir, 30, "parsing.c"), current->redir = NULL);
		if (current->fd > 0)
			(close(current->fd), current->fd = -1);
		t_free(current, 33, "parsing.c");
		current = next;
	}
	*head = NULL;
}

void	free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = *env;
	while (tmp)
	{
		tmp2 = tmp->next;
		t_free(tmp->key, 2462, "parsing.c");
		t_free(tmp->value, 2463, "parsing.c");
		t_free(tmp, 2464, "parsing.c");
		tmp = tmp2;
	}
	*env = NULL;
}

void	last_free(t_env **env, t_tree **tree, t_list_fd **head_fd)
{
	free_env(env);
	if (tree)
		free_tree(*tree);
	if (head_fd)
		free_list_fd(head_fd);
}
