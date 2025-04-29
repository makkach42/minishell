/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:47:08 by makkach           #+#    #+#             */
/*   Updated: 2025/04/29 14:46:52 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirections_list_maker(t_tree **tree)
{
    char        *old_redirs;
    char        *tmp_char;
    int         i;
    t_list_fd   *head;
    t_list_fd   *new_node;
    t_list_fd   *tmp;

    if (!tree || !*tree)
        return;
    if ((*tree)->left)
        redirections_list_maker(&(*tree)->left);
    if ((*tree)->right)
        redirections_list_maker(&(*tree)->right);
    if ((*tree)->redirections)
    {
        i = 0;
        old_redirs = ft_strdup((*tree)->redirections);
        head = malloc(sizeof(t_list_fd));
        if (!head)
            return;
        head->name = NULL;
        head->command = NULL;
        head->redir = NULL;
        head->next = NULL;
        head->fd = -1;
        while (old_redirs[i] && (old_redirs[i] == '>' || old_redirs[i] == '<' || old_redirs[i] == 32))
            i++;
        head->redir = ft_substr(old_redirs, 0, i);
        tmp_char = head->redir;
        head->redir = ft_strtrim(head->redir, " ");
        free(tmp_char);
        tmp_char = old_redirs;
        old_redirs = ft_substr(old_redirs, i, (int)ft_strlen(old_redirs) - i);
        free(tmp_char);
        i = 0;
        while (old_redirs[i] && old_redirs[i] == 32)
            i++;
        while (old_redirs[i] && old_redirs[i] != 32)
            i++;
        head->name = ft_substr(old_redirs, 0, i);
        tmp_char = old_redirs;
        old_redirs = ft_substr(old_redirs, i, (int)ft_strlen(old_redirs) - i);
        free(tmp_char);
        head->command = ft_strdup((*tree)->command);
        tmp = head;
        while (old_redirs && !check_empty(old_redirs))
        {
            i = 0;
            new_node = malloc(sizeof(t_list_fd));
            if (!new_node)
                break;
            new_node->name = NULL;
            new_node->command = NULL;
            new_node->redir = NULL;
            new_node->next = NULL;
            new_node->fd = -1;
            while (old_redirs[i] && (old_redirs[i] == '>' || old_redirs[i] == '<' || old_redirs[i] == 32))
                i++;
            new_node->redir = ft_substr(old_redirs, 0, i);
            tmp_char = new_node->redir;
            new_node->redir = ft_strtrim(new_node->redir, " ");
            free(tmp_char);
            tmp_char = old_redirs;
            old_redirs = ft_substr(old_redirs, i, (int)ft_strlen(old_redirs) - i);
            free(tmp_char);
            i = 0;
            while (old_redirs[i] && old_redirs[i] == 32)
                i++;
            while (old_redirs[i] && old_redirs[i] != 32)
                i++;
            new_node->name = ft_substr(old_redirs, 0, i);
            tmp_char = old_redirs;
            old_redirs = ft_substr(old_redirs, i, (int)ft_strlen(old_redirs) - i);
            free(tmp_char);
            new_node->command = ft_strdup((*tree)->command);
            tmp->next = new_node;
            tmp = new_node;
        }
        if (old_redirs)
            free(old_redirs);
            
        (*tree)->fd_list = head;
    }
}