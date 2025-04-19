/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_everything.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:16:58 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 11:18:39 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_tree(t_tree *tree) {
    int i;
    if (!tree)
        return;
    if (tree->left)
        free_tree(tree->left);
    if (tree->right)
        free_tree(tree->right);
    if (tree->command) {
        t_free(tree->command, __LINE__, "parsing.c");
        tree->command = NULL;  
    }
    if (tree->redirections) {
        t_free(tree->redirections, __LINE__, "parsing.c");
        tree->redirections = NULL;
    }
    if (tree->command_arr) {
        i = 0;
        while (tree->command_arr[i]) {
            t_free(tree->command_arr[i], __LINE__, "parsing.c");
            tree->command_arr[i] = NULL;
            i++;
        }
        t_free(tree->command_arr, __LINE__, "parsing.c");
        tree->command_arr = NULL;
    }
    t_free(tree, __LINE__, "parsing.c");
}

void free_list(t_list **head)
{
    if (!head || !*head)
        return;
    t_list *tmp;
    t_list *tmp2;
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