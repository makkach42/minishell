/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:47:08 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 16:14:20 by makkach          ###   ########.fr       */
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

void	redirections_opener(t_tree **tree, t_list_fd **head)
{
	int			i;
	int			j;
	t_list_fd	*tmp;
	t_list_fd	*tmp2;
	t_list_fd	*new_node;
	char		*tmp_char;
	int			flag;

	i = 0;
	j = 0;
	flag = 0;
	if ((*tree)->left)
		redirections_opener(&(*tree)->left, head);
	if ((*tree)->right)
		redirections_opener(&(*tree)->right, head);
	if ((*tree)->redirections)
	{
		if (!*head)
		{
			while (((*tree)->redirections[i] != '>' && (
						*tree)->redirections[i] != '<') && (
					*tree)->redirections[i] != '\0')
				i++;
			if ((*tree)->redirections[i] == '>' && (
					(*tree)->redirections[i + 1] == ' ' || (
						*tree)->redirections[i + 1] == '\0' || (
						*tree)->redirections[i + 1] == '\t'))
				flag = 1;
			if ((*tree)->redirections[i] == '>' && (
					*tree)->redirections[i + 1] == '>')
				flag = 2;
			if ((*tree)->redirections[i] == '<' && (
					(*tree)->redirections[i + 1] == ' ' || (
						*tree)->redirections[i + 1] == '\0' || (
						*tree)->redirections[i + 1] == '\t'))
				flag = 3;
			if ((*tree)->redirections[i] == '<' && (
					*tree)->redirections[i + 1] == '<')
				flag = 4;
			while (((*tree)->redirections[i] == '>' || (
						*tree)->redirections[i] == '<') && (
					*tree)->redirections[i] != '\0')
				i++;
			while ((*tree)->redirections[i] == ' ' && (
					*tree)->redirections[i] != '\0')
				i++;
			j = i;
			while ((*tree)->redirections[i] != ' ' && (
					*tree)->redirections[i] != '\0')
				i++;
			*head = malloc(sizeof(t_list_fd));
			if (!*head)
				return ;
			(*head)->name = ft_substr(
					(*tree)->redirections, j, i - j);
			(*head)->redir = NULL;
			(*head)->next = NULL;
			(*head)->fd = -1;
			if (ft_strcmp((*tree)->type, "OPERATION") == 0)
			{
				if ((*tree)->right && (*tree)->right->command)
					(*head)->command = ft_strdup((*tree)->right->command);
				else if ((*tree)->left && (*tree)->left->command)
					(*head)->command = ft_strdup((*tree)->left->command);
				else
					(*head)->command = ft_strdup("");
			}
			else
			{
				if (ft_strcmp((*tree)->type, "PARENTHASIS") == 0)
				{
					if ((*tree)->command)
						(*head)->command = ft_strdup((*tree)->command);
					else
						(*head)->command = ft_strdup("");
				}
				else
					(*head)->command = ft_strdup((*tree)->command);
			}
			tmp_char = (*tree)->redirections;
			(*tree)->redirections = ft_substr((*tree)->redirections, i,
					ft_strlen((*tree)->redirections) - i);
			free(tmp_char);
			tmp_char = (*tree)->redirections;
			(*tree)->redirections = ft_strtrim(
					(*tree)->redirections, " ");
			free(tmp_char);
			if (flag == 1 && !check_empty((*head)->name))
			{
				(*head)->fd = -1;
				(*head)->redir = ft_strdup(">");
			}
			if (flag == 2 && !check_empty((*head)->name))
			{
				(*head)->fd = -1;
				(*head)->redir = ft_strdup(">>");
			}
			if (flag == 3)
			{
				(*head)->fd = -1;
				(*head)->redir = ft_strdup("<");
			}
			if (flag == 4)
			{
				(*head)->fd = -1;
				(*head)->redir = ft_strdup("<<");
			}
			(*head)->next = NULL;
			tmp = *head;
			while ((*tree)->redirections && !check_empty((*tree)->redirections))
			{
				i = 0;
				while ((*tree)->redirections != NULL && (
						*tree)->redirections[i] && (
						(*tree)->redirections[i] != '>' && (
							*tree)->redirections[i] != '<'))
					i++;
				if (!(*tree)->redirections[i])
					break ;
				if ((*tree)->redirections[i] == '>' && (
						(*tree)->redirections[i + 1] == ' ' || (
							*tree)->redirections[i + 1] == '\0' || (
							*tree)->redirections[i + 1] == '\t'))
					flag = 1;
				if ((*tree)->redirections[i] == '>' && (
						*tree)->redirections[i + 1] == '>')
					flag = 2;
				if ((*tree)->redirections[i] == '<' && (
						(*tree)->redirections[i + 1] == ' ' || (
							*tree)->redirections[i + 1] == '\0' || (
							*tree)->redirections[i + 1] == '\t'))
					flag = 3;
				if ((*tree)->redirections[i] == '<' && (
						*tree)->redirections[i + 1] == '<')
					flag = 4;
				while (((*tree)->redirections[i] == '>' || (
							*tree)->redirections[i] == '<') && (
						*tree)->redirections[i] != '\0')
					i++;
				while ((*tree)->redirections[i] == ' ' && (
						*tree)->redirections[i] != '\0')
					i++;
				j = i;
				while ((*tree)->redirections[i] != ' ' && (
						*tree)->redirections[i] != '\0')
					i++;
				new_node = malloc(sizeof(t_list_fd));
				if (!new_node)
					break ;
				new_node->name = ft_substr(
						(*tree)->redirections, j, i - j);
				new_node->fd = -1;
				new_node->redir = NULL;
				new_node->next = NULL;
				if (ft_strcmp((*tree)->type, "OPERATION") == 0 || ft_strcmp(
						(*tree)->type, "PARENTHASIS") == 0)
				{
					if ((*tree)->right && (*tree)->right->command)
						new_node->command = ft_strdup((*tree)->right->command);
					else if ((*tree)->left && (*tree)->left->command)
						new_node->command = ft_strdup((*tree)->left->command);
					else
						new_node->command = ft_strdup("");
				}
				else
				{
					new_node->command = ft_strdup((*tree)->command);
				}
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_substr((*tree)->redirections, i,
						ft_strlen((*tree)->redirections) - i);
				free(tmp_char);
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_strtrim(
						(*tree)->redirections, " ");
				free(tmp_char);
				if (flag == 1 && !check_empty(new_node->name))
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup(">");
				}
				if (flag == 2 && !check_empty(new_node->name))
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup(">>");
				}
				if (flag == 3)
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup("<");
				}
				if (flag == 4)
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup("<<");
				}
				new_node->next = NULL;
				tmp->next = new_node;
				tmp = new_node;
				if (check_empty((*tree)->redirections) == 1)
					break ;
			}
		}
		else
		{
			tmp2 = *head;
			while (tmp2->next)
				tmp2 = tmp2->next;
			tmp = tmp2;
			while ((*tree)->redirections && !check_empty((*tree)->redirections))
			{
				i = 0;
				while ((*tree)->redirections != NULL && (
						*tree)->redirections[i] && (
						(*tree)->redirections[i] != '>' && (
							*tree)->redirections[i] != '<'))
					i++;
				if (!(*tree)->redirections[i])
					break ;
				if ((*tree)->redirections[i] == '>' && (
						(*tree)->redirections[i + 1] == ' ' || (
							*tree)->redirections[i + 1] == '\0' || (
							*tree)->redirections[i + 1] == '\t'))
					flag = 1;
				if ((*tree)->redirections[i] == '>' && (
						*tree)->redirections[i + 1] == '>')
					flag = 2;
				if ((*tree)->redirections[i] == '<' && (
						(*tree)->redirections[i + 1] == ' ' || (
							*tree)->redirections[i + 1] == '\0' || (
							*tree)->redirections[i + 1] == '\t'))
					flag = 3;
				if ((*tree)->redirections[i] == '<' && (
						*tree)->redirections[i + 1] == '<')
					flag = 4;
				while (((*tree)->redirections[i] == '>' || (
							*tree)->redirections[i] == '<') && (
						*tree)->redirections[i] != '\0')
					i++;
				while ((*tree)->redirections[i] == ' ' && (
						*tree)->redirections[i] != '\0')
					i++;
				j = i;
				while ((*tree)->redirections[i] != ' ' && (
						*tree)->redirections[i] != '\0')
					i++;
				new_node = malloc(sizeof(t_list_fd));
				if (!new_node)
					break ;
				new_node->name = ft_substr(
						(*tree)->redirections, j, i - j);
				new_node->fd = -1;
				new_node->redir = NULL;
				new_node->next = NULL;
				if (ft_strcmp((*tree)->type, "OPERATION") == 0 || ft_strcmp(
						(*tree)->type, "PARENTHASIS") == 0)
				{
					if ((*tree)->right && (*tree)->right->command)
						new_node->command = ft_strdup((*tree)->right->command);
					else if ((*tree)->left && (*tree)->left->command)
						new_node->command = ft_strdup((*tree)->left->command);
					else
						new_node->command = ft_strdup("");
				}
				else
				{
					new_node->command = ft_strdup((*tree)->command);
				}
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_substr((*tree)->redirections, i,
						ft_strlen((*tree)->redirections) - i);
				free(tmp_char);
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_strtrim(
						(*tree)->redirections, " ");
				free(tmp_char);
				if (flag == 1 && !check_empty(new_node->name))
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup(">");
				}
				if (flag == 2 && !check_empty(new_node->name))
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup(">>");
				}
				if (flag == 3)
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup("<");
				}
				if (flag == 4)
				{
					new_node->fd = -1;
					new_node->redir = ft_strdup("<<");
				}
				new_node->next = NULL;
				tmp->next = new_node;
				tmp = new_node;
				if (check_empty((*tree)->redirections) == 1)
					break ;
			}
		}
	}
}
