/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_nested_parentheses.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:35:48 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 13:40:50 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void fix_operation_tree_structure(t_tree *tree) 
{
    if (!tree)
        return;
    if (ft_strcmp(tree->type, "OPERATION") == 0) 
    {
        if (tree->left && ft_strcmp(tree->left->type, "COMMAND") == 0) 
        {
            if (tree->left->left)
            {
                t_tree *left_cmd_node = tree->left;
                t_tree *nested_node = left_cmd_node->left;
                tree->left = nested_node;
                left_cmd_node->left = NULL;
                free_tree(left_cmd_node);
            }
        }
        fix_operation_tree_structure(tree->left);
        fix_operation_tree_structure(tree->right);
    }
    else {
        if (tree->left) 
            fix_operation_tree_structure(tree->left);
        if (tree->right) 
            fix_operation_tree_structure(tree->right);
    }
}

void process_all_parentheses(t_tree *tree) {
    if (!tree)
        return;
    if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
        char *cmd_part = NULL;
        char *redir_part = NULL;
        extract_redirections(tree->command, &cmd_part, &redir_part);
        char *inner_cmd = NULL;
        if (cmd_part && cmd_part[0] == '(') {
            inner_cmd = extract_content_from_parentheses(cmd_part);
            t_free(cmd_part, __LINE__, "process_all_parentheses");
        } else if (cmd_part) {
            inner_cmd = cmd_part;
        } else {
            inner_cmd = extract_content_from_parentheses(tree->command);
        }
        if (inner_cmd) {
            t_list *cmd_list = list_init_leak(inner_cmd, __LINE__, "process_all_parentheses");
            if (cmd_list) {
                lexer(&cmd_list);
                t_tree *subtree = NULL;
                tree_maker(&cmd_list, &subtree);
                if (subtree) {
                    process_pipe_trees(subtree);
                    t_free(tree->command, __LINE__, "parsing.c");
                    tree->command = NULL;
                    if (redir_part) {
                        tree->redirections = redir_part;
                    }
                    if (subtree->command) {
                        tree->command = subtree->command;
                        subtree->command = NULL;
                    }
                    if (subtree->type)
                        tree->type = subtree->type;
                    else
                        tree->type = "COMMAND";
                    if (tree->left)
                        free_tree(tree->left);
                    if (tree->right)
                        free_tree(tree->right);
                    tree->left = subtree->left;
                    tree->right = subtree->right;
                    subtree->left = NULL;
                    subtree->right = NULL;
                    t_free(subtree, __LINE__, "parsing.c");
                } else {
                    if (inner_cmd != cmd_part)
                        t_free(inner_cmd, __LINE__, "parsing.c");
                    if (redir_part)
                        t_free(redir_part, __LINE__, "parsing.c");
                }
            } else {
                if (inner_cmd != cmd_part)
                    t_free(inner_cmd, __LINE__, "parsing.c");
                if (redir_part)
                    t_free(redir_part, __LINE__, "parsing.c");
            }
        } else if (redir_part) {
            t_free(redir_part, __LINE__, "parsing.c");
        }
    }
    if (tree->left)
        process_all_parentheses(tree->left);
    if (tree->right)
        process_all_parentheses(tree->right);
}

void process_nested_parentheses(t_tree **tree) {
    if (!tree || !(*tree))
        return;
    if ((*tree)->left)
        process_nested_parentheses(&((*tree)->left));
    if ((*tree)->right)
        process_nested_parentheses(&((*tree)->right));
    int passes = 0;
    int max_passes = 5;
    while (passes < max_passes) {
        process_all_parentheses(*tree);
        fix_operation_tree_structure(*tree);
        passes++;
    }
    if ((*tree)->type && ft_strcmp((*tree)->type, "PIPE") == 0) {
        if ((*tree)->right && (*tree)->right->redirections && !(*tree)->redirections) {
            (*tree)->redirections = ft_strdup((*tree)->right->redirections);
            t_free((*tree)->right->redirections, __LINE__, "process_nested_parentheses");
            (*tree)->right->redirections = NULL;
        }
    }
}