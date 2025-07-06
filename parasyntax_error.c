/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parasyntax_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:11:51 by makkach           #+#    #+#             */
/*   Updated: 2025/07/06 11:12:04 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	par_count(t_list *head)
{
	int		open_par;
	int		closed_par;
	t_list	*tmp;

	open_par = 0;
	closed_par = 0;
	tmp = head;
	while (tmp)
	{
		if (tmp && !ft_strcmp(tmp->token, "OPEN_PAR"))
			open_par++;
		else if (tmp && !ft_strcmp(tmp->token, "CLOSED_PAR"))
			closed_par++;
		tmp = tmp->next;
	}
	if (open_par == closed_par)
		return (0);
	return (1);
}

int	parenthasis_syntax_error_helper(t_list *tmp, char *prev_token)
{
	if (tmp && !ft_strcmp(prev_token, "OPEN_PAR"
		) && ((!ft_strcmp(tmp->token, "OPERATION_||"
				) || !ft_strcmp(tmp->token, "OPERATION_&&"))))
		return (1);
	if (tmp && !ft_strcmp(prev_token, "OPEN_PAR"
		) && !ft_strcmp(tmp->token, "PIPE"))
		return (1);
	if (tmp && !ft_strcmp(prev_token, "PIPE"
		) && !ft_strcmp(tmp->token, "CLOSED_PAR"))
		return (1);
	if (tmp && !ft_strcmp(prev_token, "REDIRECTION"
		) && !ft_strcmp(tmp->token, "CLOSED_PAR"))
		return (1);
	if (tmp && !ft_strcmp(prev_token, "REDIRECTION"
		) && !ft_strcmp(tmp->token, "PIPE"))
		return (1);
	if (tmp && !ft_strcmp(prev_token, "REDIRECTION"
		) && (!ft_strcmp(tmp->token, "OPERATION_&&"
			) || !ft_strcmp(tmp->token, "OPERATION_||")))
		return (1);
	return (0);
}

int	parenthasis_syntax_error_helper_two(t_list *tmp, char *prev_token)
{
	if (tmp && ((!ft_strcmp(prev_token, "OPERATION_||"
				) || !ft_strcmp(prev_token, "OPERATION_&&"))
		) && (!ft_strcmp(tmp->token, "CLOSED_PAR")))
		return (1);
	if (tmp && ((!ft_strcmp(prev_token, "OPERATION_||"
				) || !ft_strcmp(prev_token, "OPERATION_&&"))
		) && ((!ft_strcmp(tmp->token, "OPERATION_||"
				) || !ft_strcmp(tmp->token, "OPERATION_&&"))))
		return (1);
	if (tmp && (!ft_strcmp(prev_token, "CLOSED_PAR"
			) && (!ft_strcmp(tmp->token, "WORD"
				) || !ft_strcmp(tmp->token, "COMMAND"))))
		return (1);
	if (tmp && ((!ft_strcmp(prev_token, "WORD"
				) || !ft_strcmp(prev_token, "COMMAND")
			) && !ft_strcmp(tmp->token, "OPEN_PAR")))
		return (1);
	if (tmp && (!ft_strcmp(prev_token, "REDIRECTION"
			) && !ft_strcmp(tmp->token, "OPEN_PAR")))
		return (1);
	return (0);
}

int	parenthasis_syntax_error(t_list *head)
{
	t_list	*tmp;
	char	*prev_data;
	char	*prev_token;

	tmp = head;
	while (tmp)
	{
		prev_data = tmp->data;
		prev_token = tmp->token;
		tmp = tmp->next;
		if (!ft_strcmp("&", prev_data))
			return (1);
		if (par_count(head))
			return (1);
		if (tmp && !ft_strcmp(prev_token, "OPEN_PAR"
			) && (!ft_strcmp(tmp->token, "CLOSED_PAR")))
			return (1);
		if (parenthasis_syntax_error_helper(tmp, prev_token))
			return (1);
		if (parenthasis_syntax_error_helper_two(tmp, prev_token))
			return (1);
	}
	return (0);
}

int	in_para_check(char *str)
{
	t_list	*head;
	char	*new_str;

	new_str = ft_strdup(str);
	head = new_list_init_two(new_str);
	lexer_two(&head);
	if (parenthasis_syntax_error(head))
		return (free_list(&head), 1);
	free_list(&head);
	return (0);
}
