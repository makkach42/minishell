/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_new_other_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:09:50 by makkach           #+#    #+#             */
/*   Updated: 2025/06/03 13:11:18 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ambiguous_syntax_error(t_tree **tree, t_env **env)
{
	int			i;
	t_list_fd	*tmplst;

	i = 0;
	if ((*tree) && (*tree)->quotes)
	{
		tmplst = (*tree)->fd_list;
		while (tmplst)
		{
			if (countwords(tmplst->name, 32) > 1)
				(*tree)->quotes = 0;
			tmplst = tmplst->next;
		}
	}
	if ((*tree) && (*tree)->quotes == 1)
		i = 2;
	if ((*tree) && (*tree)->ambiguous == 1)
		i = 1;
	if ((*tree) && (*tree)->left)
		i = ambiguous_syntax_error(&(*tree)->left, env);
	if ((*tree) && (*tree)->right)
		i = ambiguous_syntax_error(&(*tree)->right, env);
	return (i);
}

// int	quote_detect(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str && str[i])
// 	{
// 		if (str[i] == '"' || str[i] == '\'')
// 			break ;
// 		i++;
// 	}
// 	if (str[i])
// 		return (1);
// 	return (0);
// }

// void	quote_set(t_tree **tree)
// {
// 	if ((*tree) && (*tree)->left)
// 		quote_set(&(*tree)->left);
// 	if ((*tree) && (*tree)->right)
// 		quote_set(&(*tree)->right);
// 	if ((*tree) && (*tree)->redirections)
// 	{
// 		if (variable_search_instr((*tree)->redirections
// 			) && quote_detect((*tree)->redirections))
// 			(*tree)->quotes = 1;
// 	}
// }

// static void	empty_string_error(t_list_fd **head_fd, int *flag)
// {
// 	t_list_fd	*tmp;

// 	tmp = *head_fd;
// 	while (tmp)
// 	{
// 		if ((tmp->name[0] == '"' && tmp->name[1] == '"' && !tmp->name[2]
// 			) || (tmp->name[0] == '\'' && tmp->name[1
// 				] == '\'' && !tmp->name[2]))
// 		{
// 			write(2, "No such file or directory\n", 26);
// 			*flag = 1;
// 			break ;
// 		}
// 		tmp = tmp->next;
// 	}
// }

// void	tree_empty_error(t_tree **tree, int *flag)
// {
// 	if ((*tree) && (*tree)->left)
// 		tree_empty_error(&(*tree)->left, flag);
// 	if ((*tree) && (*tree)->right)
// 		tree_empty_error(&(*tree)->right, flag);
// 	if ((*tree) && (*tree)->fd_list)
// 	{
// 		empty_string_error(&(*tree)->fd_list, flag);
// 	}
// }
