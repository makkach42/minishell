/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/16 20:06:47 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_list_fd(t_list_fd **head)
{
    if (!head || !*head)
        return;
    
    t_list_fd *current = *head;
    t_list_fd *next = NULL;
    while (current)
    {
        next = current->next;
        if (current->command)
            (t_free(current->command, 26, "parsing.c"), current->command = NULL);
        if (current->name)
            (t_free(current->name, 28, "parsing.c"), current->name = NULL);
        if (current->redir)
            (t_free(current->redir, 30, "parsing.c"), current->redir = NULL);
        if (current->fd > 0)
            (close(current->fd), current->fd = -1);
        t_free(current, 33, "parsing.c");
        current = next;
    }
    *head = NULL;
}

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
		if (str[i] != '|')
			return (0);
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
		if (str[i] != '&')
			return (0);
		if (str[i] == '&')
			break ;
		i++;
	}
	if ((str[i] == '&' && str[i + 1] && str[i + 1] == '&'))
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

int variable_recognizer(char *str)
{
	if (!str)
		return (0);
	if (*str == '$')
		return (1);
	return (0);
}
int parenthasis_recognizer(char *str)
{
	int i;
	int j;
	int flag;
	int	open_par;
	int	closed_par;

	i = 0;
	j = 0;
	flag = 0;
	open_par = 0;
	closed_par = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '(')
			open_par++;
		if (str[i] == ')')
			closed_par++;
		if (open_par != 0 && closed_par != 0 && open_par == closed_par)
		{
			flag = 1;
			j = i;
			break ;
		}
		i++;
	}
	if (flag == 1 && (str[i + 1] == 32 || str[i + 1] == '\0'))
		return (1);
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
		tmp->token = "WORD";
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

int parenthesis_in_word_recogniser(char *str)
{
	int i;
	int flag;
	int	open_par;
	int	closed_par;

	i = 0;
	flag = 0;
	open_par = 0;
	closed_par = 0;
	while ((open_par == closed_par && open_par != 0 && closed_par) || flag == 0)
	{
		if (str[i] == '(')
			open_par++;
		if (str[i] == ')')
			closed_par++;
		if (!str[i] || (str[i] == 32 && open_par == 0 && closed_par == 0) || (str[i] == 32 && open_par == closed_par))
			flag = 1;
		i++;
	}
	if (open_par  == closed_par && (open_par != 0 && closed_par != 0))
		return (1);
	return (0);
}

char *word_extractor(char *str)
{
	int i;
	int flag;
	int count_quotes;
	int open_par;
	int closed_par;
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
		word = ft_substr_leak(str, 0, i, 354);
		return (word);
	}
	else if (parenthesis_in_word_recogniser(str) == 1)
	{
		i = 0;
		open_par = 0;
		closed_par = 0;
		while (((open_par == 0 && closed_par == 0) || (open_par != closed_par)))
		{
			while (str[i] && str[i] != '(' && str[i] != 32 && str[i] != '\0')
				i++;
			if (str[i] == 32)
				break ;
			else if (str[i] == '(')
			{
				i++;
				while (str[i] != ')' && str[i] != '\0')
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
		word = ft_substr_leak(str, 0, i, 392);
		return (word);
	}
	else
	{
		while (str[++i] == 32){}
		while (str[i] != 32 && !is_operator(str[i]) && str[i] != '\0')
			i++;
		word = ft_substr_leak(str, 0, i, 400);
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
	new_str = ft_substr_leak(str, wordlenth, strlenth - wordlenth + 1, 415);
	return (new_str);
}

t_list *node_maker(char *word)
{
	t_list *node;

	node = t_malloc(sizeof(t_list), 423, "parsing.c");
	node->data = word;
	node->next = NULL;
	node->prev = NULL;
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
	if (str[i] != '\0' && str[i + 1] && (str[i + 1] == 32 || str[i + 1] == '\0'))
	{
		word = ft_substr_leak(str, 0, i + 1, 456);
		return (word);
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
		word = ft_substr_leak(str, 0, i + 1, 470);
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
	new_str = ft_substr_leak(str, word_len, strlenth - word_len, 485);
	return (new_str);
}
char *parenthesis_remover(char *str, char *word)
{
	int strlenth;
	char *new_str;
	int word_len;

	if (!str | !*str)
		return (NULL);
	strlenth = ft_strlen(str);
	word_len = ft_strlen(word);
	new_str = ft_substr_leak(str, word_len, strlenth - word_len, 498);
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
	if (str[i] == '&' && ((str[i + 1] && str[i + 1] != '&') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 511);
	else if (str[i] == '*' && ((str[i + 1] && str[i + 1] != '*') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 513);
	else if (str[i] == '|' && ((str[i + 1] && str[i + 1] != '|') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 515);
	else if (str[i] == '>' && ((str[i + 1] && str[i + 1] != '>') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1, 517);
	else if (str[i] == '<' && ((str[i + 1] && str[i + 1] != '<') || !str[i + 1]))
		operator = ft_substr_leak(str, 0, 1,519);
	else if (str[i] == '&' && str[i + 1] && str[i + 1] == '&')
		operator = ft_substr_leak(str, 0, 2, 521);
	else if (str[i] == '|' && str[i + 1] && str[i + 1] == '|')
		operator = ft_substr_leak(str, 0, 2, 523);
	else if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
		operator = ft_substr_leak(str, 0, 2, 525);
	else if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
		operator = ft_substr_leak(str, 0, 2, 527);
	return (operator);
}

char *remove_operator(char *str, char *word)
{
	int wordlen;
	int str_len;
	char *str_word;

	if (!str || !word)
		return (NULL);
	wordlen = ft_strlen(word);
	str_len = ft_strlen(str);
	str_word = ft_substr_leak(str, wordlen, str_len - wordlen + 1, 541);
	return (str_word);
}
char *extract_variable(char *str)
{
	int i;
	char *word;

	i = 0;
	while (str[i] != 32 && str[i] != '\0')
		i++;
	word = ft_substr_leak(str, 0, i + 1, 552);
	return (word);
}
char *extract_parenthesis(char *str)
{
	int i;
	int flag;
	int open_par;
	int closed_par;
	char *word;
	i = 0;
	flag = 0;
	open_par = 0;
	closed_par = 0;
	word = NULL;
	if (!str || !*str)
		return (NULL);
	while (str[i] != ')' && str[i] != '\0')
	{
		if (str[i] == '(')
			open_par++;
		i++;
	}
	if (str[i] == ')')
		closed_par++;
	i++;
	while (open_par > closed_par && str[i] != '\0')
	{
		if (str[i] == '(')
			open_par++;			
		if (str[i] == ')')
			closed_par++;
		i++;
	}
	if ((str[i] == '\0' || str[i] == 32))
	{
		word = ft_substr_leak(str, 0, i, 588);
		return (word);
	}
	else
	{
		open_par = 1;
		closed_par = 1;
		while (str[i] != '\0' && flag == 0)
		{
			if (str[i] == '(')
				open_par++;
			if (str[i] == ')')
				closed_par++;
			i++;
			if (str[i] == 32 && open_par == closed_par)
				flag = 1;
		}
		word = ft_substr_leak(str, 0, i + 1, 605);
	}
	return (word);
}

// t_list *list_init(char *str)
// {
// 	char *word;
// 	char *tmp_char;
// 	t_list	*head;
// 	t_list	*new_node;
// 	t_list	*tmp;

// 	if (*str == '\"')
// 	{
// 		word = str_extractor(str);
// 		tmp_char = word;
// 		word = ft_strtrim_leak(word, " ", 622);
// 		if (tmp_char)
// 			t_free(tmp_char, 623, "parsing.c");
// 		tmp_char = str;
// 		str = str_remover(str, word);
// 		if (tmp_char)
// 			t_free(tmp_char, 627, "parsing.c");
// 		tmp_char = str;
// 		str = ft_strtrim_leak(str, " ", 630);
// 		if (tmp_char)
// 			t_free(tmp_char, 631, "parsing.c");
// 	}
// 	else if (is_operator(*str))
// 	{
// 		word = extract_operator(str);
// 		tmp_char = word;
// 		word = ft_strtrim_leak(word, " ", 638);
// 		if (tmp_char)
// 			t_free(tmp_char, 639, "parsing.c");
// 		tmp_char = str;
// 		str = remove_operator(str, word);
// 		if (tmp_char)
// 			t_free(tmp_char, 643, "parsing.c");
// 		tmp_char = str;
// 		str = ft_strtrim_leak(str, " ", 646);
// 		if (tmp_char)
// 			t_free(tmp_char, 647, "parsing.c");
// 	}
// 	else if (*str == '$')
// 	{
// 		word = extract_variable(str);
// 		tmp_char = word;
// 		word = ft_strtrim_leak(word, " ", 654);
// 		if (tmp_char)
// 			t_free(tmp_char, 655, "parsing.c");
// 		tmp_char = str;
// 		str = first_word_remover(str, word);
// 		if (tmp_char)
// 			t_free(tmp_char, 659, "parsing.c");
// 		tmp_char = str;
// 		str= ft_strtrim_leak(str, " ", 662);
// 		if (tmp_char)
// 			t_free(tmp_char, 663, "parsing.c");
// 	}
// 	else if (*str == '(')
// 	{
// 		word = extract_parenthesis(str);
// 		tmp_char = word;
// 		word = ft_strtrim_leak(word, " ", 670);
// 		if (tmp_char)
// 			t_free(tmp_char, 671, "parsing.c");
// 		tmp_char = str;
// 		str = parenthesis_remover(str, word);
// 		if (tmp_char)
// 			t_free(tmp_char, 675, "parsing.c");
// 		tmp_char = str;
// 		str= ft_strtrim_leak(str, " ", 678);
// 		if (tmp_char)
// 			t_free(tmp_char, 679, "parsing.c");
// 	}
// 	else
// 	{
// 		word = word_extractor(str);
// 		tmp_char = word;
// 		word = ft_strtrim_leak(word, " ", 686);
// 		if (tmp_char)
// 			t_free(tmp_char, 687, "parsing.c");
// 		tmp_char = str;
// 		str = first_word_remover(str, word);
// 		if (tmp_char)
// 			t_free(tmp_char, 691, "parsing.c");
// 		tmp_char = str;
// 		str = ft_strtrim_leak(str, " ", 694);
// 		if (tmp_char)
// 			t_free(tmp_char, 695, "parsing.c");
// 	}
// 	head = node_maker(word);
// 	tmp = head;
// 	while (str && *str)
// 	{
// 		if (*str == '\"')
// 		{
// 			word = str_extractor(str);
// 			tmp_char = word;
// 			word = ft_strtrim_leak(word, " ", 706);
// 			if (tmp_char)
// 				t_free(tmp_char, 707, "parsing.c");
// 			tmp_char = str;
// 			str = str_remover(str, word);
// 			if (tmp_char)
// 				t_free(tmp_char, 711, "parsing.c");
// 			tmp_char = str;
// 			str = ft_strtrim_leak(str, " ", 714);
// 			if (tmp_char)
// 				t_free(tmp_char, 715, "parsing.c");
// 		}
// 		else if (is_operator(*str))
// 		{
// 			word = extract_operator(str);
// 			tmp_char = word;
// 			word = ft_strtrim_leak(word, " ", 722);
// 			if (tmp_char)
// 				t_free(tmp_char, 723, "parsing.c");
// 			tmp_char = str;
// 			str = remove_operator(str, word);
// 			if (tmp_char)
// 				t_free(tmp_char, 727, "parsing.c");
// 			tmp_char = str;
// 			str = ft_strtrim_leak(str, " ", 730);
// 			if (tmp_char)
// 				t_free(tmp_char, 731, "parsing.c");
// 		}
// 		else if (*str == '$')
// 		{
// 			word = extract_variable(str);
// 			tmp_char = word;
// 			word = ft_strtrim_leak(word, " ", 738);
// 			if (tmp_char)
// 				t_free(tmp_char, 739, "parsing.c");
// 			tmp_char = str;
// 			str = first_word_remover(str, word);
// 			if (tmp_char)
// 				t_free(tmp_char, 743, "parsing.c");
// 			tmp_char = str;
// 			str= ft_strtrim_leak(str, " ", 746);
// 			if (tmp_char)
// 				t_free(tmp_char, 747, "parsing.c");
// 		}
// 		else if (*str == '(')
// 		{
// 			word = extract_parenthesis(str);
// 			tmp_char = word;
// 			word = ft_strtrim_leak(word, " ", 754);
// 			if (tmp_char)
// 				t_free(tmp_char, 755, "parsing.c");
// 			tmp_char = str;
// 			str = parenthesis_remover(str, word);
// 			if (tmp_char)
// 				t_free(tmp_char, 759, "parsing.c");
// 			tmp_char = str;
// 			str= ft_strtrim_leak(str, " ", 762);
// 			if (tmp_char)
// 				t_free(tmp_char, 763, "parsing.c");
// 		}
// 		else
// 		{
// 			word = word_extractor(str);
// 			tmp_char = word;
// 			word = ft_strtrim_leak(word, " ", 770);
// 			if (tmp_char)
// 				t_free(tmp_char, 771, "parsing.c");
// 			tmp_char = str;
// 			str = first_word_remover(str, word);
// 			if (tmp_char)
// 				t_free(tmp_char, 775, "parsing.c");
// 			tmp_char = str;
// 			str = ft_strtrim_leak(str, " ", 778);
// 			if (tmp_char)
// 				t_free(tmp_char, 779, "parsing.c");
// 		}
// 		new_node = t_malloc(sizeof(t_list), 782, "parsing.c");
// 		new_node->data = word;
// 		new_node->next = NULL;
// 		new_node->prev = tmp;
// 		tmp->next = new_node;
// 		tmp = new_node;
// 	}
// 	return (head);
// }

// t_list *list_init(char *str)
// {
//     char *word = NULL;
//     char *tmp_char = NULL;
//     t_list *head = NULL;
//     t_list *new_node = NULL;
//     t_list *tmp = NULL;
    
//     if (!str || !*str)
//         return NULL;
    
//     if (*str == '\"')
//     {
//         word = str_extractor(str);
//         if (!word)
//             return NULL;
            
//         tmp_char = word;
//         word = ft_strtrim_leak(word, "", 808);
//         free(tmp_char);
        
//         if (!word)
//             return NULL;
            
//         tmp_char = str;
//         str = str_remover(str, word);
//         free(tmp_char);
        
//         if (str)
//         {
//             tmp_char = str;
//             str = ft_strtrim_leak(str, "", 818);
//             free(tmp_char);
//         }
//     }
//     else if (is_operator(*str))
//     {
//         word = extract_operator(str);
//         if (!word)
//             return NULL;
            
//         tmp_char = word;
//         word = ft_strtrim_leak(word, "", 829);
//         free(tmp_char);
        
//         if (!word)
//             return NULL;
            
//         tmp_char = str;
//         str = remove_operator(str, word);
//         free(tmp_char);
        
//         if (str)
//         {
//             tmp_char = str;
//             str = ft_strtrim_leak(str, "", 842);
//             free(tmp_char);
//         }
//     }
//     else if (*str == '$')
//     {
//         word = extract_variable(str);
//         if (!word)
//             return NULL;
            
//         tmp_char = word;
//         word = ft_strtrim_leak(word, "", 853);
//         free(tmp_char);
        
//         if (!word)
//             return NULL;
            
//         tmp_char = str;
//         str = first_word_remover(str, word);
//         free(tmp_char);
        
//         if (str)
//         {
//             tmp_char = str;
//             str = ft_strtrim_leak(str, "", 866);
//             free(tmp_char);
//         }
//     }
//     else if (*str == '(')
//     {
//         word = extract_parenthesis(str);
//         if (!word)
//             return NULL;
            
//         tmp_char = word;
//         word = ft_strtrim_leak(word, "", 877);
//         free(tmp_char);
        
//         if (!word)
//             return NULL;
            
//         tmp_char = str;
//         str = parenthesis_remover(str, word);
//         free(tmp_char);
        
//         if (str)
//         {
//             tmp_char = str;
//             str = ft_strtrim_leak(str, "", 890);
//             free(tmp_char);
//         }
//     }
//     else
//     {
//         word = word_extractor(str);
//         if (!word)
//             return NULL;
            
//         tmp_char = word;
//         word = ft_strtrim_leak(word, "", 901);
//         free(tmp_char);
        
//         if (!word)
//             return NULL;
            
//         tmp_char = str;
//         str = first_word_remover(str, word);
//         free(tmp_char);
        
//         if (str)
//         {
//             tmp_char = str;
//             str = ft_strtrim_leak(str, "", 914);
//             free(tmp_char);
//         }
//     }
    
//     head = node_maker(word);
//     if (!head)
//         return NULL;
        
//     tmp = head;
    
//     while (str && *str)
//     {
//         char *trim_result = NULL;
        
//         if (*str == '\"')
//         {
//             word = str_extractor(str);
//             if (!word)
//                 break;
                
//             tmp_char = word;
//             word = ft_strtrim_leak(word, "", 930);
//             free(tmp_char);
            
//             if (!word)
//                 break;
                
//             tmp_char = str;
//             str = str_remover(str, word);
//             free(tmp_char);
            
//             if (str)
//             {
//                 tmp_char = str;
//                 str = ft_strtrim_leak(str, "", 940);
//                 free(tmp_char);
//             }
//         }
//         else if (is_operator(*str))
//         {
//             word = extract_operator(str);
//             if (!word)
//                 break;
                
//             tmp_char = word;
//             word = ft_strtrim_leak(word, "", 951);
//             free(tmp_char);
            
//             if (!word)
//                 break;
                
//             tmp_char = str;
//             str = remove_operator(str, word);
//             free(tmp_char);
            
//             if (str)
//             {
//                 tmp_char = str;
//                 str = ft_strtrim_leak(str, "", 964);
//                 free(tmp_char);
//             }
//         }
//         else if (*str == '$')
//         {
//             word = extract_variable(str);
//             if (!word)
//                 break;
                
//             tmp_char = word;
//             word = ft_strtrim_leak(word, "", 975);
//             free(tmp_char);
            
//             if (!word)
//                 break;
                
//             tmp_char = str;
//             str = first_word_remover(str, word);
//             free(tmp_char);
            
//             if (str)
//             {
//                 tmp_char = str;
//                 str = ft_strtrim_leak(str, "", 988);
//                 free(tmp_char);
//             }
//         }
//         else if (*str == '(')
//         {
//             word = extract_parenthesis(str);
//             if (!word)
//                 break;
                
//             tmp_char = word;
//             word = ft_strtrim_leak(word, "", 999);
//             free(tmp_char);
            
//             if (!word)
//                 break;
                
//             tmp_char = str;
//             str = parenthesis_remover(str, word);
//             free(tmp_char);
            
//             if (str)
//             {
//                 tmp_char = str;
//                 str = ft_strtrim_leak(str, "", 1012);
//                 free(tmp_char);
//             }
//         }
//         else
//         {
//             word = word_extractor(str);
//             if (!word)
//                 break;
                
//             tmp_char = word;
//             trim_result = ft_strtrim_leak(word, "", 1023);
//             free(tmp_char);
//             word = trim_result;
            
//             if (!word)
//                 break;
                
//             tmp_char = str;
//             str = first_word_remover(str, word);
//             free(tmp_char);
            
//             if (str)
//             {
//                 tmp_char = str;
//                 str = ft_strtrim_leak(str, "", 1036);
//                 free(tmp_char);
//             }
//         }
        
//         new_node = malloc(sizeof(t_list));
//         if (!new_node)
//         {
//             free(word);
//             break;
//         }
        
//         new_node->data = word;
//         new_node->next = NULL;
//         new_node->prev = tmp;
//         tmp->next = new_node;
//         tmp = new_node;
//     }
    
//     if (str)
//         free(str);
        
//     return (head);
// }

t_list *list_init(char *str)
{
    char *word = NULL;
    char *tmp_char = NULL;
    t_list *head = NULL;
    t_list *new_node = NULL;
    t_list *tmp = NULL;

    if (!str || !*str)
        return NULL;
    if (*str == '\"')
    {
        word = str_extractor(str);
        if (!word)
            return NULL;
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1092);
        free(tmp_char);
        if (!word)
            return NULL;
        tmp_char = str;
        str = str_remover(str, word);
        free(tmp_char);
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1102);
            free(tmp_char);
        }
    }
    else if (is_operator(*str))
    {
        word = extract_operator(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1113);
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = remove_operator(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1126);
            free(tmp_char);
        }
    }
    else if (*str == '$')
    {
        word = extract_variable(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1137);
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = first_word_remover(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1150);
            free(tmp_char);
        }
    }
    else if (*str == '(')
    {
        word = extract_parenthesis(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1161);
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = parenthesis_remover(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1174);
            free(tmp_char);
        }
    }
    else
    {
        word = word_extractor(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1185);
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = first_word_remover(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1197);
            free(tmp_char);
        }
    }
    head = node_maker(word);
    if (!head)
        return NULL;
    tmp = head;
    while (str && *str)
    {
        if (*str == '\"')
        {
            word = str_extractor(str);
            if (!word)
                break;
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1214);
            free(tmp_char);
            if (!word)
                break;
            tmp_char = str;
            str = str_remover(str, word);
            free(tmp_char);
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1224);
                free(tmp_char);
            }
        }
        else if (is_operator(*str))
        {
            word = extract_operator(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1235);
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = remove_operator(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1248);
                free(tmp_char);
            }
        }
        else if (*str == '$')
        {
            word = extract_variable(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1259);
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = first_word_remover(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1272);
                free(tmp_char);
            }
        }
        else if (*str == '(')
        {
            word = extract_parenthesis(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1283);
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = parenthesis_remover(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1296);
                free(tmp_char);
            }
        }
        else
        {
            word = word_extractor(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1307);
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = first_word_remover(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1320);
                free(tmp_char);
            }
        }
        new_node = malloc(sizeof(t_list));
        if (!new_node)
        {
            free(word);
            break;
        }
        new_node->data = word;
        new_node->next = NULL;
        new_node->prev = tmp;
        tmp->next = new_node;
        tmp = new_node;
    }
    if (str)
        free(str);
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
// void free_list(t_list **head)
// {
//     if (!head || !*head)
//         return;
//     t_list *tmp;
//     t_list *tmp2;

//     tmp = *head;
//     tmp2 = tmp;
//     while (tmp)
//     {
//         tmp = tmp->next;
//         if (tmp2->data)
//             t_free(tmp2->data);
//         t_free(tmp2);
//         tmp2 = tmp;
//     }
//     *head = NULL;
// }

void free_list(t_list **head)
{
    if (!head || !*head)
        return;
    
    t_list *tmp;
    t_list *tmp2;

    tmp = *head;
    while (tmp)
    {
        tmp2 = tmp->next;
        if (tmp->data)
            t_free(tmp->data, 838, "parsing.c");
        t_free(tmp, 839, "parsing.c");
        tmp = tmp2;
    }
    *head = NULL;
}

// char **converter(t_list **head)
// {
// 	char **arr;
// 	int size_list;
// 	int i;
// 	t_list *tmp;

// 	i = 0;
// 	tmp = *head;
// 	size_list = lst_size(head);
// 	arr = t_malloc(sizeof(char *) * (size_list + 1));
// 	while (tmp)
// 	{
// 		arr[i] = tmp->data;
// 		i++;
// 		tmp = tmp->next;
// 	}
// 	arr[i] = NULL;
// 	free_list(head);
// 	return (arr);
// }

void	free_arr(char **arr)
{
	int i = 0;
	while (arr[i])
	{
		t_free(arr[i], 872, "parsing.c");
		i++;
	}
	t_free(arr, 875, "parsing.c");
}

char	*side_maker(t_list **head, int number, int j)
{
	t_list *tmp;
	char *tmp2;
	char *tmp_char;
	int i;

	tmp = *head;
	i = 0;
	tmp2 = NULL;
	while (i < number - j)
	{
		tmp_char = tmp2;
		tmp2 = ft_strjoin_leak(tmp2, tmp->data, 1157);
		t_free(tmp_char, 892, "parsing.c");
		tmp_char = tmp2;
		tmp2 = ft_strjoin_leak(tmp2, " ", 1160);
		t_free(tmp_char, 895, "parsing.c");
		i++;
		tmp = tmp->next;
	}
	return (tmp2);
}

void tree_maker(t_list **head, t_tree **tree)
{
    t_list *tmp;
    t_list *last;
    t_list *current;
    t_list *prev_part;
    t_list *to_free;
    t_tree *command;
    int i;
    int total_nodes;
    *tree = t_malloc(sizeof(t_tree), 912, "parsing.c");
    if (!(*tree))
        return;
	last = NULL;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->command = NULL;
    (*tree)->command_arr = NULL;
    (*tree)->redirections = NULL;
    (*tree)->type = "COMMAND";
    if (!(*head))
        return;
    tmp = *head;
    total_nodes = 0;
    while (tmp)
    {
        last = tmp;
        tmp = tmp->next;
        total_nodes++;
    }
    tmp = last;
    i = total_nodes;
    while (tmp)
    {
        if (ft_strcmp(tmp->token, "OPERATION") == 0)
        {
            command = t_malloc(sizeof(t_tree), 908, "parsing.c");
            if (!command)
                return;
            if (tmp->next)
                command->command = side_maker(&(tmp->next), total_nodes - i, 0);
            else
                command->command = NULL;
            command->left = NULL;
            command->command_arr = NULL;
            command->right = NULL;
            command->redirections = NULL;
            command->type = "COMMAND";
            (*tree)->right = command;
            (*tree)->left = NULL;
            (*tree)->command = NULL;
            (*tree)->command_arr = NULL;
            (*tree)->redirections = NULL;
            (*tree)->type = "OPERATION";
            if (tmp->prev)
            {
                prev_part = *head;
                tmp->prev->next = NULL;
                if (tmp->data)
                    t_free(tmp->data, 961, "parsing.c");
                t_free(tmp, 962, "parsing.c");
                if (prev_part)
                    tree_maker(&prev_part, &(*tree)->left);
            }
            else
            {
                if (tmp->data)
                    t_free(tmp->data, 969, "parsing.c");
                t_free(tmp, 970, "parsing.c");
                *head = NULL;
            }
            return;
        }
        tmp = tmp->prev;
        i--;
    }
    if (*head)
    {
        command = t_malloc(sizeof(t_tree), 950, "parsing.c");
        if (!command)
            return;
        command->command = side_maker(head, total_nodes, 0);
        command->left = NULL;
        command->command_arr = NULL;
        command->redirections = NULL;
        command->right = NULL;
        command->type = "COMMAND";
        (*tree)->left = command;
        (*tree)->right = NULL;
        (*tree)->command_arr = NULL;
        (*tree)->command = NULL;
        (*tree)->redirections= NULL;
        (*tree)->type = "COMMAND";
        current = *head;
        while (current)
        {
            to_free = current;
            current = current->next;
            if (to_free->data)
                t_free(to_free->data, 1001, "parsing.c");
            t_free(to_free, 1002, "parsing.c");
        }
        *head = NULL;
    }
}

t_tree *create_tree_node(void *command, char *type)
{
    t_tree *node = t_malloc(sizeof(t_tree), 1010, "parsing.c");
    if (!node)
        return NULL;
	// printf("DEBUG: create_tree_node with command %p ('%s')\n", 
		// command, command ? (char*)command : "NULL");
    node->command = command;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->redirections = NULL;
    node->command_arr = NULL;
    return (node);
}

// char *extract_command_until_pipe(t_list **head, t_list **pipe_pos)
// {
//     t_list *current = *head;
//     char *command_str = NULL;
//     char *temp;

//     *pipe_pos = NULL;
//     while (current)
//     {
//         if (current->token && ft_strcmp(current->token, "PIPE") == 0)
//         {
//             *pipe_pos = current;
//             break;
//         }
//         if (command_str)
//         {
//             temp = command_str;
//             command_str = ft_strjoin_leak(command_str, " ", 1305);
//             t_free(temp, 1040, "parsing.c");
//             if (command_str)
// 			{
//                 temp = command_str;
//                 command_str = ft_strjoin_leak(command_str, current->data, 1310);
//                 t_free(temp, 1046, "parsing.c");
//             }
//         }
//         else
//         {
//             command_str = ft_strdup(current->data);
//         }
//         current = current->next;
//     }
//     // printf("DEBUG: extract_command_until_pipe returning '%s'\n", command_str ? command_str : "NULL");
//     return (command_str);
// }

// char *extract_command_until_pipe(t_list **head, t_list **pipe_pos) {
//     t_list *current = *head;
//     char *command_str = NULL;
    
//     *pipe_pos = NULL;
    
//     // Find the pipe position without creating any new strings
//     while (current) {
//         if (current->token && ft_strcmp(current->token, "PIPE") == 0) {
//             *pipe_pos = current;
//             break;
//         }
//         current = current->next;
//     }
    
//     // Now just duplicate the first command string
//     if (*head && (*head)->data) {
//         command_str = ft_strdup((*head)->data);
//     } else {
//         command_str = ft_strdup("");
//     }
    
//     return command_str;
// }

// char *extract_command_until_pipe(t_list **head, t_list **pipe_pos) {
//     t_list *current = *head;
//     char *command_str = NULL;
    
//     *pipe_pos = NULL;
    
//     // Find pipe position
//     while (current) {
//         if (current->token && ft_strcmp(current->token, "PIPE") == 0) {
//             *pipe_pos = current;
//             break;
//         }
//         current = current->next;
//     }
    
//     // Just return a duplicate of the first command
//     if (*head && (*head)->data) {
//         command_str = ft_strdup((*head)->data);
//     } else {
//         command_str = ft_strdup("");
//     }
    
//     return command_str;
// }

char *extract_command_with_redirects(t_list **head, t_list **pipe_pos) {
    t_list *current = *head;
    char *command_str = NULL;
    char *temp_str = NULL;
    
    *pipe_pos = NULL;
    
    // Find the pipe position while building the complete command string
    while (current) {
        if (current->token && ft_strcmp(current->token, "PIPE") == 0) {
            *pipe_pos = current;
            break;
        }
        
        // Build the full command string with redirections
        if (command_str) {
            temp_str = command_str;
            command_str = ft_strjoin_leak(command_str, " ", __LINE__);
            t_free(temp_str, __LINE__, "parsing.c");
            
            temp_str = command_str;
            command_str = ft_strjoin_leak(command_str, current->data, __LINE__);
            t_free(temp_str, __LINE__, "parsing.c");
        } else if (current->data) {
            command_str = ft_strdup(current->data);
        }
        
        current = current->next;
    }
    
    // If no command found, return empty string
    if (!command_str) {
        command_str = ft_strdup("");
    }
    
    return command_str;
}


// t_tree *build_pipe_tree(t_list **head)
// {
//     t_list *pipe_pos;
//     t_tree *root = NULL;
//     t_tree *command_node;
//     char *command_str;
//     t_list *next_part;
    
//     if (!head || !(*head))
//         return NULL;
//     command_str = extract_command_until_pipe(head, &pipe_pos);
// 	// printf("DEBUG: build_pipe_tree got command_str '%s'\n", command_str ? command_str : "NULL");
//     if (!pipe_pos)
//     {
//         root = create_tree_node(command_str, "COMMAND");
//         return root;
//     }
//     root = create_tree_node(NULL, "PIPE");
//     command_node = create_tree_node(command_str, "COMMAND");
//     root->left = command_node;
//     if (pipe_pos->next)
//     {
//         next_part = pipe_pos->next;
//         pipe_pos->next = NULL;
//         root->right = build_pipe_tree(&next_part);
//     }
//     return (root);
// }
// t_tree *build_pipe_tree(t_list **head) {
//     t_list *pipe_pos;
//     t_tree *root = NULL;
//     t_tree *command_node;
//     char *command_str;
//     t_list *next_part;
//     t_list *current;
    
//     if (!head || !(*head))
//         return NULL;
        
//     // Extract command string up to the pipe
//     command_str = extract_command_until_pipe(head, &pipe_pos);
    
//     if (!pipe_pos) {
//         // No pipe found, create a simple command node
//         root = create_tree_node(command_str, "COMMAND");
        
//         // Free original list nodes but not their data (already used in the tree)
//         current = *head;
//         while (current) {
//             t_list *temp = current;
//             current = current->next;
//             temp->data = NULL; // Set to NULL so free_list won't free the data
//             t_free(temp, __LINE__, "parsing.c");
//         }
//         *head = NULL;
//         return root;
//     }
    
//     // Create pipe node
//     root = create_tree_node(NULL, "PIPE");
//     command_node = create_tree_node(command_str, "COMMAND");
//     root->left = command_node;
    
//     // Process right side of pipe
//     if (pipe_pos->next) {
//         next_part = pipe_pos->next;
        
//         // Free nodes on the left side of the pipe (but not their data)
//         current = *head;
//         while (current && current != pipe_pos) {
//             t_list *temp = current;
//             current = current->next;
//             temp->data = NULL; // Set to NULL so we don't free data already in the tree
//             t_free(temp, __LINE__, "parsing.c");
//         }
        
//         // Free pipe node
//         if (pipe_pos->data)
//             t_free(pipe_pos->data, __LINE__, "parsing.c");
//         t_free(pipe_pos, __LINE__, "parsing.c");
        
//         // Process right side
//         root->right = build_pipe_tree(&next_part);
//     } else {
//         // Free all nodes in the list (but not their data)
//         current = *head;
//         while (current) {
//             t_list *temp = current;
//             current = current->next;
//             if (temp != pipe_pos) {
//                 temp->data = NULL; // Don't free data already in the tree
//             }
//             t_free(temp, __LINE__, "parsing.c");
//         }
        
//         // No need to free pipe_pos->data as it's already handled in the loop
//     }
    
//     *head = NULL;
//     return root;
// }
// t_tree *build_pipe_tree(t_list **head) {
//     t_list *pipe_pos;
//     t_tree *root = NULL;
//     t_tree *command_node;
//     char *left_cmd_str = NULL;
//     char *right_cmd_str = NULL;
    
//     if (!head || !(*head))
//         return NULL;
    
//     // Find pipe position but don't create any strings yet
//     pipe_pos = NULL;
//     t_list *current = *head;
//     while (current) {
//         if (current->token && ft_strcmp(current->token, "PIPE") == 0) {
//             pipe_pos = current;
//             break;
//         }
//         current = current->next;
//     }
    
//     if (!pipe_pos) {
//         // No pipe found, create a simple command node
//         left_cmd_str = ft_strdup((*head)->data);
//         root = create_tree_node(left_cmd_str, "COMMAND");
//         return root;
//     }
    
//     // Create pipe node
//     root = create_tree_node(NULL, "PIPE");
    
//     // Create left command node with just the first command
//     if (*head && (*head)->data) {
//         left_cmd_str = ft_strdup((*head)->data);
//     } else {
//         left_cmd_str = ft_strdup("");
//     }
//     command_node = create_tree_node(left_cmd_str, "COMMAND");
//     root->left = command_node;
    
//     // Create right command node with just the command after the pipe
//     if (pipe_pos->next && pipe_pos->next->data) {
//         right_cmd_str = ft_strdup(pipe_pos->next->data);
//         root->right = create_tree_node(right_cmd_str, "COMMAND");
//     }
    
//     return root;
// }


// t_tree *build_pipe_tree(t_list **head) {
//     t_list *pipe_pos = NULL;
//     t_tree *root = NULL;
//     t_tree *command_node = NULL;
//     char *left_cmd = NULL;
//     char *right_cmd = NULL;
    
//     if (!head || !(*head))
//         return NULL;
    
//     // Find the pipe position
//     t_list *current = *head;
//     while (current) {
//         if (current->token && ft_strcmp(current->token, "PIPE") == 0) {
//             pipe_pos = current;
//             break;
//         }
//         current = current->next;
//     }
    
//     // Duplicate the left command (before pipe)
//     if (*head && (*head)->data) {
//         left_cmd = ft_strdup((*head)->data);
//     } else {
//         left_cmd = ft_strdup("");
//     }
    
//     // No pipe found, create simple command node
//     if (!pipe_pos) {
//         root = create_tree_node(left_cmd, "COMMAND");
//         return root;
//     }
    
//     // Create pipe node
//     root = create_tree_node(NULL, "PIPE");
    
//     // Create left command node
//     command_node = create_tree_node(left_cmd, "COMMAND");
//     root->left = command_node;
    
//     // Create right command node if pipe has a next part
//     if (pipe_pos->next && pipe_pos->next->data) {
//         right_cmd = ft_strdup(pipe_pos->next->data);
//         root->right = create_tree_node(right_cmd, "COMMAND");
//     }
    
//     return root;
// }

t_tree *build_pipe_tree(t_list **head) {
    t_list *pipe_pos = NULL;
    t_tree *root = NULL;
    t_tree *command_node = NULL;
    char *left_cmd = NULL;
    char *right_cmd = NULL;
    
    if (!head || !(*head))
        return NULL;
    
    // Extract the complete command string (including redirections)
    left_cmd = extract_command_with_redirects(head, &pipe_pos);
    
    if (!pipe_pos) {
        // No pipe found, create a simple command node
        root = create_tree_node(left_cmd, "COMMAND");
        return root;
    }
    
    // Create pipe node
    root = create_tree_node(NULL, "PIPE");
    
    // Create left command node with the full command string
    command_node = create_tree_node(left_cmd, "COMMAND");
    root->left = command_node;
    
    // Process the right part of the pipe
    if (pipe_pos->next) {
        // Extract the right command string by treating pipe_pos->next as head
        t_list *right_part = pipe_pos->next;
        t_list *dummy = NULL;
        
        right_cmd = extract_command_with_redirects(&right_part, &dummy);
        root->right = create_tree_node(right_cmd, "COMMAND");
    }
    
    return root;
}
// void process_command_with_pipes(char *command_str, t_tree **command_tree)
// {
//     t_list *cmd_list;
//     char *cmd_copy;
    
//     if (!command_str || !(*command_str))
//     {
//         *command_tree = NULL;
//         return;
//     }
//     cmd_copy = ft_strdup(command_str);
//     if (!cmd_copy)
//     {
//         *command_tree = NULL;
//         return;
//     }
//     cmd_list = list_init_leak(cmd_copy, 1656);
//     if (cmd_list)
// 	{
//         lexer(&cmd_list);
//     	*command_tree = build_pipe_tree(&cmd_list);
// 		free_list(&cmd_list);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	}
// }
// void process_command_with_pipes(char *command_str, t_tree **command_tree) {
//     t_list *cmd_list;
//     char *cmd_copy;
    
//     if (!command_str || !(*command_str)) {
//         *command_tree = NULL;
//         return;
//     }
    
//     cmd_copy = ft_strdup(command_str);
//     if (!cmd_copy) {
//         *command_tree = NULL;
//         return;
//     }
    
//     cmd_list = list_init_leak(cmd_copy, __LINE__);
//     if (!cmd_list) {
//         t_free(cmd_copy, __LINE__, "parsing.c");
//         *command_tree = NULL;
//         return;
//     }
    
//     // Apply lexical analysis
//     lexer(&cmd_list);
    
//     // Build the pipe tree
//     *command_tree = build_pipe_tree(&cmd_list);
    
//     // The cmd_list should be NULL at this point
//     // If not, we need to clean up any remaining nodes
//     if (cmd_list) {
//         free_list(&cmd_list);
//     }
    
//     // We don't need to free cmd_copy because it's either:
//     // 1. Used in the tree structure
//     // 2. Already freed by list_init_leak if it failed
// }
// Keep build_pipe_tree the same as it was originally, but don't modify cmd_list in it

// void process_command_with_pipes(char *command_str, t_tree **command_tree) {
//     t_list *cmd_list = NULL;
//     char *cmd_copy = NULL;
    
//     if (!command_str || !(*command_str)) {
//         *command_tree = NULL;
//         return;
//     }
    
//     // Make a copy of the original command string
//     cmd_copy = ft_strdup(command_str);
//     if (!cmd_copy) {
//         *command_tree = NULL;
//         return;
//     }
    
//     // Initialize the list using our copy
//     cmd_list = list_init_leak(cmd_copy, __LINE__);
//     if (!cmd_list) {
//         t_free(cmd_copy, __LINE__, "parsing.c");
//         *command_tree = NULL;
//         return;
//     }
    
//     // Apply lexical analysis
//     lexer(&cmd_list);
    
//     // Build the pipe tree but don't modify the list
//     *command_tree = build_pipe_tree(&cmd_list);
    
//     // Now properly free the entire list
//     if (cmd_list) {
//         t_list *temp;
//         while (cmd_list) {
//             temp = cmd_list;
//             cmd_list = cmd_list->next;
            
//             if (temp->data) {
//                 t_free(temp->data, __LINE__, "parsing.c");
//             }
//             t_free(temp, __LINE__, "parsing.c");
//         }
//     }
// }

// void process_command_with_pipes(char *command_str, t_tree **command_tree) {
//     t_list *cmd_list = NULL;
//     t_list *original_head = NULL;
//     char *cmd_copy = NULL;
    
//     if (!command_str || !(*command_str)) {
//         *command_tree = NULL;
//         return;
//     }
    
//     cmd_copy = ft_strdup(command_str);
//     if (!cmd_copy) {
//         *command_tree = NULL;
//         return;
//     }
    
//     cmd_list = list_init_leak(cmd_copy, __LINE__);
//     if (!cmd_list) {
//         t_free(cmd_copy, __LINE__, "parsing.c");
//         *command_tree = NULL;
//         return;
//     }
    
//     // Store the original head for cleanup later
//     original_head = cmd_list;
    
//     // Apply lexical analysis
//     lexer(&cmd_list);
    
//     // Build the pipe tree
//     *command_tree = build_pipe_tree(&cmd_list);
    
//     // Manually clean up all list nodes to prevent memory leaks
//     t_list *current = original_head;
//     t_list *next;
    
//     while (current) {
//         next = current->next;
        
//         // Free the data if it exists
//         if (current->data) {
//             t_free(current->data, __LINE__, "parsing.c");
//             current->data = NULL;
//         }
        
//         // Free the node itself
//         t_free(current, __LINE__, "parsing.c");
//         current = next;
//     }
// }

void process_command_with_pipes(char *command_str, t_tree **command_tree) {
    t_list *cmd_list = NULL;
    t_list *original_head = NULL;
    char *cmd_copy = NULL;
    
    if (!command_str || !(*command_str)) {
        *command_tree = NULL;
        return;
    }
    
    // Make a copy of the command string
    cmd_copy = ft_strdup(command_str);
    if (!cmd_copy) {
        *command_tree = NULL;
        return;
    }
    
    // Initialize the list
    cmd_list = list_init_leak(cmd_copy, __LINE__);
    if (!cmd_list) {
        t_free(cmd_copy, __LINE__, "parsing.c");
        *command_tree = NULL;
        return;
    }
    
    // Store the original head for cleanup
    original_head = cmd_list;
    
    // Apply lexical analysis
    lexer(&cmd_list);
    
    // Build the pipe tree with our improved functions
    *command_tree = build_pipe_tree(&cmd_list);
    
    // Clean up all list nodes
    t_list *current = original_head;
    t_list *next;
    
    while (current) {
        next = current->next;
        
        if (current->data) {
            t_free(current->data, __LINE__, "parsing.c");
            current->data = NULL;
        }
        
        t_free(current, __LINE__, "parsing.c");
        current = next;
    }
}
// void free_tree(t_tree *tree)
// {
// 	int i;

//     if (!tree)
//         return;
//     if (tree->left)
//         free_tree(tree->left);
//     if (tree->right)
//         free_tree(tree->right);
//     // printf("DEBUG: free_tree freeing command: %p ('%s')\n", 
//         //    (void*)tree->command, tree->command ? tree->command : "NULL");
//     if (tree->command)
//         t_free(tree->command, 1122, "parsing.c");
//     if (tree->redirections)
//         t_free(tree->redirections, 1124, "parsing.c");
//     if (tree->command_arr)
// 	{
// 		i = 0;
// 		while (tree->command_arr[i])
// 		{
// 			t_free(tree->command_arr[i], 1130, "parsing.c");
// 			i++;
// 		}
// 		t_free(tree->command_arr, 1133, "parsing.c");
// 	}
//     t_free(tree, 1135, "parsing.c");
// }

void free_tree(t_tree *tree) {
    int i;
    if (!tree)
        return;
        
    if (tree->left)
        free_tree(tree->left);
        
    if (tree->right)
        free_tree(tree->right);
        
    // Free command string if it exists
    if (tree->command) {
        t_free(tree->command, __LINE__, "parsing.c");
        tree->command = NULL;  // Good practice to set to NULL after freeing
    }
    
    // Free redirections if they exist
    if (tree->redirections) {
        t_free(tree->redirections, __LINE__, "parsing.c");
        tree->redirections = NULL;
    }
    
    // Free command array if it exists
    if (tree->command_arr) {
        i = 0;
        while (tree->command_arr[i]) {
            t_free(tree->command_arr[i], __LINE__, "parsing.c");
            tree->command_arr[i] = NULL;
            i++;
        }
        t_free(tree->command_arr, __LINE__, "parsing.c");
        tree->command_arr = NULL;
    }
    
    // Finally free the tree node itself
    t_free(tree, __LINE__, "parsing.c");
}

void process_pipe_trees(t_tree *tree)
{
    t_tree *cmd_tree = NULL;
	t_tree *pipe_node;
	t_tree *right_child;
	t_tree *left_child;
	char *command_str;
    if (!tree)
        return;
    if (tree->command && tree->type && ft_strcmp(tree->type, "COMMAND") == 0)
    {
        command_str = (char *)tree->command;
        if (ft_strchr(command_str, '|'))
        {
            process_command_with_pipes(command_str, &cmd_tree);
            if (cmd_tree)
            {
                if (cmd_tree->type && ft_strcmp(cmd_tree->type, "PIPE") == 0)
                {
                    t_free(tree->command, 1158, "parsing.c");
                    tree->command = NULL;
                    tree->type = cmd_tree->type;
                    if (tree->left)
                        free_tree(tree->left);
                    tree->left = cmd_tree->left;
                    tree->right = cmd_tree->right;
                    cmd_tree->left = NULL;
                    cmd_tree->right = NULL;
                    t_free(cmd_tree, 1167, "parsing.c");
                }
                else
                {
                    free_tree(cmd_tree);
                }
            }
        }
    }
    else if (tree->left && tree->left->command && ft_strchr(tree->left->command, '|'))
    {
        process_pipe_trees(tree->left);
        if (tree->left && ft_strcmp(tree->left->type, "PIPE") == 0 && 
            ft_strcmp(tree->type, "COMMAND") == 0)
        {
            pipe_node = tree->left;
            right_child = pipe_node->right;
            tree->type = pipe_node->type;
            left_child = pipe_node->left;
            tree->left = left_child;
            if (tree->right)
                free_tree(tree->right);
            tree->right = right_child;
            pipe_node->left = NULL;
            pipe_node->right = NULL;
            t_free(pipe_node, 1192, "parsing.c");
        }
    }
    if (tree->left)
        process_pipe_trees(tree->left);
    if (tree->right)
        process_pipe_trees(tree->right);
}


void print_tree_visual(t_tree *tree, int level, int is_left)
{
    if (!tree)
        return;
    for (int i = 0; i < level; i++)
    {
        if (i == level - 1)
            printf("%s", is_left ? "----" : "    ");
        else
            printf("    ");
    }
    printf("%s", tree->type ? tree->type : "NULL");
    if (tree->command)
    {
        printf(" (Command: %s)", tree->command);
    }
    else
    {
        printf(" (Command: NULL)");
    }
    if (tree->command_arr)
    {
		for (int i = 0; tree->command_arr[i]; i++)
		{
        	printf(" (Command_arr: %s)", tree->command_arr[i]);
		}
    }
    else
    {
        printf(" (Command_arr: NULL)");
    }
    if (tree->redirections)
    {
        printf(" [Redirections: %s]", tree->redirections);
    }
    printf("\n");
    if (tree->left || tree->right)
    {
        for (int i = 0; i < level; i++)
            printf("    ");
        printf("---left;\n");
    }
    if (tree->left)
    {
        print_tree_visual(tree->left, level + 1, 1);
    }
    if (tree->right)
    {
        for (int i = 0; i < level; i++)
            printf("    ");
        printf("---right;\n");
    }
    if (tree->right)
    {
        print_tree_visual(tree->right, level + 1, 0);
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
	while (command[i] && j > 0)
	{
		if (command[i] == '(')
			j++;
		else if (command[i] == ')')
			j--;
		i++;
	}
	if (j != 0)
		return (NULL);
	end = i - 1;
	content = ft_substr_leak(command, start, end - start, 1573);
	return (content);
}

int	has_outer_parentheses(char *command)
{
	int	i;
	int	len;

	if (!command)
		return (0);
	len = ft_strlen(command);
	i = 0;
	while (command[i] == ' ')
		i++;
	if (command[i] != '(')
		return (0);
	i = len - 1;
	while (i >= 0 && command[i] == ' ')
		i--;
	if (i < 0 || command[i] != ')')
		return (0);
	return (1);
}

void process_parentheses_in_tree(t_tree *tree)
{
    t_tree   *subtree = NULL;
    t_list   *cmd_list = NULL;
    char     *inner_cmd = NULL;

    if (!tree)
        return;
    if (tree->command && tree->type && ft_strcmp(tree->type, "COMMAND") == 0)
    {
        if (has_outer_parentheses(tree->command))
        {
            inner_cmd = extract_content_from_parentheses(tree->command);
            if (inner_cmd)
            {
                char *inner_cmd_copy = ft_strdup(inner_cmd);
                if (inner_cmd_copy)
                {
                    cmd_list = list_init_leak(inner_cmd_copy, 1886);
                    if (cmd_list)
                    {
                        lexer(&cmd_list);
                        tree_maker(&cmd_list, &subtree);
                        if (subtree)
                        {
                            process_pipe_trees(subtree);
                            t_free(tree->command, 1339, "parsing.c");
                            tree->command = NULL;
                            if (subtree->left)
                            {
                                tree->type = subtree->type;
                                if (tree->left)
                                    free_tree(tree->left);
                                if (tree->right)
                                    free_tree(tree->right);
                                tree->left = subtree->left;
                                tree->right = subtree->right;
                                subtree->left = NULL;
                                subtree->right = NULL;
                            }
                            else if (subtree->command)
                            {
                                tree->command = subtree->command;
                                subtree->command = NULL;
                            }
                            t_free(subtree, 1358, "parsing.c");
                        }
                    }
                }
                t_free(inner_cmd, 1362, "parsing.c");
            }
        }
    }
    if (tree->left)
        process_parentheses_in_tree(tree->left);
    if (tree->right)
        process_parentheses_in_tree(tree->right);
}

void fix_operation_tree_structure(t_tree *tree) 
{
    if (!tree)
        return;
    if (ft_strcmp(tree->type, "OPERATION") == 0) 
    {
        if (tree->left && ft_strcmp(tree->left->type, "COMMAND") == 0) 
        {
            if (tree->left->left)
            {
                t_tree *left_cmd_node = tree->left;
                t_tree *nested_node = left_cmd_node->left;
                tree->left = nested_node;
                left_cmd_node->left = NULL;
                free_tree(left_cmd_node);
            }
        }
        fix_operation_tree_structure(tree->left);
        fix_operation_tree_structure(tree->right);
    }
    else {
        if (tree->left) 
            fix_operation_tree_structure(tree->left);
        if (tree->right) 
            fix_operation_tree_structure(tree->right);
    }
}
void process_all_parentheses(t_tree *tree)
{
	t_tree *subtree;
	char *inner_cmd;
	t_list *cmd_list;
    if (!tree)
        return;
    if (tree->command && tree->type && ft_strcmp(tree->type, "COMMAND") == 0)
	{
        if (has_outer_parentheses(tree->command))
		{
            subtree = NULL;
            inner_cmd = extract_content_from_parentheses(tree->command);
            if (inner_cmd)
			{
                cmd_list = list_init_leak(ft_strdup(inner_cmd), 1969);
                if (cmd_list)
				{
                    lexer(&cmd_list);
                    tree_maker(&cmd_list, &subtree);
                    if (subtree)
					{
                        process_pipe_trees(subtree);
                        t_free(tree->command, 1422, "parsing.c");
                        tree->command = NULL;
                        tree->type = subtree->type;
                        if (tree->left)
                            free_tree(tree->left);
                        if (tree->right)
                            free_tree(tree->right);
                        tree->left = subtree->left;
                        tree->right = subtree->right;
                        subtree->left = NULL;
                        subtree->right = NULL;
                        t_free(subtree, 1433, "parsing.c");
                    }
                }
                t_free(inner_cmd, 1436, "parsing.c");
            }
        }
    }
    if (tree->left)
        process_all_parentheses(tree->left);
    if (tree->right)
        process_all_parentheses(tree->right);
}

void process_nested_parentheses(t_tree **tree)
{
    if (!tree || !(*tree))
        return;
    int passes = 0;
    int max_passes = 5;
    while (passes < max_passes)
    {
        process_all_parentheses(*tree);
        fix_operation_tree_structure(*tree);
        passes++;
    }
}

char *new_command_maker(char *str, int wordlen)
{
	int i;
	int j;
	int strlenth;
	char *new_str;
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	strlenth = ft_strlen(str);
	new_str = t_malloc(sizeof(char) * (strlenth - wordlen + 1), 1471, "parsing.c");
	while (str[j] != '>')
	{
		new_str[i] = str[j];
		i++;
		j++;
	}
	j += wordlen;
	while (str[j])
	{
		new_str[i] = str[j];
		i++;
		j++;
	}
	new_str[i] = '\0';
	return (new_str);
}
int remove_reds(char *str, char c)
{
	int i;

	if (!str || !c)
		return (0);
	i = 0;
	while (str[i] != '>' || str[i] != '<')
		i++;
	while (str[i] == '>' || str[i] != '<')
		i++;
	while (str[i] == 32 && str[i] != '\0')
		i++;
	while (str[i] != 32 && str[i] != '\0')
		i++;
	return (i);
}

void process_all_redirections(t_tree **tree)
{
    char *cmd;
    char *redir_out;
    char *redir_in;
    char *first_redir;
    char *cmd_prefix;
    char *command_part;
    int cmd_length;

    if (!tree || !*tree)
        return;
    if ((*tree)->left)
        process_all_redirections(&(*tree)->left);
    if ((*tree)->right)
        process_all_redirections(&(*tree)->right);
    if (*tree && (*tree)->command)
    {
        cmd = (*tree)->command;
        redir_out = ft_strchr(cmd, '>');
        redir_in = ft_strchr(cmd, '<');
        if (redir_out || redir_in)
        {
            if (!redir_out)
                first_redir = redir_in;
            else if (!redir_in)
                first_redir = redir_out;
            else
            {
                if (redir_out < redir_in)
                    first_redir = redir_out;
                else
                    first_redir = redir_in;
            }
            cmd_length = first_redir - cmd;
            if (cmd_length == 0)
            {
                (*tree)->redirections = ft_strdup(first_redir);
                char *after_redir = first_redir;
                while (*after_redir && (*after_redir == '>' || *after_redir == '<'))
                    after_redir++;
                while (*after_redir && *after_redir == ' ')
                    after_redir++;
                while (*after_redir && *after_redir != ' ')
                    after_redir++;
                while (*after_redir && *after_redir == ' ')
                    after_redir++;
                if (*after_redir)
                    command_part = ft_strdup(after_redir);
                else
                    command_part = ft_strdup("");
                t_free((*tree)->command, 1527, "parsing.c");
                (*tree)->command = command_part;
            }
            else
            {
                cmd_prefix = t_malloc(sizeof(char) * (cmd_length + 1), 1562, "parsing.c");
                if (cmd_prefix)
                {
                    ft_strncpy(cmd_prefix, cmd, cmd_length);
                    cmd_prefix[cmd_length] = '\0';
                    (*tree)->redirections = ft_strdup(first_redir);
                    t_free((*tree)->command, 1568, "parsing.c");
                    (*tree)->command = cmd_prefix;
                }
            }
        }
    }
}

int	check_quotes(char *str)
{
	int i;
	int in_quotes;
	char quote_type;

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

int	skip_quotes(char *str, int i)
{
	char quote_type;

	quote_type = str[i];
	i++;
	while (str[i] && str[i] != quote_type)
		i++;
	if (str[i])
		i++;
	return (i);
}
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
int check_empty(char *str)
{
	int i;

	i = 0;
	while (str[i] == 32)
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	redirections_opener(t_tree **tree, t_list_fd **head)
{
	int i;
	int j;
	t_list_fd *tmp;
	t_list_fd *tmp2;
	t_list_fd *new_node;
	char *tmp_char;
	int flag;

	i = 0;
	flag = 0;
	if ((*tree)->left)
		redirections_opener(&(*tree)->left, head);
	if ((*tree)->right)
		redirections_opener(&(*tree)->right, head);
	if ((*tree)->redirections)
	{
		if (!*head)
		{
			while (((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<') && (*tree)->redirections[i] != '\0') 
				i++;
			if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == 32)
				flag = 1;
			if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == '>')
				flag = 2;
			if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == 32)
				flag = 3;
			if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == '<')
				flag = 4;
			while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && (*tree)->redirections[i] != '\0')
				i++;
			while ((*tree)->redirections[i] == 32 && (*tree)->redirections[i] != '\0')
				i++;
			j = i;
			while ((*tree)->redirections[i] != 32 && (*tree)->redirections[i] != '\0')
				i++;
			*head = t_malloc(sizeof(t_list_fd), 1768, "parsing.c");
			// tmp_char = (*head)->name;
			(*head)->name = ft_substr_leak((*tree)->redirections, j, i - j, 2055);
			// t_free(tmp_char, 1771, "parsing.c");
			(*head)->redir = NULL;
			(*head)->next = NULL;
			(*head)->fd = -1;
			(*head)->command = ft_strdup((*tree)->command);
			tmp_char = (*tree)->redirections;
			(*tree)->redirections = ft_substr_leak((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i, 2062);
			t_free(tmp_char, 1778, "parsing.c");
			tmp_char = (*tree)->redirections;
			(*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", 1779);
			t_free(tmp_char, 1781, "parsing.c");
			if (flag == 1 && !check_empty((*head)->name))
			{
				(*head)->fd = open((*head)->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if ((*head)->fd == -1)
				{
					write(2, "invalid file\n", 13);
					new_node = NULL;
					if (!*head)
						*head = new_node;
					free_list_fd(head);
					exit (1);
				}
				(*head)->redir = ft_strdup(">");
			}
			if (flag == 2 && !check_empty((*head)->name))
			{
				(*head)->fd = open((*head)->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
				if ((*head)->fd == -1)
				{
					write(2, "invalid file\n", 13);
					new_node = NULL;
					if (!*head)
						*head = new_node;
					free_list_fd(head);
					exit (1);
				}
				(*head)->redir = ft_strdup(">>");
			}
			if (flag == 3)
			{
				(*head)->fd = open((*head)->name, O_RDONLY, 0644);
				if ((*head)->fd == -1)
				{
					write(2, "invalid file\n", 13);
					new_node = NULL;
					if (!*head)
						*head = new_node;
					free_list_fd(head);
					exit (1);
				}
				(*head)->redir = ft_strdup("<");
			}
			if (flag == 4)
			{
				(*head)->fd = open((*head)->name, O_RDONLY, 0644);
				if ((*head)->fd == -1)
				{
					write(2, "invalid file\n", 13);
					new_node = NULL;
					if (!*head)
						*head = new_node;
					free_list_fd(head);
					exit (1);
				}
				(*head)->redir = ft_strdup("<<");
			}
			(*head)->next = NULL;
			tmp = *head;
			while ((*tree)->redirections && !check_empty((*tree)->redirections))
			{
				i = 0;
				while ((*tree)->redirections != NULL && (*tree)->redirections[i] && ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
					i++;
				if (!(*tree)->redirections[i])
					break;
				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == 32)
					flag = 1;
				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == '>')
					flag = 2;
				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == 32)
					flag = 3;
				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == '<')
					flag = 4;
				while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && (*tree)->redirections[i] != '\0')
					i++;
				while ((*tree)->redirections[i] == 32 && (*tree)->redirections[i] != '\0')
					i++;
				j = i;
				while ((*tree)->redirections[i] != 32 && (*tree)->redirections[i] != '\0')
					i++;
				new_node = t_malloc(sizeof(t_list_fd), 1832, "parsing.c");
				tmp_char = new_node->name;
				new_node->name = ft_substr_leak((*tree)->redirections, j, i - j, 2149);
				t_free(tmp_char, 1865, "parsing.c");
				new_node->fd = -1;
				new_node->redir = NULL;
				new_node->next = NULL;
				new_node->command = ft_strdup((*tree)->command);
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_substr_leak((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i, 2156);
				t_free(tmp_char, 1872, "parsing.c");
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", 1873);
				t_free(tmp_char, 1875, "parsing.c");
				if (flag == 1 && !check_empty(new_node->name))
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 1886, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 1888, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 1890, "parsing.c");
						t_free(new_node, 1891, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup(">");
				}
				if (flag == 2 && !check_empty(new_node->name))
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 1904, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 1906, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 1908, "parsing.c");
						t_free(new_node, 1909, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup(">>");
				}
				if (flag == 3)
				{
					new_node->fd = open(new_node->name, O_RDONLY,  0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 1922, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 1924, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 1926, "parsing.c");
						t_free(new_node, 1927, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup("<");
				}
				if (flag == 4)
				{
					new_node->fd = open(new_node->name, O_RDONLY, 0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 1940, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 1942, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 1944, "parsing.c");
						t_free(new_node, 1945, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup("<<");
				}
				new_node->next = NULL;
				tmp->next = new_node;
				tmp = new_node;
				if (check_empty((*tree)->redirections) == 1)
					break ;
			}
		}
		else if (head && *head)
		{
			tmp2 = *head;
			while (tmp2->next)
				tmp2 = tmp2->next;
			tmp = tmp2;
			while ((*tree)->redirections && !check_empty((*tree)->redirections))
			{
				i = 0;
				while ((*tree)->redirections != NULL && (*tree)->redirections[i] && ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
					i++;
				if (!(*tree)->redirections[i])
					break;
				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == 32)
					flag = 1;
				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == '>')
					flag = 2;
				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == 32)
					flag = 3;
				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == '<')
					flag = 4;
				while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && (*tree)->redirections[i] != '\0')
					i++;
				while ((*tree)->redirections[i] == 32 && (*tree)->redirections[i] != '\0')
					i++;
				j = i;
				while ((*tree)->redirections[i] != 32 && (*tree)->redirections[i] != '\0')
					i++;
				new_node = t_malloc(sizeof(t_list_fd), 1956, "parsing.c");
				// tmp_char = new_node->name;
				new_node->name = ft_substr_leak((*tree)->redirections, j, i - j, 2270);
				// t_free(tmp_char, 1989, "parsing.c");
				new_node->fd = -1;
				new_node->redir = NULL;
				new_node->next = NULL;
				new_node->command = ft_strdup((*tree)->command);
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_substr_leak((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i, 2277);
				t_free(tmp_char, 1996, "parsing.c");
				tmp_char = (*tree)->redirections;
				(*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", 1994);
				t_free(tmp_char, 1999, "parsing.c");
				if (flag == 1 && !check_empty(new_node->name))
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 2007, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 2009, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 2011, "parsing.c");
						t_free(new_node, 2012, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup(">");
				}
				if (flag == 2 && !check_empty(new_node->name))
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 2025, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 2027, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 2029, "parsing.c");
						t_free(new_node, 2030, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup(">>");
				}
				if (flag == 3)
				{
					new_node->fd = open(new_node->name, O_RDONLY,  0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 2043, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 2045, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 2047, "parsing.c");
						t_free(new_node, 2048, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup("<");
				}
				if (flag == 4)
				{
					new_node->fd = open(new_node->name, O_RDONLY, 0644);
					if (new_node->fd == -1)
					{
						write(2, "invalid file\n", 13);
						if (new_node->name)
							t_free(new_node->name, 2061, "parsing.c");
						if (new_node->command)
							t_free(new_node->command, 2063, "parsing.c");
						if (new_node->redir)
							t_free(new_node->redir, 2065, "parsing.c");
						t_free(new_node, 2066, "parsing.c");
						free_list_fd(head);
						exit (1);
					}
					new_node->redir = ft_strdup("<<");
				}
				new_node->next = NULL;
				tmp->next = new_node;
				tmp = new_node;
				if (check_empty((*tree)->redirections) == 1)
					break ;
			}
		}
	}
}

int variable_search(t_list **head)
{
    t_list *tmp;

    tmp = *head;
    while (tmp)
    {
        if (!ft_strcmp(tmp->token, "VARIABLE"))
            break ;
        tmp = tmp->next;
    }
    if (tmp)
        return (1);
    return (0);
}
// void    variable_expantion(t_list **head, char **ev)
// {
//     t_list *tmp;
//     int i;
//     char *variable_name;
// 	char *tmp_char;

//     tmp = *head;
//     i = 0;
//     while (tmp && ft_strcmp(tmp->token, "VARIABLE"))
//         tmp = tmp->next;
// 	if (tmp)
//     	variable_name = ft_substr_leak(tmp->data, 1, ft_strlen(tmp->data) - 1);
//     while (ev[i])
//     {
//         if (tmp && !ft_strncmp(ev[i], variable_name, ft_strlen(variable_name)) && ev[i][ft_strlen(variable_name)] == '=')
//         {
// 			tmp_char = tmp->data;
//             tmp->data = ft_substr_leak(ev[i], ft_strlen(variable_name), ft_strlen(ev[i]) - ft_strlen(variable_name));
// 			t_free(tmp_char);
// 			tmp_char = tmp->data;
//             tmp->data = ft_strtrim(tmp->data, "=");
// 			t_free(tmp_char);
//         }
//         i++;
//     }
// }

void variable_expantion(t_list **head, char **ev)
{
    t_list *tmp;
    int i;
    char *variable_name = NULL;
    char *tmp_char;

    tmp = *head;
    i = 0;
    while (tmp && ft_strcmp(tmp->token, "VARIABLE"))
        tmp = tmp->next;
    
    if (tmp)
    {
        variable_name = ft_substr_leak(tmp->data, 1, ft_strlen(tmp->data) - 1, 2421);
        
        while (ev[i])
        {
            if (!ft_strncmp(ev[i], variable_name, ft_strlen(variable_name)) && ev[i][ft_strlen(variable_name)] == '=')
            {
                tmp_char = tmp->data;
                tmp->data = ft_substr_leak(ev[i], ft_strlen(variable_name), ft_strlen(ev[i]) - ft_strlen(variable_name), 2428);
                t_free(tmp_char, 2147, "parsing.c");
                
                tmp_char = tmp->data;
                tmp->data = ft_strtrim_leak(tmp->data, "=", 2146);
                t_free(tmp_char, 2151, "parsing.c");
            }
            i++;
        }
        t_free(variable_name, 2157, "parsing.c");
    }
}

int	check_for_variable(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	if (!str[i])
		return (0);
	return (1);
}

int	variable_in_word(t_list **head, char **argev)
{
    t_list *tmp;
	int i;
	int j;
	int k;
	int l;
	char *new_str;
	char *tmp_word;
	char *first_part;
	char *tmp_char;
	char *second_part;
	char quote_type;
	int in_quote;

    tmp = *head;
	in_quote = 0;
    while (tmp)
    {
        if (!ft_strcmp(tmp->token, "WORD"))
        {
			if (check_for_variable(tmp->data))
			{
				i = 0;
				while (tmp->data[i])
				{
					if (in_quote == 0 && (tmp->data[i] == '"' || tmp->data[i] == '\''))
					{
						quote_type = tmp->data[i];
						in_quote = 1;
					}
					else if (in_quote && tmp->data[i] == quote_type)
					{
						in_quote = 0;
						quote_type = '\0';
					}
					if ((tmp->data[i] == '$' && in_quote && quote_type == '"') || (tmp->data[i] == '$' && !in_quote))
					{
						j = i;
						l = i;
						j++;
						if ((tmp->data[j] >= '0' && tmp->data[j] <= '9') || !tmp->data[j])
							return (-1);
						while ((tmp->data[j] >= 'a' && tmp->data[j] <= 'z') || (tmp->data[j] >= 'A' && tmp->data[j] <= 'Z') || (tmp->data[j] >= '0' && tmp->data[j] <= '9'))
							j++;
						l++;
						tmp_char = tmp_word;
						tmp_word = ft_substr_leak(tmp->data, l, j - l, 2506);
						t_free(tmp_char, 2227, "parsing.c");
						k = 0;
						while (argev[k] && ft_strncmp(argev[k], tmp_word, ft_strlen(tmp_word)))
							k++;
						if (!argev[k])
						{
							t_free(tmp_word, 2233, "parsing.c");
							tmp_char = first_part;
							first_part = ft_substr_leak(tmp->data, 0, i, 2515);
							t_free(tmp_char, 2236, "parsing.c");
							tmp_char = second_part;
							second_part = ft_substr_leak(tmp->data, j, ft_strlen(tmp->data) - j, 2518);
							t_free(tmp_char, 2239, "parsing.c");
							tmp_char = new_str;
							new_str = ft_strjoin_leak(first_part, second_part, 2501);
							t_free(tmp_char, 2242, "parsing.c");
							t_free(tmp->data, 2243, "parsing.c");
							tmp->data = new_str;
						}
						else
						{
							t_free(tmp_word, 2248, "parsing.c");
							l = 0;
							while (argev[k][l] && argev[k][l] != '=')
								l++;
							tmp_char = tmp_word;
							tmp_word = ft_substr_leak(argev[k], l, ft_strlen(argev[k]) - l, 2533);
							t_free(tmp_char, 2254, "parsing.c");
							tmp_char = tmp_word;
							tmp_word = ft_strtrim_leak(tmp_word, "=", 2252);
							t_free(tmp_char, 2257, "parsing.c");
							tmp_char = first_part;
							first_part = ft_substr_leak(tmp->data, 0, i, 2539);
							t_free(tmp_char, 2260, "parsing.c");
							tmp_char = second_part;
							second_part = ft_substr_leak(tmp->data, j, ft_strlen(tmp->data) - j, 2542);
							t_free(tmp_char, 2263, "parsing.c");
							tmp_char = new_str; 
							new_str = ft_strjoin_leak(first_part, tmp_word, 2525);
							t_free(tmp_char, 2266, "parsing.c");
							tmp_char = new_str;
							new_str = ft_strjoin_leak(new_str, second_part, 2528);
							t_free(tmp_char, 2269, "parsing.c");
							t_free(tmp->data, 2270, "parsing.c");
							tmp->data = new_str;
						}
					}
					i++;
				}
			}
		}
        tmp = tmp->next;
    }
	return (0);
}

int	variable_search_two(char *str, int j, int k)
{
	int i;

	i = j;
	while (i < k)
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	if (str[i] == '$')
		return (1);
	return (0);
}

void	quote_remove_two(t_tree **tree)
{
	int i;
	int j;
	int k;
	char *old_str;
	char *new_str;
	int len;
	int final_len;
	int in_single_quotes;
	int in_double_quotes;
	if ((*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree)->command_arr)
	{
		k = 0;
		while ((*tree)->command_arr[k])
		{
			old_str = (*tree)->command_arr[k];
			len = ft_strlen(old_str);
			final_len = 0;
			in_single_quotes = 0;
			in_double_quotes = 0;
			i = 0;
			while (i < len)
			{
				if (old_str[i] == '\'' && !in_double_quotes)
				{
					in_single_quotes = !in_single_quotes;
				}
				else if (old_str[i] == '\"' && !in_single_quotes)
				{
					in_double_quotes = !in_double_quotes;
				}
				else
				{
					final_len++;
				}
				i++;
			}
			new_str = (char *)t_malloc(sizeof(char) * (final_len + 1), 2341, "parsing.c");
			if (!new_str)
			{
				k++;
				continue;
			}
			j = 0;
			in_single_quotes = 0;
			in_double_quotes = 0;
			i = 0;
			
			while (i < len)
			{
				if (old_str[i] == '\'' && !in_double_quotes)
				{
					in_single_quotes = !in_single_quotes;
				}
				else if (old_str[i] == '\"' && !in_single_quotes)
				{
					in_double_quotes = !in_double_quotes;
				}
				else
				{
					new_str[j] = old_str[i];
					j++;
				}
				i++;
			}
			new_str[j] = '\0';
			t_free((*tree)->command_arr[k], 2370, "parsing.c");
			(*tree)->command_arr[k] = new_str;
			k++;
		}
	}
}

void	command_arr_fill(t_tree **tree)
{
	t_list	*head;
	char	*str;
	char	**arr;
	int		i;
	int	list_size;
	t_list	*tmp;
	if ((*tree)->left)
		command_arr_fill(&(*tree)->left);
	if ((*tree)->right)
		command_arr_fill(&(*tree)->right);
	if ((tree) && (*tree) && (*tree)->command && !(*tree)->command_arr)
	{
		i = 0;
		str = ft_strdup((*tree)->command);
		if (!str || !*str)
        {
            arr = t_malloc(sizeof(char *), 2395, "parsing.c");
            if (arr)
                arr[0] = NULL;
            (*tree)->command_arr = arr;
            t_free(str, 2399, "parsing.c");
            return;
        }
		head = list_init_leak(str, 2952);
		list_size = lst_size(&head);
		arr = t_malloc(sizeof(char *) * (list_size + 1), 2404, "parsing.c");
		tmp = head;
		while (tmp)
		{
			arr[i] = ft_strdup(tmp->data);
			i++;
			tmp = tmp->next;
		}
		arr[i] = NULL;
		free_list(&head);
		(*tree)->command_arr = arr;
	}
}

t_env	*env_fill(char **argev)
{
	int i;
	int j;
	t_env	*head;
	t_env	*new_node;
	t_env	*tmp;

	i = 0;
	j = 0;
	head = t_malloc(sizeof(t_env), 2428, "parsing.c");
	while (argev[i] && argev[i][j] != '=')
		j++;
	head->key = ft_substr_leak(argev[i],0, j, 2711);
	head->value = ft_substr_leak(argev[i], (j + 1), ft_strlen(argev[i]) - (j + 1), 2712);
	j = 0;
	head->next = NULL;
	tmp = head;
	i++;
	while (argev[i])
	{
		new_node = t_malloc(sizeof(t_env), 2439, "parsing.c");
		while (argev[i] && argev[i][j] != '=')
			j++;
		new_node->key = ft_substr_leak(argev[i],0, j, 2722);
		new_node->value = ft_substr_leak(argev[i], (j + 1), ft_strlen(argev[i]) - (j + 1), 2723);
		j = 0;
		new_node->next = NULL;
		tmp->next = new_node;
		tmp = new_node;
		i++;
	}
	return (head);
}

void	free_env(t_env **env)
{
	t_env *tmp;
	t_env *tmp2;

	tmp = *env;
	while (tmp)
	{
		tmp2 = tmp->next;
		t_free(tmp->key, 2462, "parsing.c");
		t_free(tmp->value, 2463, "parsing.c");
		t_free(tmp, 2464, "parsing.c");
		tmp = tmp2;
	}
	*env = NULL;
}

void	f(void)
{
	system("leaks -q minishell");
}

int main(int argc, char **argv, char **argev)//wildcards //remove quotes from command_arr
{
	atexit(f);
	char *str;
	char *tmp_str;
	t_list *head;
	t_env *env;
	t_env *tmp_env;
	t_tree *tree;
	t_list *tmp;
	t_list_fd *tmp_fd;
	t_list_fd *head_fd;
	int i;

	(void)argc;
	(void)argv;
	head_fd = NULL;
	head = NULL;
	env = NULL;
	while (1)
	{
		str = readline("minishell$> ");
		// str = argv[1];
		i = 0;
		if (!str)
			break;
		else if (!*str)
		{
			free(str);
			continue ;
		}
		env = env_fill(argev);
		tmp_env = env;
		// while (tmp_env)
		// {
		// 	printf("key   = %s\n", tmp_env->key);
		// 	printf("value = %s\n", tmp_env->value);
		// 	printf("\n");
		// 	tmp_env = tmp_env->next;
		// }
		add_history(str);
		str = replace_whites_spaces(str);
		tmp_str = str;
		str = ft_strtrim_leak(str, " ", 2509);
		t_free(tmp_str, 2514, "parsing.c");
		if (check_quotes(str) == 1)
			return (1);
		head = list_init_leak(str, 3072);
		lexer(&head);
		tmp = head;
		// while (tmp)
		// {
		// 	printf("%s\n", tmp->data);
		// 	printf("%s\n", tmp->token);
		// 	printf("\n");
		// 	tmp = tmp->next;
		// }
		if (variable_search(&head))
            variable_expantion(&head, argev);
		variable_in_word(&head, argev);
		tmp = head;
		// while (tmp)
		// {
		// 	printf("%s\n", tmp->data);
		// 	printf("%s\n", tmp->token);
		// 	printf("\n");
		// 	tmp = tmp->next;
		// }
		// quote_remove(&head);
		syntax_error(&head);
		tree_maker(&head, &tree);
		// print_tree_visual(tree, 1, 1);
		process_pipe_trees(tree);
		// print_tree_visual(tree, 1, 1);
		process_nested_parentheses(&tree);
		process_all_redirections(&tree);
		command_arr_fill(&tree);
		quote_remove_two(&tree);
		// printf("\n");
		print_tree_visual(tree, 1, 1);
		syntax_error_two(&tree);
		redirections_opener(&tree, &head_fd);
		tmp_fd = head_fd;
		// while (tmp_fd)
		// {
		// 	printf("filename == %s\n", tmp_fd->name);
		// 	printf("filedescriptor == %d\n", tmp_fd->fd);
		// 	printf("command == %s\n", tmp_fd->command);
		// 	printf("redir == %s\n", tmp_fd->redir);
		// 	printf("\n");
		// 	tmp_fd = tmp_fd->next;
		// }
		free_env(&env);
		if (tree)
			free_tree(tree);
		if (head_fd)
			free_list_fd(&head_fd);
		if (head)
			free_list(&head);
		// t_free(str, 2567, "parsing.c");
	}
}
