/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:47:08 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 11:13:41 by makkach          ###   ########.fr       */
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

static int	determine_redirection_flag(char *redirection, int i)
{
	if (redirection[i] == '>' && (redirection[i + 1] == ' ' || 
		redirection[i + 1] == '\0' || redirection[i + 1] == '\t'))
		return (1);
	else if (redirection[i] == '>' && redirection[i + 1] == '>')
		return (2);
	else if (redirection[i] == '<' && (redirection[i + 1] == ' ' || 
		redirection[i + 1] == '\0' || redirection[i + 1] == '\t'))
		return (3);
	else if (redirection[i] == '<' && redirection[i + 1] == '<')
		return (4);
	return (0);
}

static void	skip_redirection_and_spaces(char *redirection, int *i)
{
	while ((redirection[*i] == '>' || redirection[*i] == '<')
		&& redirection[*i] != '\0')
		(*i)++;
	while (redirection[*i] == ' ' && redirection[*i] != '\0')
		(*i)++;
}

static char	*extract_filename(char *redirection, int *i)
{
	int	start;

	start = *i;
	while (redirection[*i] != ' ' && redirection[*i] != '\0')
		(*i)++;
	return (ft_substr(redirection, start, *i - start));
}

static void	set_command_for_node(t_tree *tree, t_list_fd *new_node)
{
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
}

static void	set_redirection_type(t_list_fd *new_node, int flag)
{
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
}

static t_list_fd	*process_single_redirection(t_tree *tree,
	char *redirection, int *pos)
{
	t_list_fd	*new_node;
	int			i;
	int			flag;

	i = *pos;
	flag = determine_redirection_flag(redirection, i);
	skip_redirection_and_spaces(redirection, &i);
	new_node = malloc(sizeof(t_list_fd));
	if (!new_node)
		return (NULL);
	new_node->name = extract_filename(redirection, &i);
	new_node->redir = NULL;
	new_node->next = NULL;
	new_node->fd = -1;
	set_command_for_node(tree, new_node);
	set_redirection_type(new_node, flag);
	*pos = i;
	return (new_node);
}

static int	skip_to_next_redirection(char *redirections, int i)
{
	while (redirections[i] && redirections[i] != '>' 
		&& redirections[i] != '<')
		i++;
	return (i);
}

static int	initialize_redirection_processing(t_tree *tree, 
				char **redirections_copy)
{
	if (!tree->redirections || check_empty(tree->redirections))
		return (0);
	*redirections_copy = ft_strdup(tree->redirections);
	if (!*redirections_copy)
		return (0);
	return (1);
}

static int	process_first_redirection(t_tree *tree, t_list_fd **head, 
				char *redirections_copy, int *i)
{
	*head = process_single_redirection(tree, redirections_copy, i);
	if (!*head)
	{
		free(redirections_copy);
		return (0);
	}
	return (1);
}

static void	append_new_redirection(t_list_fd **tmp, t_tree *tree,
				char *redirections_copy, int *i)
{
	t_list_fd	*new_node;

	new_node = process_single_redirection(tree, redirections_copy, i);
	if (new_node)
	{
		(*tmp)->next = new_node;
		*tmp = new_node;
	}
}

static void	process_remaining_redirections(t_tree *tree, t_list_fd **tmp,
				char *redirections_copy, int *i)
{
	t_list_fd	*new_node;

	while (!check_empty(redirections_copy + *i))
	{
		while (redirections_copy[*i] == ' ')
			(*i)++;
		*i = skip_to_next_redirection(redirections_copy, *i);
		if (!redirections_copy[*i])
			break ;
		new_node = process_single_redirection(tree, redirections_copy, i);
		if (new_node)
		{
			(*tmp)->next = new_node;
			*tmp = new_node;
		}
		else
			break ;
	}
}

static void	process_first_redirection_node(t_tree *tree, t_list_fd **head)
{
	char		*redirections_copy;
	int			i;
	t_list_fd	*tmp;

	if (!initialize_redirection_processing(tree, &redirections_copy))
		return ;
	i = skip_to_next_redirection(redirections_copy, 0);
	if (!redirections_copy[i])
	{
		free(redirections_copy);
		return ;
	}
	if (!process_first_redirection(tree, head, redirections_copy, &i))
		return ;
	tmp = *head;
	process_remaining_redirections(tree, &tmp, redirections_copy, &i);
	free(redirections_copy);
}

static void	process_additional_redirection_node(t_tree *tree, t_list_fd **head)
{
	char		*redirections_copy;
	int			i;
	t_list_fd	*tmp;

	if (!initialize_redirection_processing(tree, &redirections_copy))
		return ;
	i = skip_to_next_redirection(redirections_copy, 0);
	if (!redirections_copy[i])
	{
		free(redirections_copy);
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	append_new_redirection(&tmp, tree, redirections_copy, &i);
	process_remaining_redirections(tree, &tmp, redirections_copy, &i);
	free(redirections_copy);
}

static void	process_node_redirections(t_tree *tree, t_list_fd **head)
{
	if (!*head)
		process_first_redirection_node(tree, head);
	else
		process_additional_redirection_node(tree, head);
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
