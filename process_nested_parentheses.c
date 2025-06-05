/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_nested_parentheses.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:35:48 by makkach           #+#    #+#             */
/*   Updated: 2025/06/05 09:40:11 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	has_outer_parenthases_inits(int *i, int *par_open,
		int *par_closed, int *in_quotes)
{
	*i = 0;
	*par_open = 0;
	*par_closed = 0;
	*in_quotes = 0;
}

static int	has_outer_parenthases(char *str)
{
	int		i;
	int		par_open;
	int		par_closed;
	int		in_quotes;
	char	quote_type;

	has_outer_parenthases_inits(&i, &par_open, &par_closed, &in_quotes);
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		if (str[i] == '(' && !in_quotes)
			par_open++;
		else if (str[i] == ')' && !in_quotes)
			par_closed++;
		i++;
	}
	if (par_open == par_closed)
		return (1);
	return (0);
}

int	still_has_parentheses(t_tree **tree)
{
	int	results;

	if (!tree || !(*tree))
		return (0);
	results = 0;
	if ((*tree)->command)
	{
		if (has_outer_parenthases((*tree)->command))
			results = 1;
	}
	if ((*tree)->left)
		results = still_has_parentheses(&(*tree)->left);
	if ((*tree)->right)
		results = still_has_parentheses(&(*tree)->right);
	return (results);
}

void	redirections_handle(char **original_redirs, t_tree **tree)
{
	char	*temp;

	if (*original_redirs)
	{
		temp = *original_redirs;
		*original_redirs = ft_strjoin(
				*original_redirs, (*tree)->redirections);
		free(temp);
	}
	else
		*original_redirs = ft_strdup((*tree)->redirections);
}

void	process_nested_parentheses(t_tree **tree)
{
	char	*cmd_part;
	char	*original_redirs;
	char	*content;
	t_list	*sub_list;
	t_tree	*sub_tree;

	if (!tree || !(*tree))
		return ;
	if ((*tree) && (*tree)->left)
		process_nested_parentheses(&((*tree)->left));
	if ((*tree) && (*tree)->right)
		process_nested_parentheses(&((*tree)->right));
	if ((*tree) && (*tree)->command && has_outer_parenthases((*tree)->command))
	{
		redirections_in_par_handle(tree, &cmd_part, &original_redirs, &content);
		sub_tree_creation(&sub_tree, &content, &sub_list);
		if (sub_tree)
			sub_tree_attachment(&sub_tree, tree, &original_redirs);
		else
			free_all(&content, &original_redirs);
	}
}
