/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 14:45:44 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_arr_fill(t_tree **tree)
{
	t_list	*head;
	char	*str;
	char	**arr;
	int		i;
	int	list_size;
	t_list	*tmp;
	if ((*tree)->left)
		command_arr_fill(&(*tree)->left);
	if ((*tree)->right)
		command_arr_fill(&(*tree)->right);
	if ((tree) && (*tree) && (*tree)->command && !(*tree)->command_arr)
	{
		i = 0;
		str = ft_strdup((*tree)->command);
		if (!str || !*str)
        {
            arr = t_malloc(sizeof(char *), 2395, "parsing.c");
            if (arr)
                arr[0] = NULL;
            (*tree)->command_arr = arr;
            t_free(str, 2399, "parsing.c");
            return;
        }
		head = list_init_leak(str, 2952, "command_arr_fill");
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
}
t_env	*env_fill(char **argev)
{
	int i;
	int j;
	t_env	*head;
	t_env	*new_node;
	t_env	*tmp;
	i = 0;
	j = 0;
	head = t_malloc(sizeof(t_env), 2428, "parsing.c");
	while (argev[i] && argev[i][j] != '=')
		j++;
	head->key = ft_substr_leak(argev[i],0, j, 2711);
	head->value = ft_substr_leak(argev[i], (j + 1), ft_strlen(argev[i]) - (j + 1), 2712);
	j = 0;
	head->next = NULL;
	tmp = head;
	i++;
	while (argev[i])
	{
		new_node = t_malloc(sizeof(t_env), 2439, "parsing.c");
		while (argev[i] && argev[i][j] != '=')
			j++;
		new_node->key = ft_substr_leak(argev[i],0, j, 2722);
		new_node->value = ft_substr_leak(argev[i], (j + 1), ft_strlen(argev[i]) - (j + 1), 2723);
		j = 0;
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

int main(int argc, char **argv, char **argev)
{
	atexit(f);
	char *str;
	char *tmp_str;
	t_env *env;
	t_tree *tree;
	t_list_fd *head_fd;

	(void)argc;
	(void)argv;
	inits_main(&head_fd, &env, &tree);
	while (1)
	{
		str = readline("minishell$> ");
		if (!str)
			break;
		else if (!*str)
		{
			free(str);
			continue ;
		}
		add_history(str);
		env = env_fill(argev);
		str = replace_whites_spaces(str);
		tmp_str = str;
		str = ft_strtrim_leak(str, " ", 2509, "main");
		t_free(tmp_str, 2514, "parsing.c");
		if (check_quotes(str) == 1)
			return (1);
		lexer_to_tree(str, &tree, argev);
		tree_to_rediropen(tree);
		redirections_opener(&tree, &head_fd);
		free_env(&env);
		if (tree)
			free_tree(tree);
		if (head_fd)
			free_list_fd(&head_fd);
	}
}
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt