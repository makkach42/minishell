/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 16:13:27 by makkach          ###   ########.fr       */
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
	head = list_init_leak(*str, __LINE__, "command_arr_fill");
	list_size = lst_size(&head);
	arr = malloc(sizeof(char *) * (list_size + 1));
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
			arr = malloc(sizeof(char *));
			if (arr)
				arr[0] = NULL;
			(*tree)->command_arr = arr;
			free(str);
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
	*node = malloc(sizeof(t_env));
	while (argev[*i] && argev[*i][*j] != '=')
		(*j)++;
	(*node)->key = ft_substr(argev[*i], 0, *j);
	(*node)->value = ft_substr(argev[*i],
			(*j + 1), ft_strlen(argev[*i]) - (*j + 1));
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

void	f(void)
{
	system("leaks -q minishell");
}

int	main(int argc, char **argv, char **argev)
{
	char		*str;
	t_env		*env;
	t_tree		*tree;
	t_list_fd	*head_fd;
	t_list_fd	*tmp;

	atexit(f);
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
		redirections_opener(&tree, &head_fd);
		tmp = head_fd;
		while (tmp)
		{
			printf("command = %s\n", tmp->command);
			printf("name = %s\n", tmp->name);
			printf("redir = %s\n", tmp->redir);
			printf("fd = %d\n", tmp->fd);
			printf("\n");
			tmp = tmp->next;
		}
		lasfree(&env, &tree, &head_fd);
	}
}
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt
//((ls > file) && echo | la > file2) > file3

//((ls)>file2) > file
// "((ls)>file2) > file"