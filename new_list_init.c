/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_list_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:04:49 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 14:49:36 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*new_list_init(char *str)
{
	int		i;
	int		in_quotes;
	int		open_par;
	int		closed_par;
	char	quote_type;
	char	*word;
	char	*tmp_char;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if (!str)
		return (NULL);
	i = 0;
	in_quotes = 0;
	open_par = 0;
	closed_par = 0;
	word = NULL;
	if (*str == '"' || *str == '\'')
	{
		while (str[i])
		{
			if (!in_quotes && (str[i] == '"' || str[i] == '\''))
			{
				in_quotes = 1;
				quote_type = str[i];
			}
			else if (in_quotes && str[i] == quote_type)
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i + 1);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
		in_quotes = 0;
	}
	else if (str[i] == '(')
	{
		while (str[i])
		{
			if (str[i] == '(')
				open_par++;
			else if (str[i] == ')')
				closed_par++;
			if (open_par == closed_par && open_par != 0)
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i + 1);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (*str == '$')
	{
		i = 1;
		while (str[i])
		{
			if (!(str[i] >= 'a' && str[i] <= 'z') && !(
					str[i] >= 'A' && str[i] <= 'Z') && !(
					str[i] >= '0' && str[i] <= '9'))
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i, ft_strlen(str) - (i));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (*str == '+')
	{
		if (str[1] == '=')
		{
			tmp_char = word;
			word = ft_substr(str, 0, 2);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
		}
		else
		{
			tmp_char = word;
			word = ft_substr(str, 0, 1);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
		}
		tmp_char = str;
		str = ft_substr(str, ft_strlen(word),
				ft_strlen(str) - (ft_strlen(word)));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (is_operator(*str))
	{
		tmp_char = word;
		if (str[0] == '&' && ((str[1] && str[1] != '&') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '*' && ((str[1] && str[1] != '*') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '|' && ((str[1] && str[1] != '|') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '>' && ((str[1] && str[1] != '>') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '<' && ((str[1] && str[1] != '<') || !str[1]))
			word = ft_substr(str, 0, 1);
		else if (str[0] == '&' && str[1] && str[1] == '&')
			word = ft_substr(str, 0, 2);
		else if (str[0] == '|' && str[1] && str[1] == '|')
			word = ft_substr(str, 0, 2);
		else if (str[0] == '>' && str[1] && str[1] == '>')
			word = ft_substr(str, 0, 2);
		else if (str[0] == '<' && str[1] && str[1] == '<')
			word = ft_substr(str, 0, 2);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, ft_strlen(word), ft_strlen(str) - ft_strlen(word));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else
	{
		while (str[i])
		{
			if ((str[i] == '"') || (str[i] == '\''
				) || (str[i] == '$') || (
					is_operator(str[i])) || (
					str[i] == '(') || (
					str[i] == '='))
				break ;
			i++;
		}
		tmp_char = word;
		word = ft_substr(str, 0, i);
		free(tmp_char);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = ft_substr(str, i, ft_strlen(str) - (i));
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	head = malloc(sizeof(t_list));
	head->data = ft_strdup(word);
	head->token = NULL;
	head->prev = NULL;
	head->next = NULL;
	tmp = head;
	i = 0;
	while (str && *str)
	{
		if (*str == '"' || *str == '\'')
		{
			i = 0;
			while (str[i])
			{
				if (!in_quotes && (str[i] == '"' || str[i] == '\''))
				{
					in_quotes = 1;
					quote_type = str[i];
				}
				else if (in_quotes && str[i] == quote_type)
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i + 1);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
			in_quotes = 0;
		}
		else if (str[i] == '(')
		{
			i = 0;
			while (str[i])
			{
				if (str[i] == '(')
					open_par++;
				else if (str[i] == ')')
					closed_par++;
				if (open_par == closed_par && open_par != 0)
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i + 1);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (*str == '$')
		{
			i = 1;
			while (str[i])
			{
				if (!(str[i] >= 'a' && str[i] <= 'z'
					) && !(str[i] >= 'A' && str[i] <= 'Z'
					) && !(str[i] >= '0' && str[i] <= '9'))
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i, ft_strlen(str) - (i));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (is_operator(*str))
		{
			tmp_char = word;
			if (str[0] == '&' && ((str[1] && str[1] != '&') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '*' && ((str[1] && str[1] != '*') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '|' && ((str[1] && str[1] != '|') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '>' && ((str[1] && str[1] != '>') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '<' && ((str[1] && str[1] != '<') || !str[1]))
				word = ft_substr(str, 0, 1);
			else if (str[0] == '&' && str[1] && str[1] == '&')
				word = ft_substr(str, 0, 2);
			else if (str[0] == '|' && str[1] && str[1] == '|')
				word = ft_substr(str, 0, 2);
			else if (str[0] == '>' && str[1] && str[1] == '>')
				word = ft_substr(str, 0, 2);
			else if (str[0] == '<' && str[1] && str[1] == '<')
				word = ft_substr(str, 0, 2);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, ft_strlen(word),
					ft_strlen(str) - ft_strlen(word));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (*str == '=')
		{
			tmp_char = word;
			word = ft_substr(str, 0, 1);
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, 1, ft_strlen(str) - 1);
			free(tmp_char);
		}
		else
		{
			i = 0;
			while (str[i])
			{
				if ((str[i] == '"') || (str[i] == '\''
					) || (str[i] == '$') || (
						is_operator(str[i])) || (
						str[i] == '('))
					break ;
				i++;
			}
			tmp_char = word;
			word = ft_substr(str, 0, i);
			free(tmp_char);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = ft_substr(str, i, ft_strlen(str) - (i));
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		new_node = malloc(sizeof(t_list));
		new_node->data = ft_strdup(word);
		new_node->token = NULL;
		new_node->prev = tmp;
		new_node->next = NULL;
		tmp->next = new_node;
		tmp = new_node;
	}
	if (str)
		free(str);
	if (word)
		free(word);
	return (head);
}
