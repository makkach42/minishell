/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:31:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/29 09:08:36 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isnt_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '%') || (
				str[i] == '"') || (
				str[i] == '\'') || (
				str[i] == '$') || (
				str[i] == '*') || (
				str[i] == '+' && str[i + 1]) || (
				str[0] >= '0' && str[0] <= '9'))
			return (1);
		i++;
	}
	return (0);
}

void	tokenizer(t_list *tmp)
{
	if (command_recognizer(tmp->data) == 1)
		tmp->token = "COMMAND";
	else if (variable_recognizer(tmp->data) == 1)
		tmp->token = "VARIABLE";
	else if (parenthasis_recognizer(tmp->data) == 1)
		tmp->token = "PARENTHASIS";
	else if (word_recognizer(tmp->data) == 1)
		tmp->token = "WORD";
	else if (pipe_recognizer(tmp->data) == 1)
		tmp->token = "PIPE";
	else if (pipe_recognizer(tmp->data) == 2)
		tmp->token = "OPERATION_||";
	else if (redirection_recognizer(tmp->data) == 1)
		tmp->token = "REDIRECTION";
	else if (operation_recognizer(tmp->data) == 1)
		tmp->token = "OPERATION_&&";
	else
		tmp->token = "WORD";
}

void	lexer(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (tmp)
	{
		tokenizer(tmp);
		tmp = tmp->next;
	}
}

void	free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = *env;
	while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
	*env = NULL;
}
