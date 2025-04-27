/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:45:24 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 10:36:41 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lexer_to_tree(char *str, t_tree **tree, char **argev)
{
	t_list	*head;
	t_list	*tmp;

	head = list_init(str);
	lexer(&head);
	if (variable_search(&head))
		variable_expantion(&head, argev);
	variable_in_word(&head, argev);
	tmp = head;
	while (tmp)
	{
		printf("%s\n", tmp->data);
		printf("%s\n", tmp->token);
		tmp = tmp->next;
	}
	syntax_error(&head);
	if (syntax_error_parentheses(&head) == 1)
		exit(1);
	tree_maker(&head, tree);
}

void	tree_to_rediropen(t_tree *tree)
{
	process_nested_parentheses(&tree);
	process_pipe_trees(tree);
	process_all_redirections(&tree);
	command_arr_fill(&tree);
	quote_remove_two(&tree);
	syntax_error_two(&tree);
	print_tree_visual(tree, 1, 1);
}

void	inits_main(t_list_fd **head_fd, t_env **env, t_tree **tree)
{
	*head_fd = NULL;
	*env = NULL;
	*tree = NULL;
}

void	env_fill_quote_parse(t_env **env, char **str, char **argev)
{
	char	*tmp_str;

	*env = env_fill(argev);
	*str = replace_whites_spaces(*str);
	tmp_str = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp_str);
	if (check_quotes(*str) == 1)
		exit(1);
}
