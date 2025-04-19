/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 23:17:43 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_arr_fill_helper(t_tree **tree, char **str)
{
	t_list	*head;
	char	**arr;
	t_list	*tmp;
	int		i;
	int		list_size;

	i = 0;
	head = list_init_leak(*str, 2952, "command_arr_fill");
	list_size = lst_size(&head);
	arr = t_malloc(sizeof(char *) * (list_size + 1), 2404, "parsing.c");
	tmp = head;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->data);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	free_list(&head);
	(*tree)->command_arr = arr;
}

void	command_arr_fill(t_tree **tree)
{
	char	*str;
	char	*tmp;
	char	**arr;

	if ((*tree)->left)
		command_arr_fill(&(*tree)->left);
	if ((*tree)->right)
		command_arr_fill(&(*tree)->right);
	if ((tree) && (*tree) && (*tree)->command && !(*tree)->command_arr)
	{
		str = ft_strdup((*tree)->command);
		if (!str || !*str)
		{
			arr = t_malloc(sizeof(char *), 2395, "parsing.c");
			if (arr)
				arr[0] = NULL;
			(*tree)->command_arr = arr;
			t_free(str, 2399, "parsing.c");
			return ;
		}
		tmp = str;
		str = ft_strtrim(str, " ");
		free(tmp);
		command_arr_fill_helper(tree, &str);
	}
}

void	env_fill_helper(t_env **node, int *i, int *j, char **argev)
{
	*node = t_malloc(sizeof(t_env), 2428, "parsing.c");
	while (argev[*i] && argev[*i][*j] != '=')
		(*j)++;
	(*node)->key = ft_substr_leak(argev[*i], 0, *j, 2711);
	(*node)->value = ft_substr_leak(argev[*i],
			(*j + 1), ft_strlen(argev[*i]) - (*j + 1), 2712);
	*j = 0;
}

t_env	*env_fill(char **argev)
{
	int		i;
	int		j;
	t_env	*head;
	t_env	*new_node;
	t_env	*tmp;

	i = 0;
	j = 0;
	env_fill_helper(&head, &i, &j, argev);
	head->next = NULL;
	tmp = head;
	i++;
	while (argev[i])
	{
		env_fill_helper(&new_node, &i, &j, argev);
		new_node->next = NULL;
		tmp->next = new_node;
		tmp = new_node;
		i++;
	}
	return (head);
}

void	f()
{
	system("leaks -q minishell");
}

int	main(int argc, char **argv, char **argev)
{
	atexit(f);
	char		*str;
	t_env		*env;
	t_tree		*tree;
	t_list_fd	*head_fd;

	(void)argc;
	(void)argv;
	inits_main(&head_fd, &env, &tree);
	while (1)
	{
		str = readline("minishell$> ");
		if (!str)
			break ;
		else if (!*str)
		{
			free(str);
			continue ;
		}
		add_history(str);
		env_fill_quote_parse(&env, &str, argev);
		lexer_to_tree(str, &tree, argev);
		tree_to_rediropen(tree);
		// redirections_opener(&tree, &head_fd);
		last_free(&env, &tree, &head_fd);
	}
}
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt
//((ls > file) && echo | la > file2) > file3