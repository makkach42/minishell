/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_nested_parentheses.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:35:48 by makkach           #+#    #+#             */
/*   Updated: 2025/04/23 10:39:58 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_outer_parenthases(char *str)
{
	int	i;
	int	par_open;
	int	par_closed;
	int in_quotes;
	char quote_type;

	i = 0;
	par_open = 0;
	par_closed = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if(in_quotes && str[i] == quote_type)
			in_quotes = 0;
		if (str[i] == '(' && !in_quotes)
			par_open++;
		else if (str[i] == ')')
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

void	process_nested_parentheses(t_tree **tree)
{
	char	*cmd_part;
	char	*original_redirs;
	char	*temp;
	char	*content;
	t_list	*sub_list;
	t_tree	*sub_tree;

	if (!tree || !(*tree))
		return ;
	if ((*tree)->left)
		process_nested_parentheses(&((*tree)->left));
	if ((*tree)->right)
		process_nested_parentheses(&((*tree)->right));
	if ((*tree)->command && has_outer_parenthases((*tree)->command))
	{
		cmd_part = NULL;
		original_redirs = NULL;
		extract_redirections((*tree)->command, &cmd_part, &original_redirs);
		if ((*tree)->redirections)
		{
			if (original_redirs)
			{
				temp = original_redirs;
				original_redirs = ft_strjoin(
						original_redirs, (*tree)->redirections);
				free(temp);
			}
			else
				original_redirs = ft_strdup((*tree)->redirections);
		}
		if (cmd_part)
			content = extract_content_from_parentheses(cmd_part);
		else
			content = extract_content_from_parentheses((*tree)->command);
		if (cmd_part)
			free(cmd_part);
		sub_list = list_init(content);
		sub_tree = NULL;
		lexer(&sub_list);
		tree_maker(&sub_list, &sub_tree);
		if (sub_tree)
		{
			if (still_has_parentheses(&sub_tree))
				process_nested_parentheses(&sub_tree);
			if ((*tree)->command)
			{
				free((*tree)->command);
				(*tree)->command = NULL;
			}
			(*tree)->type = "PARENTHASIS";
			if ((*tree)->redirections)
				free((*tree)->redirections);
			(*tree)->redirections = original_redirs;
			if ((*tree)->left)
				free_tree((*tree)->left);
			(*tree)->left = sub_tree;
		}
		else
		{
			free(content);
			if (original_redirs)
				free(original_redirs);
		}
	}
}
