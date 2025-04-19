/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:49:23 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 13:50:23 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *str2)
{
	write(2, "syntax error near unexpected token `", 37);
	write(2, str2, ft_strlen(str2));
	write(2, "\'\n", 2);
}
void	syntax_error(t_list **head)
{
	t_list	*tmp;
	char	*prev_token;
	char	*prev_data;
	if (!head || !*head)
		return ;
	tmp = *head;
	if (ft_strcmp((*head)->token, "PIPE") == 0)
	{
		print_syntax_error("|");
		exit (1);
	}
	if (ft_strcmp((*head)->token, "OPERATION") == 0)
	{
		print_syntax_error((*head)->data);
		exit (1);
	}
	while (tmp)
	{
		prev_token = tmp->token;
		prev_data = tmp->data;
		tmp = tmp->next;
		if (ft_strcmp("REDIRECTION", prev_token) == 0 && tmp == NULL)
		{
			if (ft_strcmp(prev_data, ">") == 0 && ft_strlen(prev_data) == 1)
				write(2, "syntax error near unexpected token `newline'\n", 45);
			else if (ft_strcmp(prev_data, "<") == 0 && ft_strlen(prev_data) == 1)
				write(2, "syntax error near unexpected token `newline'\n", 45);
			else
				print_syntax_error(prev_data);
			exit(1);
		}
		if (ft_strcmp("OPERATION", prev_token) == 0 && tmp == NULL)
		{
			print_syntax_error(prev_data);
			exit(1);
		}
		if (ft_strcmp("PIPE", prev_token) == 0 && tmp == NULL)
		{
			print_syntax_error(prev_data);
			exit(1);
		}
		if (ft_strcmp("PIPE", prev_token) == 0 && ft_strcmp(prev_token, tmp->token) == 0)
		{
			write(2, "syntax error near unexpected token `|'\n", 40);
			exit(1);
		}
		if (ft_strcmp("OPERATION", prev_token) == 0 && ft_strcmp(prev_token, tmp->token) == 0)
		{
			print_syntax_error((tmp)->data);
			exit(1);
		}
		if (ft_strcmp("OPERATION", prev_token) == 0 && ft_strcmp(tmp->token, "PIPE") == 0)
		{
			print_syntax_error((tmp)->data);
			exit(1);
		}
		if (ft_strcmp("PIPE", prev_token) == 0 && ft_strcmp(tmp->token, "OPERATION") == 0)
		{
			write(2, "syntax error near unexpected token `|'\n", 40);
			exit(1);
		}
		if (ft_strcmp("REDIRECTION", prev_token) == 0 && ft_strcmp(tmp->token, "PIPE") == 0 && ft_strlen(prev_token) == 1)
		{
			print_syntax_error(prev_data);
			exit(1);
		}
		if (ft_strcmp("REDIRECTION", prev_token) == 0 && ft_strcmp(tmp->token, "OPERATION") == 0)
		{
			print_syntax_error(prev_data);
			exit(1);
		}
		if (ft_strcmp("OPERATION", prev_token) == 0 && ft_strcmp(tmp->token, "REDIRECTION") == 0)
		{
			print_syntax_error(prev_data);
			exit(1);
		}
		if (ft_strcmp("REDIRECTION", prev_token) == 0 && ft_strcmp(tmp->token, prev_token) == 0)
		{
			print_syntax_error(prev_data);
			exit(1);
		}
	}	
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
	{
		if (check_quotes((*tree)->command) == 1)
			exit (1);
	}
}