/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_new_other_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:09:50 by makkach           #+#    #+#             */
/*   Updated: 2025/06/24 18:10:32 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ambiguous_syntax_error(t_tree **tree, t_env **env)
{
	int			i;

	i = 0;
	if ((*tree) && (*tree)->ambiguous == 1)
		i = 1;
	if ((*tree) && (*tree)->left)
		i = ambiguous_syntax_error(&(*tree)->left, env);
	if ((*tree) && (*tree)->right)
		i = ambiguous_syntax_error(&(*tree)->right, env);
	return (i);
}

void	if_question_mark_two(char **line, int status, int n)
{
	char	*before;
	char	*after;
	char	*num;
	char	*new_str;

	before = ft_substr((*line), 0, n);
	n++;
	after = ft_substr((*line), n + 1,
			ft_strlen((*line)) - (n + 1));
	num = ft_itoa(status);
	new_str = ft_strjoin_three(before, num, after);
	free((*line));
	(*line) = new_str;
	free(before);
	free(after);
	free(num);
}

int	if_has_wildcards(char *str)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		if (!in_quotes && str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

int	first_return_check(char **command_arr, int arr_idx, int *var_pos)
{
	if (!command_arr || !command_arr[arr_idx
		] || (*var_pos) < 0 || !command_arr[
			arr_idx][(*var_pos)])
		return (1);
	return (0);
}

int	process_array_variable(char **command_arr,
		int arr_idx, int *var_pos, t_env **env)
{
	if (first_return_check(command_arr, arr_idx, var_pos))
		return (-1);
	return (process_variable_expantion(command_arr, arr_idx, var_pos, env));
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
