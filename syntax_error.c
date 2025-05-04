/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:49:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/04 11:14:43 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	more_ifs(char *prev_token, char *prev_data, t_list *tmp)
{
	if (!tmp)
		return (0);
	if (ft_strcmp("PIPE", prev_token) == 0 && ft_strcmp(
			prev_token, tmp->token) == 0)
		return (write(2, "syntax error near unexpected token `|'\n", 40), 1);
	if ((ft_strcmp("OPERATION_&&", prev_token) == 0 || ft_strcmp(
				"OPERATION_||", prev_token) == 0
		) && ft_strcmp(prev_token, tmp->token) == 0)
		return (print_syntax_error((tmp)->data), 1);
	if ((ft_strcmp("OPERATION_&&", prev_token) == 0 || ft_strcmp(
				"OPERATION_||", prev_token) == 0
		) && ft_strcmp(tmp->token, "PIPE") == 0)
		return (print_syntax_error((tmp)->data), 1);
	if (ft_strcmp("PIPE", prev_token) == 0 && (ft_strcmp(
				tmp->token, "OPERATION_&&") == 0 || ft_strcmp(
				tmp->token, "OPERATION_||") == 0))
		(write(2, "syntax error near unexpected token `|'\n", 40));
	if (ft_strcmp("REDIRECTION", prev_token) == 0 && ft_strcmp(
			tmp->token, "PIPE") == 0 && ft_strlen(prev_token) == 1)
		return (print_syntax_error(prev_data), 1);
	even_more_ifs(prev_token, prev_data, tmp);
	return (0);
}

void	while_loop_syntax_error(t_list *tmp, char *prev_token, char *prev_data)
{
	while (tmp)
	{
		prev_token = tmp->token;
		prev_data = tmp->data;
		tmp = tmp->next;
		if (ft_strcmp("REDIRECTION", prev_token) == 0 && tmp == NULL)
		{
			if (ft_strcmp(prev_data, ">") == 0 && ft_strlen(prev_data) == 1)
				(write(2, "syntax error near unexpected token `newline'\n",
						45));
			else if (ft_strcmp(prev_data, "<") == 0 && ft_strlen(
					prev_data) == 1)
				(write(2, "syntax error near unexpected token `newline'\n",
						45));
			else
				(print_syntax_error(prev_data));
		}
		if ((ft_strcmp("OPERATION_&&", prev_token) == 0 || ft_strcmp(
					"OPERATION_||", prev_token) == 0) && tmp == NULL)
			(print_syntax_error(prev_data));
		if (ft_strcmp("PIPE", prev_token) == 0 && tmp == NULL)
			(print_syntax_error(prev_data));
		more_ifs(prev_token, prev_data, tmp);
	}
}

void	syntax_error(t_list **head)
{
	t_list	*tmp;
	char	*prev_token;
	char	*prev_data;

	if (!head || !*head)
		return ;
	tmp = *head;
	prev_data = NULL;
	prev_token = NULL;
	if (ft_strcmp((*head)->token, "PIPE") == 0)
		(print_syntax_error("|"), exit (1));
	if (ft_strcmp((*head)->token, "OPERATION_&&") == 0 || ft_strcmp(
			(*head)->token, "OPERATION_||") == 0)
		print_syntax_error((*head)->data);
	while_loop_syntax_error(tmp, prev_token, prev_data);
}

void	syntax_error_two(t_tree **tree)
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		syntax_error_two(&(*tree)->left);
	if ((*tree)->right)
		syntax_error_two(&(*tree)->right);
	if ((*tree)->command)
		check_quotes((*tree)->command);
}

int	syntax_error_parentheses(t_list **head)
{
	t_list	*tmp;
	int		i;
	int		open_par;
	int		flag;

	tmp = *head;
	flag = 0;
	if (!head || !*head)
		return (0);
	while (tmp)
	{
		if (ft_strcmp(tmp->token, "PARENTHASIS") == 0)
		{
			i = 0;
			open_par = 0;
			syntax_error_parentheses_while_loop(tmp, &i, &flag, &open_par);
			if (flag == 1 || flag == 2 || flag == 3)
				return (print_syntax_error(tmp->data), 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
