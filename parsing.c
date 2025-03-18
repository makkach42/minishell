/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/03/18 14:39:20 by makkach          ###   ########.fr       */
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

	while (str[i] == 32)
		i++;
	while (str[i] == '-')
		i++;
	while (str[i] >= 'a' && str[i] <= 'z')
		i++;
	while (str[i] >= 'A' && str[i] <= 'Z')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);	
}

int pipe_recognizer(char *str)
{
	int i;

	i = 0;
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

int	double_quote_recognizer(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			break ;
		i++;
	}
	if (str[i] == '\"')
		return (1);
	return (0);
}

int	single_quote_recognizer(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			break ;
		i++;
	}
	if (str[i] == '\'')
		return (1);
	return (0);
}

int redirection_recognizer(char *str)
{
	int i;

	i = 0;
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
	if (double_quote_recognizer(tmp->data) == 1)
		tmp->token = "DOUBLE_QUOTE";
	else if (single_quote_recognizer(tmp->data) == 1)
		tmp->token = "SINGLE_QUOTE";
	else if (word_recognizer(tmp->data) == 1)
		tmp->token = "WORD";
	else if (pipe_recognizer(tmp->data) == 1)
		tmp->token = "PIPE";
	else if (redirection_recognizer(tmp->data) == 1)
		tmp->token = "REDIRECTION";
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


// static t_list	*set_next(t_list **tmp, int counter)
// {
// 	t_list	*tmp3;
// 	int		counter2;

// 	counter2 = 0;
// 	counter--;
// 	tmp3 = tmp[counter2];
// 	while (counter)
// 	{
// 		tmp[counter2]->next = tmp[counter2 + 1];
// 		counter2++;
// 		counter--;
// 	}
// 	free(tmp);
// 	return (tmp3);
// }

// static t_list	*init_list(char **argv, int count)
// {
// 	t_list	**tmp;
// 	t_list	*tmp3;
// 	int		counter;

// 	counter = 0;
// 	tmp = (t_list **)malloc(sizeof(t_list *) * count);
// 	if (!tmp)
// 		return (free(tmp), NULL);
// 	while (counter < count)
// 	{
// 		tmp[counter] = malloc(sizeof(t_list));
// 		if (!tmp[counter])
// 			return (NULL);
// 		if (counter == 0)
// 			tmp3 = tmp[counter];
// 		tmp[counter]->data = argv[counter];
// 		if (counter + 1 == count)
// 			tmp[counter]->next = NULL;
// 		counter++;
// 	}
// 	return (set_next(tmp, counter));
// }

int countargs(char **args)
{
	int i = 0;
	while (args[i])
		i++;
	return (i);
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

int get_next_quote_index(char *str, int pos)
{
	while (str[++pos] != '\"'){}
	return (pos);
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

char *word_extractor(char *str)
{
	int i;
	char *word;

	i = -1;
	if (!str || !*str)
		return (NULL);
	while (str[++i] == 32){}
	while (str[i] != 32 && str[i] != '\0')
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
	while (str[i] != 32 && str[i] != '\0')
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
char *remove_whites_spaces(char *str)
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
		word = word_extractor(str);
		str = first_word_remover(str);
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
		str = remove_whites_spaces(str);
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
// pipes operators redirections
// numbers quotes