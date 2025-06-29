/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:37:04 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/29 10:13:45 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	command_arr_fill_helper(t_tree **tree, char **str)
{
	t_list	*head;
	char	**arr;
	t_list	*tmp;
	int		i;
	int		list_size;

	i = 0;
	head = list_init(*str);
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

	if ((*tree) && (*tree)->left)
		command_arr_fill(&(*tree)->left);
	if ((*tree) && (*tree)->right)
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
	if (!*node)
		return ;
	while (argev[*i] && argev[*i][*j] != '=')
		(*j)++;
	(*node)->key = ft_substr(argev[*i], 0, *j);
	if (!(*node)->key)
		return ;
	(*node)->value = ft_substr(argev[*i],
			(*j + 1), ft_strlen(argev[*i]) - (*j + 1));
	if (!(*node)->value)
		return ;
	*j = 0;
}

void	env_fill_empty_helper(t_env **node)
{
	if (node == NULL)
		exit (1);
	*node = NULL;
	ft_lstadd_back(node, ft_lstnew("OLDPWD", NULL));
	ft_lstadd_back(node, ft_lstnew("PWD", getcwd(NULL, 0)));
	ft_lstadd_back(node, ft_lstnew("SHLVL", "1"));
	ft_lstadd_back(node, ft_lstnew("PATH",
			"/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
	ft_lstadd_back(node, ft_lstnew("1PWD", NULL));
}

void	env_fill_empty(t_env **node)
{
	t_env	*tmp;

	env_fill_empty_helper(node);
	tmp = *node;
	while (tmp->next)
	{
		tmp->h = 0;
		tmp->active = 0;
		tmp = tmp->next;
	}
	tmp = ft_check(*node, "OLDPWD");
	if (tmp)
		tmp->active = 1;
	tmp = ft_check(*node, "1PWD");
	if (tmp)
	{
		tmp->active = 1;
		tmp->h = 1;
	}
}
