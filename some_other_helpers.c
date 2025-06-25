/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_other_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:30:34 by makkach           #+#    #+#             */
/*   Updated: 2025/06/24 16:14:10 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ambiguous_set(t_tree **tree)
// {
// 	t_list_fd	*tmp;
// 	int			i;
// 	int			in_word;
// 	int			flag;
// 	int			in_quotes;
// 	int			count;
// 	char		quote_type;

// 	if ((*tree) && (*tree)->left)
// 		ambiguous_set(&(*tree)->left);
// 	if ((*tree) && (*tree)->right)
// 		ambiguous_set(&(*tree)->right);
// 	if ((*tree) && (*tree)->fd_list)
// 	{
// 		tmp = (*tree)->fd_list;
// 		while (tmp)
// 		{
// 			i = 0;
// 			in_quotes = 0;
// 			quote_type = 0;
// 			in_word = 0;
// 			count = 0;
// 			flag = 0;
// 			while (tmp->name[i])
// 			{
// 				if (tmp->name[i] != '"' && tmp->name[i] != '\'')
// 					flag = 1;
// 				if (!in_quotes && (tmp->name[i] == '"' || tmp->name[i] == '\''))
// 				{
// 					in_quotes = 1;
// 					quote_type = tmp->name[i];
// 				}
// 				else if (in_quotes && tmp->name[i] == quote_type)
// 					in_quotes = 0;
// 				if (!in_quotes && tmp->name[i] == 32)
// 					in_word = 0;
// 				else if (!in_word)
// 				{
// 					count++;
// 					in_word = 1;
// 				}
// 				i++;
// 			}
// 			if (count != 1)
// 				(*tree)->ambiguous = 1;
// 			else if (count == 1 && flag == 0 && ft_strcmp(tmp->redir, "<<"))
// 				(*tree)->quotes = 1;
// 			tmp = tmp->next;
// 		}
// 	}
// }
void	process_quotes_and_words_inits(int *in_quotes,
		int *in_word, int *count, int *flag)
{
	*in_quotes = 0;
	*in_word = 0;
	*count = 0;
	*flag = 0;
}

void	process_quotes_and_words(char *str, int *count, int *flag, int i)
{
	int		in_word;
	int		in_quotes;
	char	quote_type;

	i = -1;
	process_quotes_and_words_inits(&in_quotes, &in_word, count, flag);
	while (str[++i])
	{
		if (str[i] != '"' && str[i] != '\'')
			*flag = 1;
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		if (!in_quotes && str[i] == 32)
			in_word = 0;
		else if (!in_word)
		{
			(*count)++;
			in_word = 1;
		}
	}
}

void	analyze_fd_list(t_tree **tree)
{
	t_list_fd	*tmp;
	int			count;
	int			flag;
	int			i;

	tmp = (*tree)->fd_list;
	i = -1;
	while (tmp)
	{
		process_quotes_and_words(tmp->name, &count, &flag, i);
		if (count != 1 && ft_strcmp(tmp->redir, "<<"))
			(*tree)->ambiguous = 1;
		tmp = tmp->next;
	}
}

void	ambiguous_set(t_tree **tree)
{
	if (!(*tree))
		return ;
	if ((*tree)->left)
		ambiguous_set(&(*tree)->left);
	if ((*tree)->right)
		ambiguous_set(&(*tree)->right);
	if ((*tree)->fd_list)
		analyze_fd_list(tree);
}

int	new_syntax_error(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (tmp && tmp->next && tmp->next->next)
	{
		if (!ft_strcmp(tmp->token, "REDIRECTION") && !ft_strcmp(
				tmp->next->token, "WORD") && !ft_strcmp(
				tmp->next->next->token, "PARENTHASIS"))
			break ;
		tmp = tmp->next;
	}
	if (tmp && tmp->next && tmp->next->next)
		return (1);
	return (0);
}
