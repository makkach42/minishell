/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:45:24 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 14:45:50 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lexer_to_tree(char *str, t_tree **tree, char **argev)
{
	t_list	*head;

	head = list_init_leak(str, 3072, "main");
	lexer(&head);
	if (variable_search(&head))
		variable_expantion(&head, argev);
	variable_in_word(&head, argev);
	syntax_error(&head);
	tree_maker(&head, tree);
}

void	tree_to_rediropen(t_tree *tree)
{
	process_pipe_trees(tree);
	process_nested_parentheses(&tree);
	process_all_redirections(&tree);
	command_arr_fill(&tree);
	quote_remove_two(&tree);
	print_tree_visual(tree, 1, 1);
	syntax_error_two(&tree);
}

void	inits_main(t_list_fd **head_fd, t_env **env, t_tree **tree)
{
	*head_fd = NULL;
	*env = NULL;
	*tree = NULL;
}