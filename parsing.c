/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/03/21 14:24:32 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int command_recognizer(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	if (ft_strcmp(str, "echo") == 0)
		return (1);
	else if (ft_strcmp(str, "cd") == 0)
		return (1);
	else if (ft_strcmp(str, "pwd") == 0)
		return (1);
	else if (ft_strcmp(str, "env") == 0)
		return (1);
	else if (ft_strcmp(str, "export") == 0)
		return (1);
	else if (ft_strcmp(str, "exit") == 0)
		return (1);
	else if (ft_strcmp(str, "unset") == 0)
		return (1);
	return (0);
}

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
	if ((str[i] == '|' && str[i + 1] && str[i + 1] != '|') || (str[i] == '|' && !str[i + 1]))
		return (1);
	else if (str[i] == '|' && str[i + 1] && str[i + 1] == '|')
		return (2);
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
		if (str[i] == '&')
			break ;
		i++;
	}
	if ((str[i] == '&' && str[i + 1] && str[i + 1] == '&'))
		return (1);
	return (0);
}

int	string_recognizer(char *str)
{
	int i;
	int j;
	int flag;

	i = 0;
	j = 0;
	flag = 0;
	if (!str)
		return (0);
	if (str[i] != '\"')
		flag = 1;
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
	j = i;
	while (str[j])
		j++;
	if (str[j - 1] != '\"')
		flag = 1;
	if (str[i] == '\"' && flag == 0)
		return (1);
	if (str[i] == '\"' && flag == 1)
		return (2);
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
	else if (string_recognizer(tmp->data) == 2)
		tmp->token = "WORD";
	else if (command_recognizer(tmp->data) == 1)
		tmp->token = "COMMAND";
	else if (word_recognizer(tmp->data) == 1)
		tmp->token = "WORD";
	else if (number_recognizer(tmp->data) == 1)
		tmp->token = "NUMBER";
	else if (pipe_recognizer(tmp->data) == 1)
		tmp->token = "PIPE";
	else if (pipe_recognizer(tmp->data) == 2)
		tmp->token = "OPERATION";
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

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}
int	string_in_word_recognizer(char *str)
{
	int i;
	int flag;
	int	quote_counter;

	i = 0;
	flag = 0;
	quote_counter = 0;
	while ((quote_counter % 2 != 0 && quote_counter != 0) || flag == 0)
	{
		if (str[i] == '\"')
			quote_counter++;
		if (!str[i] || (str[i] == 32 && quote_counter == 0) || (str[i] == 32 && quote_counter % 2 == 0))
			flag = 1;
		i++;
	}
	if (quote_counter % 2 == 0 && quote_counter != 0)
		return (1);
	return (0);
}

char *word_extractor(char *str)
{
	int i;
	int flag;
	int count_quotes;
	char *word;

	i = -1;
	if (!str || !*str)
		return (NULL);
	flag = 0;
	if (string_in_word_recognizer(str) == 1)
	{
		i = 0;
		count_quotes = 0;
		while ((count_quotes == 0 || (count_quotes % 2 != 0)))
		{
			while (str[i] && str[i] != '\"' && str[i] != 32 && str[i] != '\0')
				i++;
			if (str[i] == 32)
				break ;
			else if (str[i] == '\"')
			{
				i++;
				while (str[i] != '\"' && str[i] != '\0')
					i++;
				if (str[i] == '\0')
					break ;
				i++;
				if (str[i] == '\0')
					break ;
				if (str[i] == 32)
					break ;
				else
				{
					while (str[i] != 32 && str[i] != '\0')
						i++;
				}
			}
			if (!str[i])
				i--;
			i++;
			if (!str[i])
				break ;
		}
		return (ft_substr(str, 0, i));
	}
	else
	{
		while (str[++i] == 32){}
		while (str[i] != 32 && !is_operator(str[i]) && str[i] != '\0')
			i++;
		word = ft_substr(str, 0, i + 1);
	}
	return (word);
}

char *first_word_remover(char *str, char *word)
{
	int	strlenth;
	int	wordlenth;
	char *new_str;

	if (!str || !word)
		return (NULL);
	strlenth = ft_strlen(str);
	wordlenth = ft_strlen(word);
	new_str = ft_substr(str, wordlenth, strlenth - wordlenth + 1);
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
	int flag;
	char *word;
	int quote_counter;

	i = 1;
	flag = 0;
	if (!str || !*str)
		return (NULL);
	while (str[i] != '\"' && str[i] != '\0')
		i++;
	if (str[i + 1] && (str[i + 1] == 32 || str[i + 1] == '\0'))
	{
		return (ft_substr(str, 0, i + 1));
	}
	else
	{
		quote_counter = 2;
		while (str[i] != '\0' && flag == 0)
		{
			if (str[i] == '\"')
				quote_counter++;
			i++;
			if (str[i] == 32 && quote_counter % 2 != 0)
				flag = 1;
		}
		word = ft_substr(str, 0, i + 1);
	}
	return (word);
}

char *str_remover(char *str, char *word)
{
	int strlenth;
	char *new_str;
	int word_len;

	if (!str | !*str)
		return (NULL);
	strlenth = ft_strlen(str);
	word_len = ft_strlen(word);
	new_str = ft_substr(str, word_len, strlenth - word_len);
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
	else if (str[i] == '*' && str[i + 1] && str[i + 1] != '*')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '|' && str[i + 1] && str[i + 1] != '|')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '>' && str[i + 1] && str[i + 1] != '>')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '<' && str[i + 1] && str[i + 1] != '<')
		operator = ft_substr(str, 0, 1);
	else if (str[i] == '&' && str[i + 1] && str[i + 1] == '&')
		operator = ft_substr(str, 0, 2);
	else if (str[i] == '|' && str[i + 1] && str[i + 1] == '|')
		operator = ft_substr(str, 0, 2);
	else if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
		operator = ft_substr(str, 0, 2);
	else if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
		operator = ft_substr(str, 0, 2);
	return (operator);
}

char *remove_operator(char *str, char *word)
{
	int wordlen;
	int str_len;

	if (!str || !word)
		return (NULL);
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

	if (*str == '\"')
	{
		word = str_extractor(str);
		str = str_remover(str, word);
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
		str = first_word_remover(str, word);
		str = ft_strtrim(str, " ");
	}
	head = node_maker(word);
	tmp = head;
	while (str && *str)
	{
		if (*str == '\"')
		{
			word = str_extractor(str);
			str = str_remover(str, word);
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
			str = first_word_remover(str, word);
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
int	lst_size(t_list **head)
{
	t_list *tmp;
	int	i;

	tmp = *head;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
void	free_list(t_list **head)
{
	t_list *tmp;
	t_list *tmp2;

	tmp = *head;
	tmp2 = tmp;
	while (tmp)
	{
		tmp = tmp->next;
		free(tmp2);
		tmp2 = tmp;
	}
}

char **converter(t_list **head)
{
	char **arr;
	int size_list;
	int i;
	t_list *tmp;

	i = 0;
	tmp = *head;
	size_list = lst_size(head);
	arr = malloc(sizeof(char *) * (size_list + 1));
	while (tmp)
	{
		arr[i] = tmp->data;
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	free_list(head);
	return (arr);
}

int main(void)
{
	while (1)
	{
		char *str;
		char **arr;
		t_list *head;
		int i;

		str = readline("minishell$> ");
		i = 0;
		if (!str)
			break;
		add_history(str);
		str = replace_whites_spaces(str);
		head = list_init(str);
		lexer(&head);
		arr = converter(&head);
		while (arr[i])
		{
			printf("%s\n", arr[i]);
			i++;
		}
	}
}
