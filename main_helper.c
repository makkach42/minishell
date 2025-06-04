/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:45:24 by makkach           #+#    #+#             */
/*   Updated: 2025/06/04 11:09:50 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_search_instr(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	if (str && str[i])
		return (1);
	return (0);
}

void	lexer_to_tree(char *str, t_tree **tree, int *flag)
{
	t_list	*head;
	t_list	*tmp;

	head = list_init(str);
	lexer(&head);
	if (new_syntax_error(&head))
		(print_syntax_error("("), *flag = 1);
	// tmp = head;
	// while (tmp)
	// {
	// 	printf("%s\n", tmp->data);
	// 	printf("%s\n", tmp->token);
	// 	printf("\n");
	// 	tmp = tmp->next;
	// }
	// dprintf(2, "this is stat: %d\n", (*tree)->status);
	syntax_error(&head, flag);
	if (syntax_error_parentheses(&head))
		*flag = 1;
	if (!*flag)
		tree_maker(&head, tree);
	else
		free_list(&head);
}

void	tree_to_rediropen(t_tree *tree, int *flag)
{
	if (!*flag)
	{
		process_nested_parentheses(&tree);
		process_pipe_trees(tree);
		process_all_redirections(&tree);
		command_arr_fill(&tree);
		syntax_error_two(&tree, flag);
	}
}

void	inits_main(t_env **env,
		t_tree **tree, char **argev)
{
	*env = env_fill(argev);
	*tree = NULL;
}

void	quote_parse(char **str, int *flag)
{
	char	*tmp_str;

	*str = replace_whites_spaces(*str);
	tmp_str = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp_str);
	check_quotes(*str, flag);
}
