/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:45:24 by makkach           #+#    #+#             */
/*   Updated: 2025/05/09 17:23:08 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_search_instr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	if (str[i])
		return (1);
	return (0);
}

void	lexer_to_tree(char *str, t_tree **tree)
{
	t_list	*head;
	t_list	*tmp;

	head = list_init(str);
	lexer(&head);
	if (new_syntax_error(&head))
		(print_syntax_error("("));
	tmp = head;
	while (tmp)
	{
		printf("%s\n", tmp->data);
		printf("%s\n", tmp->token);
		printf("\n");
		tmp = tmp->next;
	}
	syntax_error(&head);
	syntax_error_parentheses(&head);
	tree_maker(&head, tree);
}

void	tree_to_rediropen(t_tree *tree)
{
	process_nested_parentheses(&tree);
	process_pipe_trees(tree);
	process_all_redirections(&tree);
	command_arr_fill(&tree);
	quote_set(&tree);
	syntax_error_two(&tree);
}

void	inits_main(t_env **env,
		t_tree **tree, char **argev)
{
	*env = env_fill(argev);
	*tree = NULL;
}

void	quote_parse(char **str)
{
	char	*tmp_str;

	*str = replace_whites_spaces(*str);
	tmp_str = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp_str);
	check_quotes(*str);
}
