/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:58:14 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 16:12:19 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_whites_spaces(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] >= 9 && str[i] <= 13)
			str[i] = 32;
	}
	return (str);
}

int	lst_size(t_list **head)
{
	t_list	*tmp;
	int		i;

	tmp = *head;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	extract_content_from_parentheses_helper(char *command, int *i, int *j)
{
	while (command[*i] && *j > 0)
	{
		if (command[*i] == '(')
			(*j)++;
		else if (command[*i] == ')')
			(*j)--;
		(*i)++;
	}
}

char	*extract_content_from_parentheses(char *command)
{
	int		i;
	int		j;
	int		start;
	int		end;
	char	*content;

	if (!command)
		return (NULL);
	i = 0;
	while (command[i] && command[i] != '(')
		i++;
	if (!command[i])
		return (NULL);
	start = i + 1;
	i++;
	j = 1;
	extract_content_from_parentheses_helper(command, &i, &j);
	if (j != 0)
		return (NULL);
	end = i - 1;
	content = ft_substr(command, start, end - start);
	return (content);
}

int	check_quotes(char *str)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		i++;
	}
	if (in_quotes)
		return (write(2, "Syntax error: open quotes.\n", 28), 1);
	return (0);
}
