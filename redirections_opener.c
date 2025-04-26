/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:47:08 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:27:30 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32)
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

static t_list_fd	*process_single_redirection(t_tree *tree,
	char *redirection, int *pos)
{
	int			i;
	int			j;
	int			flag;
	t_list_fd	*new_node;

	i = *pos;
	flag = 0;
	if (redirection[i] == '>' && (redirection[i + 1] == ' ' || redirection[
				i + 1] == '\0' || redirection[i + 1] == '\t'))
		flag = 1;
	else if (redirection[i] == '>' && redirection[i + 1] == '>')
		flag = 2;
	else if (redirection[i] == '<' && (redirection[i + 1] == ' ' || redirection[
				i + 1] == '\0' || redirection[i + 1] == '\t'))
		flag = 3;
	else if (redirection[i] == '<' && redirection[i + 1] == '<')
		flag = 4;
	while ((redirection[i] == '>' || redirection[i] == '<'
		) && redirection[i] != '\0')
		i++;
	while (redirection[i] == ' ' && redirection[i] != '\0')
		i++;
	j = i;
	while (redirection[i] != ' ' && redirection[i] != '\0')
		i++;
	*pos = i;
	new_node = malloc(sizeof(t_list_fd));
	if (!new_node)
		return (NULL);
	new_node->name = ft_substr(redirection, j, i - j);
	new_node->redir = NULL;
	new_node->next = NULL;
	new_node->fd = -1;
	if (ft_strcmp(tree->type, "OPERATION") == 0)
	{
		if (tree->right && tree->right->command)
			new_node->command = ft_strdup(tree->right->command);
		else if (tree->left && tree->left->command)
			new_node->command = ft_strdup(tree->left->command);
		else
			new_node->command = ft_strdup("");
	}
	else if (ft_strcmp(tree->type, "PARENTHASIS") == 0)
	{
		if (tree->command)
			new_node->command = ft_strdup(tree->command);
		else
			new_node->command = ft_strdup("");
	}
	else
	{
		new_node->command = ft_strdup(tree->command);
	}
	if (flag == 1 && !check_empty(new_node->name))
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup(">");
	}
	else if (flag == 2 && !check_empty(new_node->name))
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup(">>");
	}
	else if (flag == 3)
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup("<");
	}
	else if (flag == 4)
	{
		new_node->fd = -1;
		new_node->redir = ft_strdup("<<");
	}
	return (new_node);
}

static void	process_node_redirections(t_tree *tree, t_list_fd **head)
{
	int			i;
	t_list_fd	*tmp;
	t_list_fd	*new_node;
	char		*redirections_copy;

	if (!tree->redirections || check_empty(tree->redirections))
		return ;
	redirections_copy = ft_strdup(tree->redirections);
	if (!redirections_copy)
		return ;
	i = 0;
	while (redirections_copy[i] && redirections_copy[i
		] != '>' && redirections_copy[i] != '<')
		i++;
	if (!redirections_copy[i])
	{
		free(redirections_copy);
		return ;
	}
	if (!*head)
	{
		*head = process_single_redirection(tree, redirections_copy, &i);
		if (!*head)
		{
			free(redirections_copy);
			return ;
		}
		tmp = *head;
	}
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		new_node = process_single_redirection(tree, redirections_copy, &i);
		if (new_node)
		{
			tmp->next = new_node;
			tmp = new_node;
		}
	}
	while (!check_empty(redirections_copy + i))
	{
		while (redirections_copy[i] == ' ')
			i++;
		while (redirections_copy[i] && redirections_copy[i
			] != '>' && redirections_copy[i] != '<')
			i++;
		if (!redirections_copy[i])
			break ;
		new_node = process_single_redirection(tree, redirections_copy, &i);
		if (new_node)
		{
			tmp->next = new_node;
			tmp = new_node;
		}
		else
			break ;
	}
	free(redirections_copy);
}

void	redirections_opener(t_tree **tree, t_list_fd **head)
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		redirections_opener(&(*tree)->left, head);
	if ((*tree)->right)
		redirections_opener(&(*tree)->right, head);
	if ((*tree)->redirections)
		process_node_redirections(*tree, head);
}
