/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/03/19 10:03:46 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    TOKEN_WHITESPACE,
    TOKEN_COMMENT,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_VARIABLE,
    TOKEN_COMMAND,
    TOKEN_REDIRECTION,
    TOKEN_SPECIAL,
    TOKEN_UNKNOWN
*/

int word_recognizer(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] == 32)
		i++;
	while (str[i] == '-')
		i++;
	while (str[i] >= 'a' && str[i] <= 'z')
		i++;
	while (str[i] >= 'A' && str[i] <= 'Z')
		i++;
	while (str[i] == 32)
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);	
}

int number_recognizer(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] == 32)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	while (str[i] == 32)
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);	
}

int pipe_recognizer(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '|')
			break ;
		i++;
	}
	if (str[i] == '|')
		return (1);
	return (0);
}
int operation_recognizer(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '&' || str[i] == '+' || str[i] == '-' ||  str[i] == '*' ||  str[i] == '/')
			break ;
		i++;
	}
	if (str[i] == '&' || str[i] == '+' || str[i] == '-' ||  str[i] == '*' ||  str[i] == '/')
		return (1);
	return (0);
}

int	string_recognizer(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\"')
		{
			while (str[i] != '\"' && str[i] != '\0')
				i++;
			if (str[i] == '\"')
				break ;
		}
		i++;
	}
	if (str[i] == '\"')
		return (1);
	return (0);
}

int redirection_recognizer(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<')
			break ;
		i++;
	}
	if (str[i] == '>'|| str[i] == '<')
		return (1);
	return (0);	
}

void	tokenizer(t_list *tmp)
{
	if (string_recognizer(tmp->data) == 1)
		tmp->token = "STRING";
	else if (word_recognizer(tmp->data) == 1)
		tmp->token = "WORD";
	else if (number_recognizer(tmp->data) == 1)
		tmp->token = "NUMBER";
	else if (pipe_recognizer(tmp->data) == 1)
		tmp->token = "PIPE";
	else if (redirection_recognizer(tmp->data) == 1)
		tmp->token = "REDIRECTION";
	else if (operation_recognizer(tmp->data) == 1)
		tmp->token = "OPERATION";
	else
		tmp->token = "FUCK YOU";
}

void	lexer(t_list **head)
{
	t_list *tmp;

	tmp = *head;
	while (tmp)
	{
		tokenizer(tmp);
		tmp = tmp->next;
	}
}

int countstrings(char *str)
{
	int i = 0;
	int count = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			count++;
		i++;
	}	
	return (count / 2);
}

int	*where_string_starts(char *str)
{
	int i;
	int j;
	int	flag;
	int	how_many_strings;
	int *arr;

	how_many_strings = countstrings(str);
	i = -1;
	j = 0;
	flag = 0;
	arr = malloc(sizeof(int) * how_many_strings);
	while (how_many_strings)
	{
		while (str[++i] != '\"'){}
		if (str[i] == '\0')
			return (NULL);
		if (flag == 0 && str[i] == '\"')
		{
			arr[j] = i;
			j++;
            i++;
			while (str && str[i] && str[i] != '\"')
				i++;
			how_many_strings--;
		}
	}
	return (arr);
}

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

char *word_extractor(char *str)
{
	int i;
	char *word;

	i = -1;
	if (!str || !*str)
		return (NULL);
	while (str[++i] == 32){}
	while (str[i] != 32 && !is_operator(str[i]) && str[i] != '\0')
		i++;
	word = ft_substr(str, 0, i);
	return (word);
}

char *first_word_remover(char *str)
{
	int i;
	int	strlenth;
	char *new_str;

	i = -1;
	strlenth = ft_strlen(str);
	while (str[++i] == 32){}
	while (str[i] != 32 && !is_operator(str[i]) && str[i] != '\0')
		i++;
	while (str[i] == 32)
		i++;
	new_str = ft_substr(str, i, strlenth - i + 1);
	return (new_str);
}

t_list *node_maker(char *word)
{
	t_list *node;

	node = malloc(sizeof(t_list));
	node->data = word;
	node->next = NULL;
	return (node);
}
char *replace_whites_spaces(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (str[i] >= 9 && str[i] <= 13)
			str[i] = 32;
	}
	return (str);
}
char *str_extractor(char *str)
{
	int i;
	char *word;

	i = 1;
	if (!str | !*str)
		return (NULL);
	while (str[i] != '\"' && str[i] != '\0')
		i++;
	word = ft_substr(str, 0, i + 1);
	return (word);
}

char *str_remover(char *str)
{
	int i;
	int strlenth;
	char *new_str;

	i = 1;
	if (!str | !*str)
		return (NULL);
	strlenth = ft_strlen(str);
	while (str[i] == 32)
		i++;
	while (str[i] != '\"' && str[i] != '\0')
		i++;
	while (str[i] == 32)
		i++;
	i++;
	new_str = ft_substr(str, i, strlenth - i + 1);
	return (new_str);
}

char	*extract_operator(char *str)
{
	int	i;
	char *operator;

	if (!str | !*str)
		return (NULL);
	i = 0;
	operator = NULL;
	if (str[i] == '&' && str[i + 1] && str[i + 1] != '&')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '|' && str[i + 1] && str[i + 1] != '|')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '>' && str[i + 1] && str[i + 1] != '>')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '<' && str[i + 1] && str[i + 1] != '<')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '&' && str[i + 1] && str[i + 1] == '&' && str[i + 2] && str[i + 2] != '&')
		operator = ft_substr(str, 0, 2);
	else if (str[i] == '|' && str[i + 1] && str[i + 1] == '|' && str[i + 2] && str[i + 2] != '|')
		operator = ft_substr(str, 0, 2);
	else if (str[i] == '>' && str[i + 1] && str[i + 1] == '>' && str[i + 2] && str[i + 2] != '>')
		operator = ft_substr(str, 0, 2);
	else if (str[i] == '<' && str[i + 1] && str[i + 1] == '<' && str[i + 2] && str[i + 2] != '<')
		operator = ft_substr(str, 0, 2);
	return (operator);
}

char *remove_operator(char *str, char *word)
{
	int wordlen;
	int str_len;

	wordlen = ft_strlen(word);
	str_len = ft_strlen(str);
	str = ft_substr(str, wordlen, str_len - wordlen + 1);
	return (str);
}

t_list *list_init(char *str)
{
	char *word;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	word = word_extractor(str);
	str = first_word_remover(str);
	head = node_maker(word);
	tmp = head;
	while (str && *str)
	{
		if (*str == '\"')
		{
			word = str_extractor(str);
			str = str_remover(str);
			str = ft_strtrim(str, " ");
		}
		else if (is_operator(*str))
		{
			word = extract_operator(str);
			str = remove_operator(str, word);
			str = ft_strtrim(str, " ");
		}
		else
		{
			word = word_extractor(str);
			str = first_word_remover(str);
			str = ft_strtrim(str, " ");
		}
		new_node = malloc(sizeof(t_list));
		new_node->data = word;
		new_node->next = NULL;
		tmp->next = new_node;
		tmp = new_node;
	}
	return (head);
}

int main(void)
{
	while (1)
	{
		char *str;
		t_list *head;

		str = readline("minishell$> ");
		if (!str)
			break;
		add_history(str);
		str = replace_whites_spaces(str);
		head = list_init(str);
		lexer(&head);
		while (head)
		{
			printf("%s\n", head->data);
			printf("%s\n", head->token);
			printf("\n");
			head = head->next;
		}
	}
}
