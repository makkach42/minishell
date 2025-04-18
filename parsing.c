/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/18 11:57:14 by makkach          ###   ########.fr       */
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
        word = ft_strtrim_leak(word, " ", 1092, "list_init");
        free(tmp_char);
        if (!word)
            return NULL;
        tmp_char = str;
        str = str_remover(str, word);
        free(tmp_char);
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1102, "list_init");
            free(tmp_char);
        }
    }
    else if (is_operator(*str))
    {
        word = extract_operator(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1113, "list_init");
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = remove_operator(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1126, "list_init");
            free(tmp_char);
        }
    }
    else if (*str == '$')
    {
        word = extract_variable(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1137, "list_init");
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = first_word_remover(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1150, "list_init");
            free(tmp_char);
        }
    }
    else if (*str == '(')
    {
        word = extract_parenthesis(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1161, "list_init");
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = parenthesis_remover(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1174, "list_init");
            free(tmp_char);
        }
    }
    else
    {
        word = word_extractor(str);
        if (!word)
            return NULL;
            
        tmp_char = word;
        word = ft_strtrim_leak(word, " ", 1185, "list_init");
        free(tmp_char);
        
        if (!word)
            return NULL;
            
        tmp_char = str;
        str = first_word_remover(str, word);
        free(tmp_char);
        
        if (str)
        {
            tmp_char = str;
            str = ft_strtrim_leak(str, " ", 1197, "list_init");
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
            word = ft_strtrim_leak(word, " ", 1214, "list_init");
            free(tmp_char);
            if (!word)
                break;
            tmp_char = str;
            str = str_remover(str, word);
            free(tmp_char);
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1224, "list_init");
                free(tmp_char);
            }
        }
        else if (is_operator(*str))
        {
            word = extract_operator(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1235, "list_init");
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = remove_operator(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1248, "list_init");
                free(tmp_char);
            }
        }
        else if (*str == '$')
        {
            word = extract_variable(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1259, "list_init");
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = first_word_remover(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1272, "list_init");
                free(tmp_char);
            }
        }
        else if (*str == '(')
        {
            word = extract_parenthesis(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1283, "list_init");
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = parenthesis_remover(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1296, "list_init");
                free(tmp_char);
            }
        }
        else
        {
            word = word_extractor(str);
            if (!word)
                break;
                
            tmp_char = word;
            word = ft_strtrim_leak(word, " ", 1307, "list_init");
            free(tmp_char);
            
            if (!word)
                break;
                
            tmp_char = str;
            str = first_word_remover(str, word);
            free(tmp_char);
            
            if (str)
            {
                tmp_char = str;
                str = ft_strtrim_leak(str, " ", 1320, "list_init");
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


t_tree *create_tree_node(void *command, char *type)
{
    t_tree *node = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
    if (!node)
        return NULL;
	
		
    node->command = command;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->redirections = NULL;
    node->command_arr = NULL;
    return (node);
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
            t_free(tmp->data, 1388, "parsing.c");
        t_free(tmp, 1389, "parsing.c");
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
		t_free(arr[i], 1422, "parsing.c");
		i++;
	}
	t_free(arr, 1425, "parsing.c");
}

// char	*side_maker(t_list **head, int number, int j)
// {
// 	t_list *tmp;
// 	char *tmp2;
// 	char *tmp_char;
// 	int i;

// 	tmp = *head;
// 	i = 0;
// 	tmp2 = NULL;
// 	while (i < number - j)
// 	{
// 		tmp_char = tmp2;
// 		tmp2 = ft_strjoin_leak(tmp2, tmp->data, 1441);
// 		t_free(tmp_char, 1442, "parsing.c");
// 		tmp_char = tmp2;
// 		tmp2 = ft_strjoin_leak(tmp2, " ", 1444);
// 		t_free(tmp_char, 1445, "parsing.c");
// 		i++;
// 		tmp = tmp->next;
// 	}
// 	return (tmp2);
// }

char *side_maker(t_list **head, int number, int j) {
    t_list *tmp;
    char *tmp2;
    char *tmp_char;
    int i;
    
    tmp = *head;
    i = 0;
    tmp2 = NULL;
    
    while (i < number - j) {
        // Add the current token
        tmp_char = tmp2;
        tmp2 = ft_strjoin_leak(tmp2, tmp->data, 1441);
        t_free(tmp_char, 1442, "parsing.c");
        
        // Add space only if not a redirection operator followed by a word
        if (tmp->next && 
            !(ft_strcmp(tmp->token, "REDIRECTION") == 0 && 
              ft_strcmp(tmp->next->token, "WORD") == 0)) {
            tmp_char = tmp2;
            tmp2 = ft_strjoin_leak(tmp2, " ", 1444);
            t_free(tmp_char, 1445, "parsing.c");
        }
        
        i++;
        tmp = tmp->next;
    }
    
    return (tmp2);
}

// void join_redirections_with_commands(t_list **head) {
//     t_list *current;
//     t_list *next;
//     t_list *target;
//     char *combined;
//     char *temp;
    
//     if (!head || !(*head))
//         return;
    
//     current = *head;
    
//     while (current && current->next) {
//         next = current->next;
        
//         // Check for redirection pattern: [cmd/word] [>/<] [file]
//         if (next && ((ft_strcmp(next->token, "WORD") == 0 || ft_strcmp(next->token, "COMMAND") == 0 || ft_strcmp(next->token, "REDIRECTION") == 0) && (ft_strcmp(current->token, "WORD") == 0 || ft_strcmp(current->token, "COMMAND") == 0 || ft_strcmp(current->token, "REDIRECTION") == 0)) && next->next) {
//             target = next->next;
            
//             // Initialize combined with a copy of the current data
//             combined = ft_strdup(current->data);
//             if (!combined)
//                 return;
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, " ", __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, next->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, " ", __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, target->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
            
//             // Update the current token's data with the combined string
//             t_free(current->data, __LINE__, "join_redirections.c");
//             current->data = combined;
            
//             // Skip the tokens we've combined
//             current->next = target->next;
//             if (target->next)
//                 target->next->prev = current;
            
//             // Free the tokens we no longer need
//             t_free(next->data, __LINE__, "join_redirections.c");
//             t_free(next, __LINE__, "join_redirections.c");
//             t_free(target->data, __LINE__, "join_redirections.c");
//             t_free(target, __LINE__, "join_redirections.c");
            
//             // Continue from the current position to catch multiple redirections
//             continue;
//         }
        
//         current = current->next;
//     }
// }

// void join_redirections_with_commands(t_list **head)
// {
//     t_list *current;
//     t_list *next;
//     t_list *target;
//     char *combined;
//     char *temp;
    
//     if (!head || !(*head))
//         return;
    
//     // Handle parentheses case - when redirection follows a parenthesized expression
//     current = *head;
//     while (current) {
//         if (current->token && ft_strcmp(current->token, "PARENTHASIS") == 0) {
//             // Find if there's a redirection after this parenthesized command
//             t_list *redir_token = current->next;
//             if (redir_token && ft_strcmp(redir_token->token, "REDIRECTION") == 0 && redir_token->next) {
//                 t_list *file_token = redir_token->next;
                
//                 // Create combined string: (cmd) > file
//                 combined = ft_strjoin_leak(current->data, " ", __LINE__);
//                 if (!combined)
//                     break;
                
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, redir_token->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
                
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, " ", __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
                
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
                
//                 // Replace the parenthesis token with the combined string
//                 t_free(current->data, __LINE__, "join_redirections.c");
//                 current->data = combined;
                
//                 // Link the current node to the node after the file token
//                 current->next = file_token->next;
//                 if (file_token->next)
//                     file_token->next->prev = current;
                
//                 // Free the tokens that were combined
//                 t_free(redir_token->data, __LINE__, "join_redirections.c");
//                 t_free(redir_token, __LINE__, "join_redirections.c");
//                 t_free(file_token->data, __LINE__, "join_redirections.c");
//                 t_free(file_token, __LINE__, "join_redirections.c");
                
//                 // Stay at current position to check for more redirections
//                 continue;
//             }
//         }
        
//         current = current->next;
//     }
    
//     // Handle normal redirection case - when redirection is in the middle of a command
//     current = *head;
//     while (current && current->next) {
//         next = current->next;
        
//         if (next && ft_strcmp(next->token, "REDIRECTION") == 0 && next->next) {
//             target = next->next;
            
//             // Create combined string: cmd > file
//             combined = ft_strjoin_leak(current->data, " ", __LINE__);
//             if (!combined)
//                 break;
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, next->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, " ", __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, target->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
            
//             // Replace the current token with the combined string
//             t_free(current->data, __LINE__, "join_redirections.c");
//             current->data = combined;
            
//             // Link the current node to the node after the target
//             current->next = target->next;
//             if (target->next)
//                 target->next->prev = current;
            
//             // Free the tokens that were combined
//             t_free(next->data, __LINE__, "join_redirections.c");
//             t_free(next, __LINE__, "join_redirections.c");
//             t_free(target->data, __LINE__, "join_redirections.c");
//             t_free(target, __LINE__, "join_redirections.c");
            
//             // Stay at current position to check for more redirections
//             continue;
//         }
        
//         current = current->next;
//     }
    
//     // Handle operation case - when redirection follows an operation
//     current = *head;
//     while (current && current->next) {
//         if (current->token && ft_strcmp(current->token, "OPERATION") == 0 &&
//             current->prev && current->next && 
//             ft_strcmp(current->next->token, "REDIRECTION") == 0 && current->next->next) {
            
//             t_list *prev_cmd = current->prev;
//             t_list *redir_token = current->next;
//             t_list *file_token = current->next->next;
            
//             // Create combined string: cmd > file
//             combined = ft_strjoin_leak(prev_cmd->data, " ", __LINE__);
//             if (!combined)
//                 break;
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, redir_token->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, " ", __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
            
//             temp = combined;
//             combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
            
//             // Replace the previous token with the combined string
//             t_free(prev_cmd->data, __LINE__, "join_redirections.c");
//             prev_cmd->data = combined;
            
//             // Link past the redirection and file tokens
//             current->next = file_token->next;
//             if (file_token->next)
//                 file_token->next->prev = current;
            
//             // Free the tokens that were combined
//             t_free(redir_token->data, __LINE__, "join_redirections.c");
//             t_free(redir_token, __LINE__, "join_redirections.c");
//             t_free(file_token->data, __LINE__, "join_redirections.c");
//             t_free(file_token, __LINE__, "join_redirections.c");
            
//             // Stay at current position to check for more redirections
//             continue;
//         }
        
//         current = current->next;
//     }
    
//     // Special case: handle redirections at the end of the list
//     // This helps with handling commands like: cmd && cmd2 > file
//     current = *head;
//     while (current) {
//         if (!current->next) {
//             t_list *last = current;
            
//             // Walk backward to find a REDIRECTION token
//             while (last && last->prev && ft_strcmp(last->token, "REDIRECTION") != 0)
//                 last = last->prev;
            
//             // If we found a REDIRECTION and it's not at the beginning
//             if (last && ft_strcmp(last->token, "REDIRECTION") == 0 && last->prev && last->next) {
//                 t_list *cmd_token = last->prev;
//                 t_list *file_token = last->next;
                
//                 // Create combined string: cmd > file
//                 combined = ft_strjoin_leak(cmd_token->data, " ", __LINE__);
//                 if (!combined)
//                     break;
                
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, last->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
                
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, " ", __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
                
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
                
//                 // Replace the command token with the combined string
//                 t_free(cmd_token->data, __LINE__, "join_redirections.c");
//                 cmd_token->data = combined;
                
//                 // Link the command node to the node after the file token or NULL
//                 cmd_token->next = file_token->next;
//                 if (file_token->next)
//                     file_token->next->prev = cmd_token;
                
//                 // Free the tokens that were combined
//                 t_free(last->data, __LINE__, "join_redirections.c");
//                 t_free(last, __LINE__, "join_redirections.c");
//                 t_free(file_token->data, __LINE__, "join_redirections.c");
//                 t_free(file_token, __LINE__, "join_redirections.c");
                
//                 // We processed the end of the list, so break
//                 break;
//             }
            
//             // If no more to process at the end, break
//             break;
//         }
        
//         current = current->next;
//     }
// }
// void join_redirections_with_commands(t_list **head)
// {
//     t_list *current;
//     t_list *next;
//     t_list *target;
//     char *combined;
//     char *temp;
    
//     if (!head || !(*head))
//         return;
    
//     // First pass: handle parentheses and redirections outside them
//     current = *head;
//     while (current) {
//         if (current->token && ft_strcmp(current->token, "PARENTHASIS") == 0) {
//             t_list *redir_token = current->next;
//             if (redir_token && ft_strcmp(redir_token->token, "REDIRECTION") == 0 && redir_token->next) {
//                 t_list *file_token = redir_token->next;
//                 combined = ft_strjoin_leak(current->data, " ", __LINE__);
//                 if (!combined)
//                     break;
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, redir_token->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, " ", __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
//                 t_free(current->data, __LINE__, "join_redirections.c");
//                 current->data = combined;
//                 current->next = file_token->next;
//                 if (file_token->next)
//                     file_token->next->prev = current;
//                 t_free(redir_token->data, __LINE__, "join_redirections.c");
//                 t_free(redir_token, __LINE__, "join_redirections.c");
//                 t_free(file_token->data, __LINE__, "join_redirections.c");
//                 t_free(file_token, __LINE__, "join_redirections.c");
//                 continue;
//             }
//         }
//         current = current->next;
//     }
    
//     // Second pass: handle redirections within regular commands
//     current = *head;
//     while (current && current->next) {
//         next = current->next;
        
//         // Special handling for heredoc (<<) and other redirections
//         if (next && ft_strcmp(next->token, "REDIRECTION") == 0 && next->next) {
//             target = next->next;
            
//             // Check if this is a heredoc redirection
//             // int is_heredoc = (ft_strstr(next->data, "<<") != NULL);
            
//             combined = ft_strjoin_leak(current->data, " ", __LINE__);
//             if (!combined)
//                 break;
                
//             temp = combined;
//             combined = ft_strjoin_leak(combined, next->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
                
//             temp = combined;
//             combined = ft_strjoin_leak(combined, " ", __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
                
//             temp = combined;
//             combined = ft_strjoin_leak(combined, target->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
                
//             t_free(current->data, __LINE__, "join_redirections.c");
//             current->data = combined;
            
//             // Update list links
//             current->next = target->next;
//             if (target->next)
//                 target->next->prev = current;
                
//             t_free(next->data, __LINE__, "join_redirections.c");
//             t_free(next, __LINE__, "join_redirections.c");
//             t_free(target->data, __LINE__, "join_redirections.c");
//             t_free(target, __LINE__, "join_redirections.c");
//             continue;
//         }
        
//         current = current->next;
//     }
    
//     // Third pass: handle operations with redirections
//     current = *head;
//     while (current && current->next) {
//         if (current->token && ft_strcmp(current->token, "OPERATION") == 0 && 
//             current->prev && current->next && 
//             ft_strcmp(current->next->token, "REDIRECTION") == 0 && current->next->next) {
            
//             t_list *prev_cmd = current->prev;
//             t_list *redir_token = current->next;
//             t_list *file_token = current->next->next;
            
//             combined = ft_strjoin_leak(prev_cmd->data, " ", __LINE__);
//             if (!combined)
//                 break;
//             temp = combined;
//             combined = ft_strjoin_leak(combined, redir_token->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
//             temp = combined;
//             combined = ft_strjoin_leak(combined, " ", __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
//             temp = combined;
//             combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
//             t_free(temp, __LINE__, "join_redirections.c");
//             if (!combined)
//                 break;
            
//             t_free(prev_cmd->data, __LINE__, "join_redirections.c");
//             prev_cmd->data = combined;
//             current->next = file_token->next;
//             if (file_token->next)
//                 file_token->next->prev = current;
            
//             t_free(redir_token->data, __LINE__, "join_redirections.c");
//             t_free(redir_token, __LINE__, "join_redirections.c");
//             t_free(file_token->data, __LINE__, "join_redirections.c");
//             t_free(file_token, __LINE__, "join_redirections.c");
//             continue;
//         }
        
//         current = current->next;
//     }
    
//     // Fourth pass: handle trailing redirections
//     current = *head;
//     while (current) {
//         if (!current->next) {
//             t_list *last = current;
//             while (last && last->prev && ft_strcmp(last->token, "REDIRECTION") != 0)
//                 last = last->prev;
                
//             if (last && ft_strcmp(last->token, "REDIRECTION") == 0 && last->prev && last->next) {
//                 t_list *cmd_token = last->prev;
//                 t_list *file_token = last->next;
                
//                 combined = ft_strjoin_leak(cmd_token->data, " ", __LINE__);
//                 if (!combined)
//                     break;
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, last->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, " ", __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
//                 temp = combined;
//                 combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
//                 t_free(temp, __LINE__, "join_redirections.c");
//                 if (!combined)
//                     break;
                
//                 t_free(cmd_token->data, __LINE__, "join_redirections.c");
//                 cmd_token->data = combined;
//                 cmd_token->next = file_token->next;
//                 if (file_token->next)
//                     file_token->next->prev = cmd_token;
                
//                 t_free(last->data, __LINE__, "join_redirections.c");
//                 t_free(last, __LINE__, "join_redirections.c");
//                 t_free(file_token->data, __LINE__, "join_redirections.c");
//                 t_free(file_token, __LINE__, "join_redirections.c");
//                 break;
//             }
//             break;
//         }
        
//         current = current->next;
//     }
// }
void join_redirections_with_commands(t_list **head)
{
    t_list *current;
    t_list *next;
    t_list *target;
    char *combined;
    char *temp;
    
    if (!head || !(*head))
        return;
    
    // First pass: handle parentheses with redirections
    current = *head;
    while (current) {
        if (current->token && ft_strcmp(current->token, "PARENTHASIS") == 0) {
            t_list *redir_token = current->next;
            if (redir_token && ft_strcmp(redir_token->token, "REDIRECTION") == 0 && redir_token->next) {
                t_list *file_token = redir_token->next;
                combined = ft_strjoin_leak(current->data, " ", __LINE__);
                if (!combined)
                    break;
                temp = combined;
                combined = ft_strjoin_leak(combined, redir_token->data, __LINE__);
                t_free(temp, __LINE__, "join_redirections.c");
                if (!combined)
                    break;
                temp = combined;
                combined = ft_strjoin_leak(combined, " ", __LINE__);
                t_free(temp, __LINE__, "join_redirections.c");
                if (!combined)
                    break;
                temp = combined;
                combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
                t_free(temp, __LINE__, "join_redirections.c");
                if (!combined)
                    break;
                t_free(current->data, __LINE__, "join_redirections.c");
                current->data = combined;
                current->next = file_token->next;
                if (file_token->next)
                    file_token->next->prev = current;
                t_free(redir_token->data, __LINE__, "join_redirections.c");
                t_free(redir_token, __LINE__, "join_redirections.c");
                t_free(file_token->data, __LINE__, "join_redirections.c");
                t_free(file_token, __LINE__, "join_redirections.c");
                continue;
            }
        }
        current = current->next;
    }
    
    // Second pass: Handle regular commands with redirections
    current = *head;
    while (current && current->next) {
        next = current->next;
        if (next && ft_strcmp(next->token, "REDIRECTION") == 0 && next->next) {
            target = next->next;
            combined = ft_strjoin_leak(current->data, " ", __LINE__);
            if (!combined)
                break;
            temp = combined;
            combined = ft_strjoin_leak(combined, next->data, __LINE__);
            t_free(temp, __LINE__, "join_redirections.c");
            if (!combined)
                break;
            temp = combined;
            combined = ft_strjoin_leak(combined, " ", __LINE__);
            t_free(temp, __LINE__, "join_redirections.c");
            if (!combined)
                break;
            temp = combined;
            combined = ft_strjoin_leak(combined, target->data, __LINE__);
            t_free(temp, __LINE__, "join_redirections.c");
            if (!combined)
                break;
            t_free(current->data, __LINE__, "join_redirections.c");
            current->data = combined;
            current->next = target->next;
            if (target->next)
                target->next->prev = current;
            t_free(next->data, __LINE__, "join_redirections.c");
            t_free(next, __LINE__, "join_redirections.c");
            t_free(target->data, __LINE__, "join_redirections.c");
            t_free(target, __LINE__, "join_redirections.c");
            continue;
        }
        current = current->next;
    }
    
    // Third pass: Handle operations with redirections
    current = *head;
    while (current && current->next) {
        if (current->token && ft_strcmp(current->token, "OPERATION") == 0 && 
            current->prev && current->next && 
            ft_strcmp(current->next->token, "REDIRECTION") == 0 && 
            current->next->next) {
            
            t_list *prev_cmd = current->prev;
            t_list *redir_token = current->next;
            t_list *file_token = current->next->next;
            
            combined = ft_strjoin_leak(prev_cmd->data, " ", __LINE__);
            if (!combined)
                break;
            temp = combined;
            combined = ft_strjoin_leak(combined, redir_token->data, __LINE__);
            t_free(temp, __LINE__, "join_redirections.c");
            if (!combined)
                break;
            temp = combined;
            combined = ft_strjoin_leak(combined, " ", __LINE__);
            t_free(temp, __LINE__, "join_redirections.c");
            if (!combined)
                break;
            temp = combined;
            combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
            t_free(temp, __LINE__, "join_redirections.c");
            if (!combined)
                break;
            
            t_free(prev_cmd->data, __LINE__, "join_redirections.c");
            prev_cmd->data = combined;
            current->next = file_token->next;
            if (file_token->next)
                file_token->next->prev = current;
            
            t_free(redir_token->data, __LINE__, "join_redirections.c");
            t_free(redir_token, __LINE__, "join_redirections.c");
            t_free(file_token->data, __LINE__, "join_redirections.c");
            t_free(file_token, __LINE__, "join_redirections.c");
            continue;
        }
        
        current = current->next;
    }
    
    // Fourth pass: Handle trailing redirections
    current = *head;
    while (current) {
        if (!current->next) {
            t_list *last = current;
            while (last && last->prev && ft_strcmp(last->token, "REDIRECTION") != 0)
                last = last->prev;
                
            if (last && ft_strcmp(last->token, "REDIRECTION") == 0 && last->prev && last->next) {
                t_list *cmd_token = last->prev;
                t_list *file_token = last->next;
                
                combined = ft_strjoin_leak(cmd_token->data, " ", __LINE__);
                if (!combined)
                    break;
                temp = combined;
                combined = ft_strjoin_leak(combined, last->data, __LINE__);
                t_free(temp, __LINE__, "join_redirections.c");
                if (!combined)
                    break;
                temp = combined;
                combined = ft_strjoin_leak(combined, " ", __LINE__);
                t_free(temp, __LINE__, "join_redirections.c");
                if (!combined)
                    break;
                temp = combined;
                combined = ft_strjoin_leak(combined, file_token->data, __LINE__);
                t_free(temp, __LINE__, "join_redirections.c");
                if (!combined)
                    break;
                
                t_free(cmd_token->data, __LINE__, "join_redirections.c");
                cmd_token->data = combined;
                cmd_token->next = file_token->next;
                if (file_token->next)
                    file_token->next->prev = cmd_token;
                
                t_free(last->data, __LINE__, "join_redirections.c");
                t_free(last, __LINE__, "join_redirections.c");
                t_free(file_token->data, __LINE__, "join_redirections.c");
                t_free(file_token, __LINE__, "join_redirections.c");
                break;
            }
            break;
        }
        current = current->next;
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
// void tree_maker(t_list **head, t_tree **tree)
// {
//     t_list *tmp;
//     t_list *last;
//     t_list *current;
//     t_list *prev_part;
//     t_list *to_free;
//     t_tree *command;
//     int i;
//     int total_nodes;
//     *tree = t_malloc(sizeof(t_tree), 1462, "parsing.c");
//     if (!(*tree))
//         return;
// 	last = NULL;
//     (*tree)->left = NULL;
//     (*tree)->right = NULL;
//     (*tree)->command = NULL;
//     (*tree)->command_arr = NULL;
//     (*tree)->redirections = NULL;
//     (*tree)->type = "COMMAND";
//     if (!(*head))
//         return;
//     tmp = *head;
//     total_nodes = 0;
//     while (tmp)
//     {
//         last = tmp;
//         tmp = tmp->next;
//         total_nodes++;
//     }
//     tmp = last;
//     i = total_nodes;
//     while (tmp)
//     {
//         if (ft_strcmp(tmp->token, "OPERATION") == 0)
//         {
//             command = t_malloc(sizeof(t_tree), 908, "parsing.c");
//             if (!command)
//                 return;
//             if (tmp->next)
//                 command->command = side_maker(&(tmp->next), total_nodes - i, 0);
//             else
//                 command->command = NULL;
//             command->left = NULL;
//             command->command_arr = NULL;
//             command->right = NULL;
//             command->redirections = NULL;
//             command->type = "COMMAND";
//             (*tree)->right = command;
//             (*tree)->left = NULL;
//             (*tree)->command = NULL;
//             (*tree)->command_arr = NULL;
//             (*tree)->redirections = NULL;
//             (*tree)->type = "OPERATION";
//             if (tmp->prev)
//             {
//                 prev_part = *head;
//                 tmp->prev->next = NULL;
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
//                 if (prev_part)
//                     tree_maker(&prev_part, &(*tree)->left);
//             }
//             else
//             {
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
//                 *head = NULL;
//             }
//             return;
//         }
//         tmp = tmp->prev;
//         i--;
//     }
//     if (*head)
//     {
//         command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//         if (!command)
//             return;
//         command->command = side_maker(head, total_nodes, 0);
//         command->left = NULL;
//         command->command_arr = NULL;
//         command->redirections = NULL;
//         command->right = NULL;
//         command->type = "COMMAND";
//         (*tree)->left = command;
//         (*tree)->right = NULL;
//         (*tree)->command_arr = NULL;
//         (*tree)->command = NULL;
//         (*tree)->redirections= NULL;
//         (*tree)->type = "COMMAND";
//         current = *head;
//         while (current)
//         {
//             to_free = current;
//             current = current->next;
//             if (to_free->data)
//                 t_free(to_free->data, __LINE__, "parsing.c");
//             t_free(to_free, __LINE__, "parsing.c");
//         }
//         *head = NULL;
//     }
// }
// void tree_maker(t_list **head, t_tree **tree) {
//     t_list *tmp;
//     t_list *last;
//     t_list *current;
//     t_list *prev_part;
//     t_tree *command;
//     int i;
//     int total_nodes;
    
//     *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//     if (!(*tree))
//         return;
        
//     last = NULL;
//     (*tree)->left = NULL;
//     (*tree)->right = NULL;
//     (*tree)->command = NULL;
//     (*tree)->command_arr = NULL;
//     (*tree)->redirections = NULL;
//     (*tree)->type = "COMMAND";
    
//     if (!(*head))
//         return;
        
//     tmp = *head;
//     total_nodes = 0;
    
//     while (tmp) {
//         last = tmp;
//         tmp = tmp->next;
//         total_nodes++;
//     }
    
//     tmp = last;
//     i = total_nodes;
    
//     while (tmp) {
//         if (ft_strcmp(tmp->token, "OPERATION") == 0) {
//             command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//             if (!command)
//                 return;
                
//             if (tmp->next) {
//                 command->command = side_maker(&(tmp->next), total_nodes - i, 0);
                
//                 // Create a temporary list for cleanup if needed
//                 // t_list *right_part = tmp->next;
//                 // t_list *right_current = right_part;
                
//                 // Skip actual freeing here as side_maker handles it
//             } else {
//                 command->command = NULL;
//             }
            
//             command->left = NULL;
//             command->command_arr = NULL;
//             command->right = NULL;
//             command->redirections = NULL;
//             command->type = "COMMAND";
            
//             (*tree)->right = command;
//             (*tree)->left = NULL;
//             (*tree)->command = NULL;
//             (*tree)->command_arr = NULL;
//             (*tree)->redirections = NULL;
//             (*tree)->type = "OPERATION";
            
//             if (tmp->prev) {
//                 prev_part = *head;
//                 tmp->prev->next = NULL; // Cut the list at operation
                
//                 // Free the operation token node itself
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
                
//                 // Process the left part recursively
//                 if (prev_part) {
//                     tree_maker(&prev_part, &(*tree)->left);
//                     // Ensure prev_part is NULL after tree_maker (it should handle this)
//                     if (prev_part != NULL) {
//                         free_list(&prev_part); // Make sure it's fully freed
//                     }
//                 }
//             } else {
//                 // Free the operation token if it's at the beginning
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
//                 *head = NULL;
//             }
//             return;
//         }
//         tmp = tmp->prev;
//         i--;
//     }
    
//     // No operation found, create a simple command node
//     if (*head) {
//         command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//         if (!command)
//             return;
            
//         command->command = side_maker(head, total_nodes, 0);
//         command->left = NULL;
//         command->command_arr = NULL;
//         command->redirections = NULL;
//         command->right = NULL;
//         command->type = "COMMAND";
        
//         (*tree)->left = command;
//         (*tree)->right = NULL;
//         (*tree)->command_arr = NULL;
//         (*tree)->command = NULL;
//         (*tree)->redirections = NULL;
//         (*tree)->type = "COMMAND";
        
//         // Free the list nodes but not their data (as it's been moved to the tree)
//         current = *head;
//         while (current) {
//             t_list *to_free = current;
//             current = current->next;
//             if (to_free->data)
//                 t_free(to_free->data, __LINE__, "parsing.c");
//             t_free(to_free, __LINE__, "parsing.c");
//         }
//         *head = NULL;
//     }
// }

// void tree_maker(t_list **head, t_tree **tree) {
//     t_list *tmp;
//     t_list *last;
//     t_list *current;
//     t_list *prev_part;
//     t_tree *command;
//     int i;
//     int total_nodes;
    
//     *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//     if (!(*tree))
//         return;
    
//     last = NULL;
//     (*tree)->left = NULL;
//     (*tree)->right = NULL;
//     (*tree)->command = NULL;
//     (*tree)->command_arr = NULL;
//     (*tree)->redirections = NULL;
//     (*tree)->type = "COMMAND";
    
//     if (!(*head))
//         return;
//     tmp = *head;
//     total_nodes = 0;
//     while (tmp) {
//         last = tmp;
//         tmp = tmp->next;
//         total_nodes++;
//     }
    
//     tmp = last;
//     i = total_nodes;
    
//     while (tmp) {
//         if (ft_strcmp(tmp->token, "OPERATION") == 0) {
//             command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//             if (!command)
//                 return;
                
//             if (tmp->next) {
//                 command->command = side_maker(&(tmp->next), total_nodes - i, 0);
                
//                 // Free any remaining nodes in the right side that weren't included
//                 t_list *right_nodes = tmp->next;
//                 t_list *next_right;
//                 while (right_nodes) {
//                     next_right = right_nodes->next;
//                     if (right_nodes->data)
//                         t_free(right_nodes->data, __LINE__, "parsing.c");
//                     t_free(right_nodes, __LINE__, "parsing.c");
//                     right_nodes = next_right;
//                 }
//             } else {
//                 command->command = NULL;
//             }
            
//             command->left = NULL;
//             command->command_arr = NULL;
//             command->right = NULL;
//             command->redirections = NULL;
//             command->type = "COMMAND";
            
//             (*tree)->right = command;
//             (*tree)->left = NULL;
//             (*tree)->command = NULL;
//             (*tree)->command_arr = NULL;
//             (*tree)->redirections = NULL;
//             (*tree)->type = "OPERATION";
            
//             if (tmp->prev) {
//                 // Store the head of the list for processing the left part
//                 prev_part = *head;
                
//                 // Cut the list at the operation token
//                 tmp->prev->next = NULL;
                
//                 // Free the operation token itself
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
                
//                 if (prev_part) {
//                     // Process the left part recursively
//                     tree_maker(&prev_part, &(*tree)->left);
                    
//                     // Check if there are still nodes to free (should not happen, but for safety)
//                     if (prev_part) {
//                         t_list *left_cleanup = prev_part;
//                         t_list *next_left;
//                         while (left_cleanup) {
//                             next_left = left_cleanup->next;
//                             if (left_cleanup->data)
//                                 t_free(left_cleanup->data, __LINE__, "parsing.c");
//                             t_free(left_cleanup, __LINE__, "parsing.c");
//                             left_cleanup = next_left;
//                         }
//                     }
//                 }
//             } else {
//                 // The operation token is at the beginning of the list
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
//             }
            
//             // Make sure we mark the head as NULL, even if we freed everything
//             *head = NULL;
//             return;
//         }
//         tmp = tmp->prev;
//         i--;
//     }
    
//     // No operation found, so create a simple command node
//     if (*head) {
//         command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//         if (!command)
//             return;
            
//         command->command = side_maker(head, total_nodes, 0);
//         command->left = NULL;
//         command->command_arr = NULL;
//         command->redirections = NULL;
//         command->right = NULL;
//         command->type = "COMMAND";
        
//         (*tree)->left = command;
//         (*tree)->right = NULL;
//         (*tree)->command_arr = NULL;
//         (*tree)->command = NULL;
//         (*tree)->redirections = NULL;
//         (*tree)->type = "COMMAND";
        
//         // Make sure we free all nodes in the list after processing
//         current = *head;
//         while (current) {
//             t_list *to_free = current;
//             current = current->next;
//             if (to_free->data)
//                 t_free(to_free->data, __LINE__, "parsing.c");
//             t_free(to_free, __LINE__, "parsing.c");
//         }
//         *head = NULL;
//     }
// }
// void tree_maker(t_list **head, t_tree **tree) {
//     t_list *tmp;
//     t_list *last;
//     t_list *current;
//     t_list *prev_part;
//     t_tree *command;
//     int i;
//     int total_nodes;
    
//     if (!(*head)) {
//         *tree = NULL;
//         return;
//     }
    
//     tmp = *head;
//     total_nodes = 0;
//     while (tmp) {
//         last = tmp;
//         tmp = tmp->next;
//         total_nodes++;
//     }
    
//     tmp = last;
//     i = total_nodes;
    
//     // Only check for OPERATION tokens (not PIPE)
//     while (tmp) {
//         if (ft_strcmp(tmp->token, "OPERATION") == 0) {
//             // Create an OPERATION node directly as the root
//             *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//             if (!(*tree))
//                 return;
            
//             // Set type to OPERATION
//             (*tree)->type = "OPERATION";
            
//             (*tree)->command = NULL;
//             (*tree)->command_arr = NULL;
//             (*tree)->redirections = NULL;
            
//             // Process right side
//             command = NULL;
//             if (tmp->next) {
//                 command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//                 if (!command) {
//                     t_free(*tree, __LINE__, "parsing.c");
//                     *tree = NULL;
//                     return;
//                 }
                
//                 command->command = side_maker(&(tmp->next), total_nodes - i, 0);
//                 command->left = NULL;
//                 command->right = NULL;
//                 command->command_arr = NULL;
//                 command->redirections = NULL;
//                 command->type = "WORD";
                
//                 // Free right side list nodes
//                 t_list *right_nodes = tmp->next;
//                 t_list *next_right;
//                 while (right_nodes) {
//                     next_right = right_nodes->next;
//                     if (right_nodes->data)
//                         t_free(right_nodes->data, __LINE__, "parsing.c");
//                     t_free(right_nodes, __LINE__, "parsing.c");
//                     right_nodes = next_right;
//                 }
//             }
            
//             (*tree)->right = command;
            
//             // Process left side
//             if (tmp->prev) {
//                 prev_part = *head;
//                 tmp->prev->next = NULL;
                
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
                
//                 if (prev_part) {
//                     tree_maker(&prev_part, &(*tree)->left);
                    
//                     // Make sure to free any remaining left side nodes
//                     if (prev_part) {
//                         t_list *left_cleanup = prev_part;
//                         t_list *next_left;
//                         while (left_cleanup) {
//                             next_left = left_cleanup->next;
//                             if (left_cleanup->data)
//                                 t_free(left_cleanup->data, __LINE__, "parsing.c");
//                             t_free(left_cleanup, __LINE__, "parsing.c");
//                             left_cleanup = next_left;
//                         }
//                     }
//                 }
//             } else {
//                 // No left side, free the operation token
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
//                 (*tree)->left = NULL;
//             }
            
//             *head = NULL;
//             return;
//         }
//         tmp = tmp->prev;
//         i--;
//     }
    
//     // If no operations found, create a simple node based on the token type
//     if (*head) {
//         *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//         if (!(*tree))
//             return;
        
//         // Set the type based on the first token's type
//         // if ((*head)->token != NULL) {
//         //     (*tree)->type = (*head)->token;  // This will preserve token types like "PARENTHASIS", "PIPE", etc.
//         // } else {
//             (*tree)->type = "WORD";  // Default type if no token
//         // }
        
//         (*tree)->command = side_maker(head, total_nodes, 0);
//         (*tree)->left = NULL;
//         (*tree)->right = NULL;
//         (*tree)->command_arr = NULL;
//         (*tree)->redirections = NULL;
        
//         // Free all the original list nodes
//         current = *head;
//         while (current) {
//             t_list *to_free = current;
//             current = current->next;
//             if (to_free->data)
//                 t_free(to_free->data, __LINE__, "parsing.c");
//             t_free(to_free, __LINE__, "parsing.c");
//         }
//         *head = NULL;
//     }
// }

// void tree_maker(t_list **head, t_tree **tree) {
//     t_list *tmp;
//     t_list *last;
//     t_list *current;
//     t_list *prev_part;
//     t_tree *command;
//     int i;
//     int total_nodes;
    
//     if (!(*head)) {
//         *tree = NULL;
//         return;
//     }
    
//     tmp = *head;
//     total_nodes = 0;
//     while (tmp) {
//         last = tmp;
//         tmp = tmp->next;
//         total_nodes++;
//     }
    
//     tmp = last;
//     i = total_nodes;
    
//     // First pass: find operation tokens outside of parentheses
//     while (tmp) {
//         if (ft_strcmp(tmp->token, "OPERATION") == 0) {
//             // Create operation node
//             *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//             if (!(*tree))
//                 return;
                
//             (*tree)->type = "OPERATION";
//             (*tree)->command = NULL;
//             (*tree)->command_arr = NULL;
//             (*tree)->redirections = NULL;
            
//             // Handle right part
//             command = NULL;
//             if (tmp->next) {
//                 command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//                 if (!command) {
//                     t_free(*tree, __LINE__, "parsing.c");
//                     *tree = NULL;
//                     return;
//                 }
                
//                 // Preserve type for the right side
//                 if (tmp->next->token && ft_strcmp(tmp->next->token, "PARENTHASIS") == 0) {
//                     command->type = "PARENTHASIS";
//                 } else {
//                     command->type = "WORD";
//                 }
                
//                 command->command = side_maker(&(tmp->next), total_nodes - i, 0);
//                 command->left = NULL;
//                 command->right = NULL;
//                 command->command_arr = NULL;
//                 command->redirections = NULL;
                
//                 // Free tokens
//                 t_list *right_nodes = tmp->next;
//                 t_list *next_right;
//                 while (right_nodes) {
//                     next_right = right_nodes->next;
//                     if (right_nodes->data)
//                         t_free(right_nodes->data, __LINE__, "parsing.c");
//                     t_free(right_nodes, __LINE__, "parsing.c");
//                     right_nodes = next_right;
//                 }
//             }
            
//             (*tree)->right = command;
            
//             // Handle left part
//             if (tmp->prev) {
//                 prev_part = *head;
//                 tmp->prev->next = NULL;
                
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
                
//                 if (prev_part) {
//                     tree_maker(&prev_part, &(*tree)->left);
                    
//                     // Clean up left nodes
//                     if (prev_part) {
//                         t_list *left_cleanup = prev_part;
//                         t_list *next_left;
//                         while (left_cleanup) {
//                             next_left = left_cleanup->next;
//                             if (left_cleanup->data)
//                                 t_free(left_cleanup->data, __LINE__, "parsing.c");
//                             t_free(left_cleanup, __LINE__, "parsing.c");
//                             left_cleanup = next_left;
//                         }
//                     }
//                 }
//             } else {
//                 if (tmp->data)
//                     t_free(tmp->data, __LINE__, "parsing.c");
//                 t_free(tmp, __LINE__, "parsing.c");
//                 (*tree)->left = NULL;
//             }
            
//             *head = NULL;
//             return;
//         }
        
//         tmp = tmp->prev;
//         i--;
//     }
    
//     // If no operation found, create a node for the entire list
//     if (*head) {
//         *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
//         if (!(*tree))
//             return;
        
//         // Preserve PARENTHASIS type if needed
//         if ((*head)->token && ft_strcmp((*head)->token, "PARENTHASIS") == 0) {
//             (*tree)->type = "PARENTHASIS";
//         } else if ((*head)->token) {
//             (*tree)->type = (*head)->token;
//         } else {
//             (*tree)->type = "WORD";
//         }
        
//         (*tree)->command = side_maker(head, total_nodes, 0);
//         (*tree)->left = NULL;
//         (*tree)->right = NULL;
//         (*tree)->command_arr = NULL;
//         (*tree)->redirections = NULL;
        
//         // Free tokens
//         current = *head;
//         while (current) {
//             t_list *to_free = current;
//             current = current->next;
//             if (to_free->data)
//                 t_free(to_free->data, __LINE__, "parsing.c");
//             t_free(to_free, __LINE__, "parsing.c");
//         }
        
//         *head = NULL;
//     }
// }

void tree_maker(t_list **head, t_tree **tree)
{
    t_list *tmp;
    t_list *last;
    t_list *current;
    t_list *prev_part;
    t_tree *command;
    int i;
    int total_nodes;
    
    if (!(*head)) {
        *tree = NULL;
        return;
    }
    
    // Count total nodes and get the last node
    tmp = *head;
    total_nodes = 0;
    while (tmp) {
        last = tmp;
        tmp = tmp->next;
        total_nodes++;
    }
    
    tmp = last;
    i = total_nodes;
    
    // Process from right to left, looking for operations first
    while (tmp) {
        // Handle operations (&&, ||) with highest priority
        if (ft_strcmp(tmp->token, "OPERATION") == 0) {
            *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
            if (!(*tree))
                return;
                
            (*tree)->type = "OPERATION";
            (*tree)->command = NULL;
            (*tree)->command_arr = NULL;
            (*tree)->redirections = NULL;
            
            // Process the right side
            command = NULL;
            if (tmp->next) {
                command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
                if (!command) {
                    t_free(*tree, __LINE__, "parsing.c");
                    *tree = NULL;
                    return;
                }
                
                // Preserve the token type for the right side
                if (tmp->next->token && tmp->next->token[0] != '\0') {
                    command->type = tmp->next->token;
                } else {
                    command->type = "WORD";
                }
                
                command->command = side_maker(&(tmp->next), total_nodes - i, 0);
                command->left = NULL;
                command->right = NULL;
                command->command_arr = NULL;
                command->redirections = NULL;
                
                // Clean up right tokens
                t_list *right_nodes = tmp->next;
                t_list *next_right;
                while (right_nodes) {
                    next_right = right_nodes->next;
                    if (right_nodes->data)
                        t_free(right_nodes->data, __LINE__, "parsing.c");
                    t_free(right_nodes, __LINE__, "parsing.c");
                    right_nodes = next_right;
                }
            }
            
            (*tree)->right = command;
            
            // Process the left side
            if (tmp->prev) {
                prev_part = *head;
                tmp->prev->next = NULL;
                
                if (tmp->data)
                    t_free(tmp->data, __LINE__, "parsing.c");
                t_free(tmp, __LINE__, "parsing.c");
                
                if (prev_part) {
                    tree_maker(&prev_part, &(*tree)->left);
                    
                    // Clean up remaining left nodes if any
                    if (prev_part) {
                        t_list *left_cleanup = prev_part;
                        t_list *next_left;
                        while (left_cleanup) {
                            next_left = left_cleanup->next;
                            if (left_cleanup->data)
                                t_free(left_cleanup->data, __LINE__, "parsing.c");
                            t_free(left_cleanup, __LINE__, "parsing.c");
                            left_cleanup = next_left;
                        }
                    }
                }
            } else {
                if (tmp->data)
                    t_free(tmp->data, __LINE__, "parsing.c");
                t_free(tmp, __LINE__, "parsing.c");
                (*tree)->left = NULL;
            }
            
            *head = NULL;
            return;
        }
        
        tmp = tmp->prev;
        i--;
    }
    
    // Reset and look for pipes if no operations found
    tmp = last;
    i = total_nodes;
    
    while (tmp) {
        // Handle pipes
        if (ft_strcmp(tmp->token, "PIPE") == 0) {
            *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
            if (!(*tree))
                return;
                
            (*tree)->type = "PIPE";
            (*tree)->command = NULL;
            (*tree)->command_arr = NULL;
            (*tree)->redirections = NULL;
            
            // Process the right side
            command = NULL;
            if (tmp->next) {
                command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
                if (!command) {
                    t_free(*tree, __LINE__, "parsing.c");
                    *tree = NULL;
                    return;
                }
                
                // Preserve the token type for the right side
                if (tmp->next->token && tmp->next->token[0] != '\0') {
                    command->type = tmp->next->token;
                } else {
                    command->type = "WORD";
                }
                
                command->command = side_maker(&(tmp->next), total_nodes - i, 0);
                command->left = NULL;
                command->right = NULL;
                command->command_arr = NULL;
                command->redirections = NULL;
                
                // Clean up right tokens
                t_list *right_nodes = tmp->next;
                t_list *next_right;
                while (right_nodes) {
                    next_right = right_nodes->next;
                    if (right_nodes->data)
                        t_free(right_nodes->data, __LINE__, "parsing.c");
                    t_free(right_nodes, __LINE__, "parsing.c");
                    right_nodes = next_right;
                }
            }
            
            (*tree)->right = command;
            
            // Process the left side
            if (tmp->prev) {
                prev_part = *head;
                tmp->prev->next = NULL;
                
                if (tmp->data)
                    t_free(tmp->data, __LINE__, "parsing.c");
                t_free(tmp, __LINE__, "parsing.c");
                
                if (prev_part) {
                    tree_maker(&prev_part, &(*tree)->left);
                    
                    // Clean up remaining left nodes if any
                    if (prev_part) {
                        t_list *left_cleanup = prev_part;
                        t_list *next_left;
                        while (left_cleanup) {
                            next_left = left_cleanup->next;
                            if (left_cleanup->data)
                                t_free(left_cleanup->data, __LINE__, "parsing.c");
                            t_free(left_cleanup, __LINE__, "parsing.c");
                            left_cleanup = next_left;
                        }
                    }
                }
            } else {
                if (tmp->data)
                    t_free(tmp->data, __LINE__, "parsing.c");
                t_free(tmp, __LINE__, "parsing.c");
                (*tree)->left = NULL;
            }
            
            *head = NULL;
            return;
        }
        
        tmp = tmp->prev;
        i--;
    }
    
    // If no operations or pipes, create a leaf node
    if (*head) {
        *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
        if (!(*tree))
            return;
        
        // Preserve the original token type
        if ((*head)->token && (*head)->token[0] != '\0') {
            (*tree)->type = (*head)->token;
        } else {
            (*tree)->type = "WORD";
        }
        
        // For the command, use the original data
        // (*tree)->command = ft_strdup((*head)->data);
		(*tree)->command = side_maker(head, total_nodes - i, 0);
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->command_arr = NULL;
        (*tree)->redirections = NULL;
        
        // Clean up tokens
        current = *head;
        while (current) {
            t_list *to_free = current;
            current = current->next;
            if (to_free->data)
                t_free(to_free->data, __LINE__, "parsing.c");
            t_free(to_free, __LINE__, "parsing.c");
        }
        
        *head = NULL;
    }
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

// char *extract_command_with_redirects(t_list **head, t_list **pipe_pos) {
//     t_list *current = *head;
//     char *command_str = NULL;
//     char *temp_str = NULL;
    
//     *pipe_pos = NULL;
    
//     // Find the pipe position while building the complete command string
//     while (current) {
//         if (current->token && ft_strcmp(current->token, "PIPE") == 0) {
//             *pipe_pos = current;
//             break;
//         }
        
//         // Build the full command string with redirections
//         if (command_str) {
//             temp_str = command_str;
//             command_str = ft_strjoin_leak(command_str, " ", __LINE__);
//             t_free(temp_str, __LINE__, "parsing.c");
            
//             temp_str = command_str;
//             command_str = ft_strjoin_leak(command_str, current->data, __LINE__);
//             t_free(temp_str, __LINE__, "parsing.c");
//         } else if (current->data) {
//             command_str = ft_strdup(current->data);
//         }
        
//         current = current->next;
//     }
    
//     // If no command found, return empty string
//     if (!command_str) {
//         command_str = ft_strdup("");
//     }
    
//     return command_str;
// }

char *extract_command_with_redirects(t_list **head, t_list **pipe_pos) {
    t_list *current = *head;
    char *command_str = NULL;
    char *temp_str = NULL;
    
    *pipe_pos = NULL;
    
    // Find the first pipe or operation token
    while (current) {
        if ((current->token && ft_strcmp(current->token, "PIPE") == 0) || 
            (current->token && ft_strcmp(current->token, "OPERATION") == 0)) {
            *pipe_pos = current;
            break;
        }
        
        // Build the full command string up to the pipe
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

// t_tree *build_pipe_tree(t_list **head) {
//     t_list *pipe_pos = NULL;
//     t_tree *root = NULL;
//     t_tree *command_node = NULL;
//     char *left_cmd = NULL;
//     char *right_cmd = NULL;
    
//     if (!head || !(*head))
//         return NULL;
    
//     // Extract the complete command string (including redirections)
//     left_cmd = extract_command_with_redirects(head, &pipe_pos);
    
//     if (!pipe_pos) {
//         // No pipe found, create a simple command node
//         root = create_tree_node(left_cmd, "COMMAND");
//         return root;
//     }
    
//     // Create pipe node
//     root = create_tree_node(NULL, "PIPE");
    
//     // Create left command node with the full command string
//     command_node = create_tree_node(left_cmd, "COMMAND");
//     root->left = command_node;
    
//     // Process the right part of the pipe
//     if (pipe_pos->next) {
//         // Extract the right command string by treating pipe_pos->next as head
//         t_list *right_part = pipe_pos->next;
//         t_list *dummy = NULL;
        
//         right_cmd = extract_command_with_redirects(&right_part, &dummy);
//         root->right = create_tree_node(right_cmd, "COMMAND");
//     }
    
//     return root;
// }

// t_tree *build_pipe_tree(t_list **head) {
//     t_list *pipe_pos = NULL;
//     // t_list *op_pos = NULL;
//     t_tree *root = NULL;
//     t_tree *command_node = NULL;
//     char *left_cmd = NULL;
    
//     if (!head || !(*head))
//         return NULL;
    
//     // Extract the complete command string (including redirections)
//     left_cmd = extract_command_with_redirects(head, &pipe_pos);
    
//     if (!pipe_pos) {
//         // No pipe or operation found, create a simple command node
//         root = create_tree_node(left_cmd, "COMMAND");
//         return root;
//     }
    
//     // Check if we found a pipe or an operation
//     if (ft_strcmp(pipe_pos->token, "PIPE") == 0) {
//         // Create pipe node
//         root = create_tree_node(NULL, "PIPE");
        
//         // Create left command node
//         command_node = create_tree_node(left_cmd, "COMMAND");
//         root->left = command_node;
        
//         // Process the right part after the pipe
//         if (pipe_pos->next) {
//             // Recursive call to handle multiple pipes/operations
//             root->right = build_pipe_tree(&(pipe_pos->next));
//         }
//     } else if (ft_strcmp(pipe_pos->token, "OPERATION") == 0) {
//         // Create operation node
//         root = create_tree_node(NULL, "OPERATION");
        
//         // For operations, we need to handle the left part differently
//         // Create a pipe tree for the left part if needed
//         t_list *left_part = *head;
        
//         // Temporarily terminate the list at the operation
//         t_list *temp_next = pipe_pos->next;
//         pipe_pos->next = NULL;
        
//         // Process the left part (might contain pipes)
//         root->left = build_pipe_tree(&left_part);
        
//         // Restore the next pointer and process the right part
//         pipe_pos->next = temp_next;
        
//         if (pipe_pos->next) {
//             // Process the right part of the operation
//             root->right = build_pipe_tree(&(pipe_pos->next));
//         }
//     }
    
//     return root;
// }

t_tree *build_pipe_tree(t_list **head) {
    t_list *pipe_pos = NULL;
    t_tree *root = NULL;
    t_tree *command_node = NULL;
    char *left_cmd = NULL;
    
    if (!head || !(*head))
        return NULL;
    
    // Extract the left command (this doesn't free any nodes)
    left_cmd = extract_command_with_redirects(head, &pipe_pos);
    
    if (!pipe_pos) {
        // No pipe found, create a simple command node
        root = create_tree_node(left_cmd, "COMMAND");
        
        // Now manually free the list
        t_list *current = *head;
        t_list *next = NULL;
        
        while (current) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        
        *head = NULL;
        return root;
    }
    
    if (ft_strcmp(pipe_pos->token, "PIPE") == 0) {
        root = create_tree_node(NULL, "PIPE");
        command_node = create_tree_node(left_cmd, "COMMAND");
        root->left = command_node;
        
        // Free the left part of the list (up to pipe_pos)
        t_list *current = *head;
        t_list *next = NULL;
        
        while (current && current != pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        
        // Now handle the pipe token itself
        if (current == pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        
        // Process the right side recursively
        if (current) {
            t_list *right_head = current;
            root->right = build_pipe_tree(&right_head);
            // right_head should be NULL after the recursive call
        }
        
        *head = NULL;  // Make sure head is NULL
        
    } else if (ft_strcmp(pipe_pos->token, "OPERATION") == 0) {
        root = create_tree_node(NULL, "OPERATION");
        
        // Need to make a copy of the left side to process separately
        t_list *left_head = *head;
        t_list *tmp_list = NULL;
        
        // Create a temporary list for the left part
        while (left_head && left_head != pipe_pos) {
            // Create a new node for the left part
            t_list *new_node = t_malloc(sizeof(t_list), __LINE__, "parsing.c");
            new_node->data = ft_strdup(left_head->data);
            new_node->token = left_head->token;
            new_node->next = NULL;
            new_node->prev = NULL;
            
            // Add to our temporary list
            if (!tmp_list) {
                tmp_list = new_node;
            } else {
                t_list *last = tmp_list;
                while (last->next)
                    last = last->next;
                last->next = new_node;
                new_node->prev = last;
            }
            
            left_head = left_head->next;
        }
        
        // Process the left part
        root->left = build_pipe_tree(&tmp_list);
        
        // Free current list up to and including the pipe_pos
        t_list *current = *head;
        t_list *next = NULL;
        
        while (current && current != pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        
        // Now handle the operation token itself
        if (current == pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        
        // Process the right part
        if (current) {
            t_list *right_head = current;
            root->right = build_pipe_tree(&right_head);
            // right_head should be NULL after the recursive call
        }
        
        *head = NULL;  // Make sure head is NULL
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

// void process_command_with_pipes(char *command_str, t_tree **command_tree) {
//     t_list *cmd_list = NULL;
//     t_list *original_head = NULL;
//     char *cmd_copy = NULL;
    
//     if (!command_str || !(*command_str)) {
//         *command_tree = NULL;
//         return;
//     }
    
//     // Make a copy of the command string
//     cmd_copy = ft_strdup(command_str);
//     if (!cmd_copy) {
//         *command_tree = NULL;
//         return;
//     }
    
//     // Initialize the list
//     cmd_list = list_init_leak(cmd_copy, __LINE__);
//     if (!cmd_list) {
//         t_free(cmd_copy, __LINE__, "parsing.c");
//         *command_tree = NULL;
//         return;
//     }
    
//     // Store the original head for cleanup
//     original_head = cmd_list;
    
//     // Apply lexical analysis
//     lexer(&cmd_list);
    
//     // Build the pipe tree with our improved functions
//     *command_tree = build_pipe_tree(&cmd_list);
    
//     // Clean up all list nodes
//     t_list *current = original_head;
//     t_list *next;
    
//     while (current) {
//         next = current->next;
        
//         if (current->data) {
//             t_free(current->data, __LINE__, "parsing.c");
//             current->data = NULL;
//         }
        
//         t_free(current, __LINE__, "parsing.c");
//         current = next;
//     }
// }

void process_command_with_pipes(char *command_str, t_tree **command_tree) {
    t_list *cmd_list = NULL;
    char *cmd_copy = NULL;
    
    if (!command_str || !(*command_str)) {
        *command_tree = NULL;
        return;
    }
    
    cmd_copy = ft_strdup(command_str);
    if (!cmd_copy) {
        *command_tree = NULL;
        return;
    }
    
    cmd_list = list_init_leak(cmd_copy, __LINE__, "process_command_with_pipes");
    if (!cmd_list) {
        t_free(cmd_copy, __LINE__, "parsing.c");
        *command_tree = NULL;
        return;
    }
    
    // Don't need to store original_head, since we'll fully process the list in build_pipe_tree
    lexer(&cmd_list);
    
    *command_tree = build_pipe_tree(&cmd_list);
    
    // After build_pipe_tree returns, cmd_list should be NULL
    // If it's not, clean up anything that remains (this should not normally happen)
    if (cmd_list) {
        t_list *current = cmd_list;
        t_list *next;
        
        while (current) {
            next = current->next;
            if (current->data) {
                t_free(current->data, __LINE__, "parsing.c");
            }
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        cmd_list = NULL;
    }
    
    // Don't need to free cmd_copy because it gets freed in list_init_leak
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
// void process_pipe_trees(t_tree *tree)
// {
//     t_tree *cmd_tree = NULL;
// 	t_tree *pipe_node;
// 	t_tree *right_child;
// 	t_tree *left_child;
// 	char *command_str;
//     if (!tree)
//         return;
//     if (tree->command && tree->type && ft_strcmp(tree->type, "COMMAND") == 0)
//     {
//         command_str = (char *)tree->command;
//         if (ft_strchr(command_str, '|'))
//         {
//             process_command_with_pipes(command_str, &cmd_tree);
//             if (cmd_tree)
//             {
//                 if (cmd_tree->type && ft_strcmp(cmd_tree->type, "PIPE") == 0)
//                 {
//                     t_free(tree->command, 1158, "parsing.c");
//                     tree->command = NULL;
//                     tree->type = cmd_tree->type;
//                     if (tree->left)
//                         free_tree(tree->left);
//                     tree->left = cmd_tree->left;
//                     tree->right = cmd_tree->right;
//                     cmd_tree->left = NULL;
//                     cmd_tree->right = NULL;
//                     t_free(cmd_tree, 1167, "parsing.c");
//                 }
//                 else
//                 {
//                     free_tree(cmd_tree);
//                 }
//             }
//         }
//     }
//     else if (tree->left && tree->left->command && ft_strchr(tree->left->command, '|'))
//     {
//         process_pipe_trees(tree->left);
//         if (tree->left && ft_strcmp(tree->left->type, "PIPE") == 0 && 
//             ft_strcmp(tree->type, "COMMAND") == 0)
//         {
//             pipe_node = tree->left;
//             right_child = pipe_node->right;
//             tree->type = pipe_node->type;
//             left_child = pipe_node->left;
//             tree->left = left_child;
//             if (tree->right)
//                 free_tree(tree->right);
//             tree->right = right_child;
//             pipe_node->left = NULL;
//             pipe_node->right = NULL;
//             t_free(pipe_node, 1192, "parsing.c");
//         }
//     }
//     if (tree->left)
//         process_pipe_trees(tree->left);
//     if (tree->right)
//         process_pipe_trees(tree->right);
// }

// void process_pipe_trees(t_tree *tree) {
//     t_tree *cmd_tree = NULL;
//     char *command_str;
    
//     if (!tree)
//         return;
    
//     // Process the current node's command if it has one
//     if (tree->command && tree->type && ft_strcmp(tree->type, "COMMAND") == 0) {
//         command_str = (char*)tree->command;
//         if (ft_strchr(command_str, '|') || ft_strchr(command_str, '&')) {
//             // Process the command string if it contains a pipe or operation
//             process_command_with_pipes(command_str, &cmd_tree);
//             if (cmd_tree) {
//                 // Replace current node with the parsed tree
//                 t_free(tree->command, __LINE__, "parsing.c");
//                 tree->command = NULL;
//                 tree->type = cmd_tree->type;
//                 if (tree->left) 
//                     free_tree(tree->left);
//                 if (tree->right)
//                     free_tree(tree->right);
//                 tree->left = cmd_tree->left;
//                 tree->right = cmd_tree->right;
//                 cmd_tree->left = NULL;
//                 cmd_tree->right = NULL;
//                 t_free(cmd_tree, __LINE__, "parsing.c");
//             }
//         }
//     }
    
//     // Recursively process child nodes
//     if (tree->left)
//         process_pipe_trees(tree->left);
//     if (tree->right)
//         process_pipe_trees(tree->right);
// }

// void process_pipe_trees(t_tree *tree) {
//     t_tree *cmd_tree = NULL;
//     char *command_str;
//     char *inner_cmd;
//     if (!tree)
//         return;
    
//     if ((tree->command && tree->type && ft_strcmp(tree->type, "WORD") == 0)) {
        
//         command_str = (char *)tree->command;
        
//         // Handle parentheses content first if it's a PARENTHASIS token
//         if (ft_strcmp(tree->type, "PARENTHASIS") == 0 && has_outer_parentheses(command_str)) {
//             inner_cmd = extract_content_from_parentheses(command_str);
//             if (inner_cmd) {
//                 // Process the inner command
//                 process_command_with_pipes(inner_cmd, &cmd_tree);
//                 if (cmd_tree) {
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                     tree->command = NULL;
//                     tree->type = cmd_tree->type;
//                     if (tree->left) free_tree(tree->left);
//                     if (tree->right) free_tree(tree->right);
//                     tree->left = cmd_tree->left;
//                     tree->right = cmd_tree->right;
//                     cmd_tree->left = NULL;
//                     cmd_tree->right = NULL;
//                     t_free(cmd_tree, __LINE__, "parsing.c");
//                 } else {
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                 }
//             }
//         }
//         // Handle pipes and operations for COMMAND type
//         else if (ft_strchr(command_str, '|') || ft_strchr(command_str, '&')) {
//             process_command_with_pipes(command_str, &cmd_tree);
//             if (cmd_tree) {
//                 t_free(tree->command, __LINE__, "parsing.c");
//                 tree->command = NULL;
//                 tree->type = cmd_tree->type;
//                 if (tree->left) free_tree(tree->left);
//                 if (tree->right) free_tree(tree->right);
//                 tree->left = cmd_tree->left;
//                 tree->right = cmd_tree->right;
//                 cmd_tree->left = NULL;
//                 cmd_tree->right = NULL;
//                 t_free(cmd_tree, __LINE__, "parsing.c");
//             }
//         }
//     }
    
//     if (tree->left)
//         process_pipe_trees(tree->left);
//     if (tree->right)
//         process_pipe_trees(tree->right);
// }

// void process_pipe_trees(t_tree *tree)
// {
//     t_tree *cmd_tree = NULL;
    
//     if (!tree)
//         return;
    
//     // Process the current node first
//     if (tree->command && tree->type)
//     {
//         // Always process PARENTHASIS nodes first
//         if (ft_strcmp(tree->type, "PARENTHASIS") == 0)
//         {
//             char *inner_cmd = extract_content_from_parentheses(tree->command);
//             if (inner_cmd)
//             {
//                 // Process the content of parentheses to detect pipes and operations
//                 process_command_with_pipes(inner_cmd, &cmd_tree);
//                 if (cmd_tree)
//                 {
//                     // Replace current node with the processed tree
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     t_free(inner_cmd, __LINE__, "parsing.c");
                    
//                     tree->command = NULL;
//                     tree->type = cmd_tree->type;
                    
//                     // Clean up existing children
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                    
//                     // Transfer the processed structure
//                     tree->left = cmd_tree->left;
//                     tree->right = cmd_tree->right;
                    
//                     // Nullify pointers to avoid double-free
//                     cmd_tree->left = NULL;
//                     cmd_tree->right = NULL;
//                     t_free(cmd_tree, __LINE__, "parsing.c");
//                 }
//                 else
//                 {
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                 }
//             }
//         }
//         // Also process commands that might contain pipes or operations
//         else if (tree->command && (ft_strchr(tree->command, '|') || ft_strchr(tree->command, '&')))
//         {
//             process_command_with_pipes(tree->command, &cmd_tree);
//             if (cmd_tree)
//             {
//                 // Replace current node with the processed tree
//                 t_free(tree->command, __LINE__, "parsing.c");
//                 tree->command = NULL;
//                 tree->type = cmd_tree->type;
                
//                 // Clean up existing children
//                 if (tree->left)
//                     free_tree(tree->left);
//                 if (tree->right)
//                     free_tree(tree->right);
                
//                 // Transfer the processed structure
//                 tree->left = cmd_tree->left;
//                 tree->right = cmd_tree->right;
                
//                 // Nullify pointers to avoid double-free
//                 cmd_tree->left = NULL;
//                 cmd_tree->right = NULL;
//                 t_free(cmd_tree, __LINE__, "parsing.c");
//             }
//         }
//     }
    
//     // Then process children recursively
//     if (tree->left)
//         process_pipe_trees(tree->left);
//     if (tree->right)
//         process_pipe_trees(tree->right);
// }

void process_pipe_trees(t_tree *tree) {
    if (!tree)
        return;

    // Process pipes and operations first (not parentheses)
    if (tree->command && tree->type && 
        ft_strcmp(tree->type, "PARENTHASIS") != 0 && 
        (ft_strchr(tree->command, '|') || ft_strchr(tree->command, '&'))) {
        
        // Only process pipes or operations outside parentheses
        t_tree *cmd_tree = NULL;
        process_command_with_pipes(tree->command, &cmd_tree);
        
        if (cmd_tree) {
            // Replace with processed tree
            t_free(tree->command, __LINE__, "parsing.c");
            tree->command = NULL;
            tree->type = cmd_tree->type;
            
            // Clean up any existing children
            if (tree->left)
                free_tree(tree->left);
            if (tree->right)
                free_tree(tree->right);
            
            // Transfer the children
            tree->left = cmd_tree->left;
            tree->right = cmd_tree->right;
            
            // Nullify to avoid double-free
            cmd_tree->left = NULL;
            cmd_tree->right = NULL;
            t_free(cmd_tree, __LINE__, "parsing.c");
        }
    }
    
    // Recursively process children
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
                    cmd_list = list_init_leak(inner_cmd_copy, 1886, "process_parentheses_in_tree");
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
// // Add this function to extract redirections from a command string
// void extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
// {
//     char *redir_start = NULL;
//     char *redir_out = ft_strchr(cmd_str, '>');
//     char *redir_in = ft_strchr(cmd_str, '<');
//     int cmd_len = 0;
    
//     // Initialize outputs to NULL
//     *cmd_part = NULL;
//     *redir_part = NULL;
    
//     if (!cmd_str)
//         return;
    
//     // Determine where redirection begins
//     if (redir_out && redir_in) {
//         // Choose the earlier redirection
//         redir_start = (redir_out < redir_in) ? redir_out : redir_in;
//     } else if (redir_out) {
//         redir_start = redir_out;
//     } else if (redir_in) {
//         redir_start = redir_in;
//     } else {
//         // No redirection found
//         *cmd_part = ft_strdup(cmd_str);
//         return;
//     }
    
//     // Check if the redirection is inside parentheses
//     int paren_count = 0;
//     char *p = cmd_str;
//     while (p < redir_start) {
//         if (*p == '(') paren_count++;
//         if (*p == ')') paren_count--;
//         p++;
//     }
    
//     // If we're inside parentheses, redirections are part of the parenthesized command
//     if (paren_count > 0) {
//         // Find the end of parenthesized expression
//         while (*p && paren_count > 0) {
//             if (*p == '(') paren_count++;
//             if (*p == ')') paren_count--;
//             p++;
//         }
        
//         // Look for redirections after parentheses
//         redir_out = ft_strchr(p, '>');
//         redir_in = ft_strchr(p, '<');
        
//         if (redir_out && redir_in) {
//             redir_start = (redir_out < redir_in) ? redir_out : redir_in;
//         } else if (redir_out) {
//             redir_start = redir_out;
//         } else if (redir_in) {
//             redir_start = redir_in;
//         } else {
//             // No redirection found after parentheses
//             *cmd_part = ft_strdup(cmd_str);
//             return;
//         }
//     }
    
//     // Split command into command part and redirection part
//     cmd_len = redir_start - cmd_str;
//     *cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
//     *redir_part = ft_strdup(redir_start);
// }

// void extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
// {
//     char *redir_start = NULL;
//     char *redir_out = ft_strchr(cmd_str, '>');
//     char *redir_in = ft_strchr(cmd_str, '<');
//     int cmd_len = 0;
    
//     // Initialize outputs
//     *cmd_part = NULL;
//     *redir_part = NULL;
    
//     if (!cmd_str)
//         return;
    
//     // Determine where redirection begins
//     if (redir_out && redir_in) {
//         redir_start = (redir_out < redir_in) ? redir_out : redir_in;
//     } else if (redir_out) {
//         redir_start = redir_out;
//     } else if (redir_in) {
//         redir_start = redir_in;
//     } else {
//         // No redirection found
//         *cmd_part = ft_strdup(cmd_str);
//         return;
//     }
    
//     // Check if the redirection is inside balanced parentheses
//     int paren_count = 0;
//     int in_quotes = 0;
//     char quote_type = 0;
//     char *p = cmd_str;
    
//     while (p < redir_start) {
//         // Handle quotes
//         if ((*p == '\'' || *p == '\"') && !in_quotes) {
//             in_quotes = 1;
//             quote_type = *p;
//         } else if (in_quotes && *p == quote_type) {
//             in_quotes = 0;
//         }
        
//         // Count parentheses only when not in quotes
//         if (!in_quotes) {
//             if (*p == '(') paren_count++;
//             if (*p == ')') paren_count--;
//         }
//         p++;
//     }
    
//     // If inside parentheses, look for redirection outside parentheses
//     if (paren_count > 0) {
//         // Find the matching closing parenthesis
//         in_quotes = 0;
//         while (*p) {
//             if ((*p == '\'' || *p == '\"') && !in_quotes) {
//                 in_quotes = 1;
//                 quote_type = *p;
//             } else if (in_quotes && *p == quote_type) {
//                 in_quotes = 0;
//             }
            
//             if (!in_quotes) {
//                 if (*p == '(') paren_count++;
//                 if (*p == ')') paren_count--;
//                 if (paren_count == 0) break;
//             }
//             p++;
//         }
        
//         // Skip past closing parenthesis
//         if (*p) p++;
        
//         // Look for redirection after balanced parentheses
//         redir_out = ft_strchr(p, '>');
//         redir_in = ft_strchr(p, '<');
        
//         if (redir_out && redir_in) {
//             redir_start = (redir_out < redir_in) ? redir_out : redir_in;
//         } else if (redir_out) {
//             redir_start = redir_out;
//         } else if (redir_in) {
//             redir_start = redir_in;
//         } else {
//             // No redirection after parentheses
//             *cmd_part = ft_strdup(cmd_str);
//             return;
//         }
//     }
    
//     // Split the command into command part and redirection part
//     cmd_len = redir_start - cmd_str;
//     *cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
//     *redir_part = ft_strdup(redir_start);
// }
// void extract_redirections(char *cmd_str, char **cmd_part, char **redir_part)
// {
//     char *redir_start = NULL;
//     char *redir_out = ft_strchr(cmd_str, '>');
//     char *redir_in = ft_strchr(cmd_str, '<');
//     int cmd_len = 0;
    
//     *cmd_part = NULL;
//     *redir_part = NULL;
    
//     if (!cmd_str)
//         return;
    
//     // First check for heredoc specifically as it needs special handling
//     char *heredoc = ft_strstr(cmd_str, "<<");
//     if (heredoc) {
//         // For heredoc, prioritize it to ensure it stays with the command
//         redir_start = heredoc;
//     }
//     else if (redir_out && redir_in) {
//         // If both types of redirection exist, take the first one
//         redir_start = (redir_out < redir_in) ? redir_out : redir_in;
//     } else if (redir_out) {
//         redir_start = redir_out;
//     } else if (redir_in) {
//         redir_start = redir_in;
//     } else {
//         // No redirection found
//         *cmd_part = ft_strdup(cmd_str);
//         return;
//     }
    
//     // Check if the redirection is inside balanced parentheses
//     int paren_count = 0;
//     int in_quotes = 0;
//     char quote_type = 0;
//     char *p = cmd_str;
    
//     // Process text before redirection to check for balanced structures
//     while (p < redir_start) {
//         // Handle quotes
//         if ((*p == '\'' || *p == '\"') && !in_quotes) {
//             in_quotes = 1;
//             quote_type = *p;
//         } else if (in_quotes && *p == quote_type) {
//             in_quotes = 0;
//         }
        
//         // Count parentheses only when not in quotes
//         if (!in_quotes) {
//             if (*p == '(') paren_count++;
//             if (*p == ')') paren_count--;
//         }
//         p++;
//     }
    
//     // Handle pipe character specially with heredocs
//     char *pipe_char = ft_strchr(cmd_str, '|');
    
//     // If inside parentheses, look for redirection outside or after parentheses
//     if (paren_count > 0) {
//         // Find the matching closing parenthesis
//         in_quotes = 0;
//         while (*p) {
//             if ((*p == '\'' || *p == '\"') && !in_quotes) {
//                 in_quotes = 1;
//                 quote_type = *p;
//             } else if (in_quotes && *p == quote_type) {
//                 in_quotes = 0;
//             }
            
//             if (!in_quotes) {
//                 if (*p == '(') paren_count++;
//                 if (*p == ')') paren_count--;
//                 if (paren_count == 0) break;
//             }
//             p++;
//         }
        
//         // Skip past closing parenthesis
//         if (*p) p++;
        
//         // Look for redirection after balanced parentheses
//         redir_out = ft_strchr(p, '>');
//         redir_in = ft_strchr(p, '<');
        
//         if (redir_out && redir_in) {
//             redir_start = (redir_out < redir_in) ? redir_out : redir_in;
//         } else if (redir_out) {
//             redir_start = redir_out;
//         } else if (redir_in) {
//             redir_start = redir_in;
//         } else {
//             // No redirection after parentheses
//             *cmd_part = ft_strdup(cmd_str);
//             return;
//         }
//     }
//     // Special case for heredoc with a pipe
//     else if (heredoc && pipe_char && pipe_char > heredoc) {
//         // Extract the heredoc delimiter
//         p = heredoc + 2; // Skip past "<<"
        
//         // Skip spaces
//         while (*p && (*p == ' ' || *p == '\t')) p++;
        
//         // Find the end of the heredoc delimiter
//         // char *delim_start = p;
//         while (*p && *p != ' ' && *p != '\t' && *p != '|') p++;
        
//         // If there's a pipe after the heredoc, handle specially to keep heredoc with command
//         if (pipe_char > p) {
//             // For commands like "cat << EOF | grep", we want the heredoc to stay with "cat"
//             // So split at the pipe, not the heredoc
//             cmd_len = pipe_char - cmd_str;
//             *cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
//             *redir_part = heredoc;
//             return;
//         }
//     }
    
//     // Standard case - split at the redirection
//     cmd_len = redir_start - cmd_str;
//     *cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
//     *redir_part = ft_strdup(redir_start);
// }
void extract_redirections(char *cmd_str, char **cmd_part, char **redir_part) {
    char *redir_start = NULL;
    char *redir_out = ft_strchr(cmd_str, '>');
    char *redir_in = ft_strchr(cmd_str, '<');
    int cmd_len = 0;
    
    *cmd_part = NULL;
    *redir_part = NULL;
    
    if (!cmd_str)
        return;
    if (redir_out && redir_in) {
        redir_start = (redir_out < redir_in) ? redir_out : redir_in;
    } else if (redir_out) {
        redir_start = redir_out;
    } else if (redir_in) {
        redir_start = redir_in;
    } else {
        *cmd_part = ft_strdup(cmd_str);
        return;
    }
    
    int paren_count = 0;
    int in_quotes = 0;
    char quote_type = 0;
    char *p = cmd_str;
    
    while (p < redir_start) {
        if ((*p == '\'' || *p == '\"') && !in_quotes) {
            in_quotes = 1;
            quote_type = *p;
        } else if (in_quotes && *p == quote_type) {
            in_quotes = 0;
        }
        if (!in_quotes) {
            if (*p == '(') paren_count++;
            if (*p == ')') paren_count--;
        }
        p++;
    }
    
    // char *pipe_char = ft_strchr(cmd_str, '|');
    if (paren_count > 0) {
        in_quotes = 0;
        while (*p) {
            if ((*p == '\'' || *p == '\"') && !in_quotes) {
                in_quotes = 1;
                quote_type = *p;
            } else if (in_quotes && *p == quote_type) {
                in_quotes = 0;
            }
            if (!in_quotes) {
                if (*p == '(') paren_count++;
                if (*p == ')') paren_count--;
                if (paren_count == 0) break;
            }
            p++;
        }
        if (*p) p++;
        redir_out = ft_strchr(p, '>');
        redir_in = ft_strchr(p, '<');
        if (redir_out && redir_in) {
            redir_start = (redir_out < redir_in) ? redir_out : redir_in;
        } else if (redir_out) {
            redir_start = redir_out;
        } else if (redir_in) {
            redir_start = redir_in;
        } else {
            *cmd_part = ft_strdup(cmd_str);
            return;
        }
    }
    
    cmd_len = redir_start - cmd_str;
    *cmd_part = ft_substr_leak(cmd_str, 0, cmd_len, __LINE__);
    *redir_part = ft_strdup(redir_start);
}


// void process_all_parentheses(t_tree *tree)
// {
//     if (!tree)
//         return;
    
//     // First, process any parenthesis nodes
//     if (tree->command && tree->type && ft_strcmp(tree->type, "WORD") == 0)
//     {
//         // Extract the content inside the parentheses
//         char *inner_cmd = extract_content_from_parentheses(tree->command);
//         if (inner_cmd)
//         {
//             // Create a list from the inner command string
//             t_list *cmd_list = list_init_leak(ft_strdup(inner_cmd), 1969, "process_all_parentheses");
//             if (cmd_list)
//             {
//                 // Tokenize the list
//                 lexer(&cmd_list);
                
//                 // Create a subtree from the tokens
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 // Process any pipes or nested operations within the subtree
//                 if (subtree)
//                 {
//                     // Process any pipes or operations in the subtree
//                     process_pipe_trees(subtree);
                    
//                     // Replace the current node with the processed subtree
//                     t_free(tree->command, 1422, "parsing.c");
//                     tree->command = NULL;
                    
//                     // Transfer type and tree structure
//                     tree->type = subtree->type;
                    
//                     // Clean up any existing children
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                    
//                     // Transfer the children
//                     tree->left = subtree->left;
//                     tree->right = subtree->right;
                    
//                     // Nullify subtree pointers to avoid double-free
//                     subtree->left = NULL;
//                     subtree->right = NULL;
                    
//                     // Free the subtree itself
//                     t_free(subtree, 1433, "parsing.c");
//                 }
//             }
//             // Free the extracted command
//             t_free(inner_cmd, 1436, "parsing.c");
//         }
//     }
    
//     // Process children recursively after processing this node
//     // This ensures that parentheses are processed depth-first
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }

// void process_all_parentheses(t_tree *tree) {
//     if (!tree)
//         return;

//     // Process parentheses nodes
//     if (tree->command && tree->type && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
//         char *inner_cmd = extract_content_from_parentheses(tree->command);
//         if (inner_cmd) {
//             // Create a list from the inner command
//             t_list *cmd_list = list_init_leak(ft_strdup(inner_cmd), __LINE__, "process_all_parentheses");
//             if (cmd_list) {
//                 // Process the list
//                 lexer(&cmd_list);
                
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 if (subtree) {
//                     // Process pipes and operations inside the parentheses
//                     process_pipe_trees(subtree);
                    
//                     // Replace the current node with the expanded subtree
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     tree->command = NULL;
//                     tree->type = subtree->type;
                    
//                     // Clean up existing children
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                    
//                     // Transfer the children
//                     tree->left = subtree->left;
//                     tree->right = subtree->right;
                    
//                     // Avoid double-free
//                     subtree->left = NULL;
//                     subtree->right = NULL;
//                     t_free(subtree, __LINE__, "parsing.c");
//                 }
//             }
//             t_free(inner_cmd, __LINE__, "parsing.c");
//         }
//     }
    
//     // Process children after the current node
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }

// void process_all_parentheses(t_tree *tree) {
//     if (!tree)
//         return;

//     // Process PARENTHASIS nodes
//     if (tree->type && ft_strcmp(tree->type, "PARENTHASIS") == 0 && tree->command) {
//         // Extract content from parentheses
//         char *inner_cmd = extract_content_from_parentheses(tree->command);
//         if (inner_cmd) {
            
//             // Create token list from inner command
//             t_list *cmd_list = list_init_leak(ft_strdup(inner_cmd), __LINE__, "process_all_parentheses");
//             if (cmd_list) {
//                 // Process the tokens
//                 lexer(&cmd_list);
                
//                 // Debug print the tokens
//                 t_list *tmp = cmd_list;
//                 while (tmp) {
//                     tmp = tmp->next;
//                 }
                
//                 // Create subtree
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 // If subtree was created successfully
//                 if (subtree) {
//                     // Free the original command string
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     tree->command = NULL;
                    
//                     // If subtree has a command, copy it to current node
//                     if (subtree->command) {
//                         tree->command = ft_strdup(subtree->command);
//                         tree->type = "COMMAND"; // Change type to COMMAND
                        
//                         // If the subtree has children, we need to keep them
//                         if (subtree->left || subtree->right) {
//                             if (tree->left) free_tree(tree->left);
//                             if (tree->right) free_tree(tree->right);
//                             tree->left = subtree->left;
//                             tree->right = subtree->right;
//                             subtree->left = NULL;
//                             subtree->right = NULL;
//                         }
//                     } else {
//                         // If subtree doesn't have a command, it's a pipe or operation
//                         tree->type = subtree->type;
//                         if (tree->left) free_tree(tree->left);
//                         if (tree->right) free_tree(tree->right);
//                         tree->left = subtree->left;
//                         tree->right = subtree->right;
//                         subtree->left = NULL;
//                         subtree->right = NULL;
//                     }
                    
//                     // Free the subtree (but not its children)
//                     t_free(subtree, __LINE__, "parsing.c");
//                 }
//             }
//             // Free the extracted command
//             t_free(inner_cmd, __LINE__, "parsing.c");
//         }
//     }
    
//     // Process children recursively
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }

// void process_all_parentheses(t_tree *tree)
// {
//     if (!tree)
//         return;
    
//     // First handle any "PARENTHASIS" nodes
//     if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
//         // Extract content from parentheses
//         char *inner_cmd = extract_content_from_parentheses(tree->command);
//         if (inner_cmd) {
//             // Make a copy to tokenize
//             char *inner_copy = ft_strdup(inner_cmd);
//             if (inner_copy) {
//                 // Create a token list from the inner command
//                 t_list *cmd_list = list_init_leak(inner_copy, __LINE__, "process_all_parentheses");
//                 if (cmd_list) {
//                     // Process into tokens
//                     lexer(&cmd_list);
                    
//                     // Create a subtree from tokens
//                     t_tree *subtree = NULL;
//                     tree_maker(&cmd_list, &subtree);
                    
//                     if (subtree) {
//                         // Process any pipes/operations inside parentheses
//                         process_pipe_trees(subtree);
                        
//                         // Replace current node with processed subtree
//                         t_free(tree->command, __LINE__, "parsing.c");
//                         tree->command = NULL;
                        
//                         // If subtree has a command, take it
//                         if (subtree->command) {
//                             tree->command = ft_strdup(subtree->command);
//                         }
                        
//                         // Set the type based on the subtree
//                         tree->type = subtree->type;
                        
//                         // Handle any children
//                         if (tree->left)
//                             free_tree(tree->left);
//                         if (tree->right)
//                             free_tree(tree->right);
                            
//                         // Transfer the children
//                         tree->left = subtree->left;
//                         tree->right = subtree->right;
                        
//                         // Prevent double freeing
//                         subtree->left = NULL;
//                         subtree->right = NULL;
                        
//                         // Free the subtree node itself
//                         t_free(subtree, __LINE__, "parsing.c");
//                     }
//                 }
//             }
//             // Free the extracted command
//             t_free(inner_cmd, __LINE__, "parsing.c");
//         }
//     }
    
//     // Then process children recursively
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }

// void process_all_parentheses(t_tree *tree)
// {
//     if (!tree)
//         return;
    
//     // First handle any "PARENTHASIS" nodes
//     if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
//         // Extract content from parentheses
//         char *inner_cmd = extract_content_from_parentheses(tree->command);
//         if (inner_cmd) {
//             // Create a token list from the inner command
//             t_list *cmd_list = list_init_leak(inner_cmd, __LINE__, "process_all_parentheses");
//             if (cmd_list) {
//                 // Process into tokens
//                 lexer(&cmd_list);
                
//                 // Create a subtree from tokens
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 if (subtree) {
//                     // Process any pipes/operations inside parentheses
//                     process_pipe_trees(subtree);
                    
//                     // Free the original command string
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     tree->command = NULL;
                    
//                     // If subtree has a command, set it directly (without duplicating)
//                     if (subtree->command) {
//                         tree->command = subtree->command;
//                         subtree->command = NULL;  // Prevent double free
//                     }
                    
//                     // Set the type
//                     if (subtree->type)
//                         tree->type = subtree->type;
//                     else
//                         tree->type = "COMMAND";
                    
//                     // Free existing children
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                        
//                     // Transfer children
//                     tree->left = subtree->left;
//                     tree->right = subtree->right;
//                     subtree->left = NULL;
//                     subtree->right = NULL;
                    
//                     // Free subtree node only
//                     t_free(subtree, __LINE__, "parsing.c");
//                 } 
//                 else {
//                     // Free the extracted command if subtree creation failed
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                 }
//             }
//             else {
//                 // Free the extracted command if list creation failed
//                 t_free(inner_cmd, __LINE__, "parsing.c");
//             }
//         }
//     }
    
//     // Then process children recursively
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }

// void process_all_parentheses(t_tree *tree) {
//     if (!tree)
//         return;
        
//     if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
//         // Check if this parenthesized command has redirections
//         char *cmd_part = NULL;
//         char *redir_part = NULL;
//         extract_redirections(tree->command, &cmd_part, &redir_part);
        
//         // Extract the content inside the parentheses
//         char *inner_cmd = NULL;
//         if (cmd_part && cmd_part[0] == '(') {
//             inner_cmd = extract_content_from_parentheses(cmd_part);
//             t_free(cmd_part, __LINE__, "process_all_parentheses");
//         } else if (cmd_part) {
//             inner_cmd = cmd_part;  // No parentheses, use as is
//         } else {
//             inner_cmd = extract_content_from_parentheses(tree->command);
//         }
        
//         if (inner_cmd) {
//             t_list *cmd_list = list_init_leak(inner_cmd, __LINE__, "process_all_parentheses");
//             if (cmd_list) {
//                 lexer(&cmd_list);
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 if (subtree) {
//                     process_pipe_trees(subtree);
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     tree->command = NULL;
                    
//                     // Preserve the redirection information
//                     if (redir_part) {
//                         tree->redirections = redir_part;
//                     }
                    
//                     if (subtree->command) {
//                         tree->command = subtree->command;
//                         subtree->command = NULL;
//                     }
                    
//                     if (subtree->type)
//                         tree->type = subtree->type;
//                     else
//                         tree->type = "COMMAND";
                        
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                        
//                     tree->left = subtree->left;
//                     tree->right = subtree->right;
//                     subtree->left = NULL;
//                     subtree->right = NULL;
//                     t_free(subtree, __LINE__, "parsing.c");
//                 } else {
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                     if (redir_part)
//                         t_free(redir_part, __LINE__, "parsing.c");
//                 }
//             } else {
//                 t_free(inner_cmd, __LINE__, "parsing.c");
//                 if (redir_part)
//                     t_free(redir_part, __LINE__, "parsing.c");
//             }
//         } else if (redir_part) {
//             t_free(redir_part, __LINE__, "parsing.c");
//         }
//     }
    
//     // Process children
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }

// void process_all_parentheses(t_tree *tree)
// {
//     if (!tree)
//         return;
    
//     if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
//         // Extract redirections if any
//         char *cmd_part = NULL;
//         char *redir_part = NULL;
//         extract_redirections(tree->command, &cmd_part, &redir_part);
        
//         // Get content inside parentheses
//         char *inner_cmd = NULL;
//         if (cmd_part && cmd_part[0] == '(') {
//             inner_cmd = extract_content_from_parentheses(cmd_part);
//             t_free(cmd_part, __LINE__, "process_all_parentheses");
//         } else if (cmd_part) {
//             inner_cmd = cmd_part; // Use cmd_part if no parentheses
//         } else {
//             inner_cmd = extract_content_from_parentheses(tree->command);
//         }
        
//         if (inner_cmd) {
//             // Parse the inner command
//             t_list *cmd_list = list_init_leak(inner_cmd, __LINE__, "process_all_parentheses");
            
//             if (cmd_list) {
//                 lexer(&cmd_list);
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 if (subtree) {
//                     // Process any pipes in the subtree
//                     process_pipe_trees(subtree);
                    
//                     // Free original command and replace with processed subtree
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     tree->command = NULL;
                    
//                     // Save redirection part if found
//                     if (redir_part) {
//                         tree->redirections = redir_part;
//                     }
                    
//                     // Copy command from subtree
//                     if (subtree->command) {
//                         tree->command = subtree->command;
//                         subtree->command = NULL;
//                     }
                    
//                     // Copy type from subtree
//                     if (subtree->type)
//                         tree->type = subtree->type;
//                     else
//                         tree->type = "COMMAND";
                    
//                     // Transfer children from subtree
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                        
//                     tree->left = subtree->left;
//                     tree->right = subtree->right;
//                     subtree->left = NULL;
//                     subtree->right = NULL;
                    
//                     // Free the subtree structure
//                     t_free(subtree, __LINE__, "parsing.c");
//                 } else {
//                     // Clean up if subtree creation failed
//                     if (inner_cmd != cmd_part)
//                         t_free(inner_cmd, __LINE__, "parsing.c");
//                     if (redir_part)
//                         t_free(redir_part, __LINE__, "parsing.c");
//                 }
//             } else {
//                 // Clean up if cmd_list creation failed
//                 if (inner_cmd != cmd_part)
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                 if (redir_part)
//                     t_free(redir_part, __LINE__, "parsing.c");
//             }
//         } else if (redir_part) {
//             // Clean up if inner_cmd extraction failed
//             t_free(redir_part, __LINE__, "parsing.c");
//         }
//     }
    
//     // Process child nodes
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }

// void process_all_parentheses(t_tree *tree)
// {
//     if (!tree)
//         return;
    
//     if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
//         // Extract and separate redirections in the parenthesized expression
//         char *cmd_part = NULL;
//         char *redir_part = NULL;
//         extract_redirections(tree->command, &cmd_part, &redir_part);
        
//         // Process the command part
//         char *inner_cmd = NULL;
//         if (cmd_part && cmd_part[0] == '(') {
//             inner_cmd = extract_content_from_parentheses(cmd_part);
//             t_free(cmd_part, __LINE__, "process_all_parentheses");
//         } else if (cmd_part) {
//             inner_cmd = cmd_part;
//         } else {
//             inner_cmd = extract_content_from_parentheses(tree->command);
//         }
        
//         if (inner_cmd) {
//             // Parse the inner command
//             t_list *cmd_list = list_init_leak(inner_cmd, __LINE__, "process_all_parentheses");
//             if (cmd_list) {
//                 lexer(&cmd_list);
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 if (subtree) {
//                     // Process pipes inside the subtree
//                     process_pipe_trees(subtree);
                    
//                     // Replace the parenthesized command with the processed subtree
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     tree->command = NULL;
                    
//                     // Store any redirections found
//                     if (redir_part) {
//                         tree->redirections = redir_part;
//                     }
                    
//                     // Set command and type from subtree
//                     if (subtree->command) {
//                         tree->command = subtree->command;
//                         subtree->command = NULL;
//                     }
                    
//                     if (subtree->type)
//                         tree->type = subtree->type;
//                     else
//                         tree->type = "COMMAND";
                    
//                     // Transfer children
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                    
//                     tree->left = subtree->left;
//                     tree->right = subtree->right;
//                     subtree->left = NULL;
//                     subtree->right = NULL;
                    
//                     t_free(subtree, __LINE__, "parsing.c");
//                 } else {
//                     // Failed to create subtree, clean up
//                     if (inner_cmd != cmd_part)
//                         t_free(inner_cmd, __LINE__, "parsing.c");
//                     if (redir_part)
//                         t_free(redir_part, __LINE__, "parsing.c");
//                 }
//             } else {
//                 // Failed to parse inner command, clean up
//                 if (inner_cmd != cmd_part)
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                 if (redir_part)
//                     t_free(redir_part, __LINE__, "parsing.c");
//             }
//         } else if (redir_part) {
//             t_free(redir_part, __LINE__, "parsing.c");
//         }
//     }
    
//     // Process children recursively
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }
// void process_all_parentheses(t_tree *tree)
// {
//     if (!tree)
//         return;
    
//     if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
//         // Extract any redirections from the command
//         char *cmd_part = NULL;
//         char *redir_part = NULL;
//         extract_redirections(tree->command, &cmd_part, &redir_part);
        
//         // Get the content inside parentheses
//         char *inner_cmd = NULL;
//         if (cmd_part && cmd_part[0] == '(') {
//             inner_cmd = extract_content_from_parentheses(cmd_part);
//             t_free(cmd_part, __LINE__, "process_all_parentheses");
//         } else if (cmd_part) {
//             inner_cmd = cmd_part;  // Use cmd_part if no parentheses
//         } else {
//             inner_cmd = extract_content_from_parentheses(tree->command);
//         }
        
//         if (inner_cmd) {
//             // Parse the inner command into tokens
//             t_list *cmd_list = list_init_leak(inner_cmd, __LINE__, "process_all_parentheses");
//             if (cmd_list) {
//                 lexer(&cmd_list);
                
//                 // Check if we have a heredoc redirection in this parenthesized content
//                 int has_heredoc = 0;
//                 t_list *tmp = cmd_list;
//                 while (tmp) {
//                     if (tmp->data && ft_strstr(tmp->data, "<<")) {
//                         has_heredoc = 1;
//                         break;
//                     }
//                     tmp = tmp->next;
//                 }
                
//                 // Build a syntax tree from the tokens
//                 t_tree *subtree = NULL;
//                 tree_maker(&cmd_list, &subtree);
                
//                 if (subtree) {
//                     // Process pipe operations in the subtree
//                     process_pipe_trees(subtree);
                    
//                     // Free the original command
//                     t_free(tree->command, __LINE__, "parsing.c");
//                     tree->command = NULL;
                    
//                     // Save the redirection info if found
//                     if (redir_part) {
//                         tree->redirections = redir_part;
//                     }
                    
//                     // Handle heredoc inside pipe specially
//                     if (has_heredoc && subtree->type && ft_strcmp(subtree->type, "PIPE") == 0) {
//                         // For commands with pipes like "cat << EOF | grep", the heredoc should
//                         // be associated with the left-side command ("cat")
//                         if (subtree->left && !subtree->left->redirections) {
//                             // Look for heredoc in remaining command text
//                             if (subtree->command) {
//                                 char *cmd_part_sub = NULL;
//                                 char *redir_part_sub = NULL;
//                                 extract_redirections(subtree->command, &cmd_part_sub, &redir_part_sub);
                                
//                                 if (redir_part_sub && ft_strstr(redir_part_sub, "<<")) {
//                                     // Associate the heredoc with the left side of the pipe
//                                     subtree->left->redirections = redir_part_sub;
                                    
//                                     // Update the remaining command
//                                     if (cmd_part_sub) {
//                                         t_free(subtree->command, __LINE__, "parsing.c");
//                                         subtree->command = cmd_part_sub;
//                                     }
//                                 } else {
//                                     if (cmd_part_sub) t_free(cmd_part_sub, __LINE__, "parsing.c");
//                                     if (redir_part_sub) t_free(redir_part_sub, __LINE__, "parsing.c");
//                                 }
//                             }
//                         }
//                     }
                    
//                     // Transfer command and properties from subtree to this node
//                     if (subtree->command) {
//                         tree->command = subtree->command;
//                         subtree->command = NULL;
//                     }
                    
//                     if (subtree->type)
//                         tree->type = subtree->type;
//                     else
//                         tree->type = "COMMAND";
                    
//                     // Replace children
//                     if (tree->left)
//                         free_tree(tree->left);
//                     if (tree->right)
//                         free_tree(tree->right);
                    
//                     tree->left = subtree->left;
//                     tree->right = subtree->right;
//                     subtree->left = NULL;
//                     subtree->right = NULL;
                    
//                     t_free(subtree, __LINE__, "parsing.c");
//                 } else {
//                     // Clean up if subtree creation failed
//                     if (inner_cmd != cmd_part)
//                         t_free(inner_cmd, __LINE__, "parsing.c");
//                     if (redir_part)
//                         t_free(redir_part, __LINE__, "parsing.c");
//                 }
//             } else {
//                 // Clean up if list creation failed
//                 if (inner_cmd != cmd_part)
//                     t_free(inner_cmd, __LINE__, "parsing.c");
//                 if (redir_part)
//                     t_free(redir_part, __LINE__, "parsing.c");
//             }
//         } else if (redir_part) {
//             t_free(redir_part, __LINE__, "parsing.c");
//         }
//     }
    
//     // Process children recursively
//     if (tree->left)
//         process_all_parentheses(tree->left);
//     if (tree->right)
//         process_all_parentheses(tree->right);
// }
void process_all_parentheses(t_tree *tree) {
    if (!tree)
        return;
        
    if (tree->type && tree->command && ft_strcmp(tree->type, "PARENTHASIS") == 0) {
        char *cmd_part = NULL;
        char *redir_part = NULL;
        extract_redirections(tree->command, &cmd_part, &redir_part);
        
        char *inner_cmd = NULL;
        if (cmd_part && cmd_part[0] == '(') {
            inner_cmd = extract_content_from_parentheses(cmd_part);
            t_free(cmd_part, __LINE__, "process_all_parentheses");
        } else if (cmd_part) {
            inner_cmd = cmd_part;
        } else {
            inner_cmd = extract_content_from_parentheses(tree->command);
        }
        
        if (inner_cmd) {
            t_list *cmd_list = list_init_leak(inner_cmd, __LINE__, "process_all_parentheses");
            
            if (cmd_list) {
                lexer(&cmd_list);
                t_tree *subtree = NULL;
                tree_maker(&cmd_list, &subtree);
                
                if (subtree) {
                    process_pipe_trees(subtree);
                    t_free(tree->command, __LINE__, "parsing.c");
                    tree->command = NULL;
                    
                    if (redir_part) {
                        tree->redirections = redir_part;
                    }
                    if (subtree->command) {
                        tree->command = subtree->command;
                        subtree->command = NULL;
                    }
                    
                    if (subtree->type)
                        tree->type = subtree->type;
                    else
                        tree->type = "COMMAND";
                        
                    if (tree->left)
                        free_tree(tree->left);
                    if (tree->right)
                        free_tree(tree->right);
                        
                    tree->left = subtree->left;
                    tree->right = subtree->right;
                    subtree->left = NULL;
                    subtree->right = NULL;
                    
                    t_free(subtree, __LINE__, "parsing.c");
                } else {
                    if (inner_cmd != cmd_part)
                        t_free(inner_cmd, __LINE__, "parsing.c");
                    if (redir_part)
                        t_free(redir_part, __LINE__, "parsing.c");
                }
            } else {
                if (inner_cmd != cmd_part)
                    t_free(inner_cmd, __LINE__, "parsing.c");
                if (redir_part)
                    t_free(redir_part, __LINE__, "parsing.c");
            }
        } else if (redir_part) {
            t_free(redir_part, __LINE__, "parsing.c");
        }
    }
    
    if (tree->left)
        process_all_parentheses(tree->left);
    if (tree->right)
        process_all_parentheses(tree->right);
}

// void process_nested_parentheses(t_tree **tree)
// {
//     if (!tree || !(*tree))
//         return;
    
//     // Process all parentheses in this node first
//     int passes = 0;
//     int max_passes = 5;  // Limit to avoid infinite loops
    
//     // Process children first (bottom-up approach)
//     if ((*tree)->left)
//         process_nested_parentheses(&((*tree)->left));
//     if ((*tree)->right)
//         process_nested_parentheses(&((*tree)->right));
    
//     // Then process this node
//     while (passes < max_passes)
//     {
//         process_all_parentheses(*tree);
//         fix_operation_tree_structure(*tree);
//         passes++;
//     }
// }

// void process_nested_parentheses(t_tree **tree) {
//     if (!tree || !(*tree))
//         return;
    
//     // Process children first (bottom-up)
//     if ((*tree)->left)
//         process_nested_parentheses(&((*tree)->left));
//     if ((*tree)->right)
//         process_nested_parentheses(&((*tree)->right));
    
//     // Then process the current node
//     int passes = 0;
//     int max_passes = 5;
//     while (passes < max_passes) {
//         process_all_parentheses(*tree);
//         fix_operation_tree_structure(*tree);
//         passes++;
//     }
// }
// void process_nested_parentheses(t_tree **tree)
// {
//     if (!tree || !(*tree))
//         return;
    
//     // Process children first
//     if ((*tree)->left)
//         process_nested_parentheses(&((*tree)->left));
//     if ((*tree)->right)
//         process_nested_parentheses(&((*tree)->right));
    
//     // Process parentheses in this node
//     int passes = 0;
//     int max_passes = 5; // Limit to prevent infinite loops
    
//     while (passes < max_passes) {
//         process_all_parentheses(*tree);
//         fix_operation_tree_structure(*tree);
//         passes++;
//     }
    
//     // DON'T automatically move redirections from right child to parent for OPERATION nodes
//     // Only do this for PIPE nodes where it makes sense
//     if ((*tree)->type && ft_strcmp((*tree)->type, "PIPE") == 0) {
//         if ((*tree)->right && (*tree)->right->redirections && !(*tree)->redirections) {
//             (*tree)->redirections = ft_strdup((*tree)->right->redirections);
//             t_free((*tree)->right->redirections, __LINE__, "process_nested_parentheses");
//             (*tree)->right->redirections = NULL;
//         }
//     }
    
//     // For top-level operations, don't automatically pull up redirections
//     // Let them stay with their respective commands
// }
// void process_nested_parentheses(t_tree **tree)
// {
//     if (!tree || !(*tree))
//         return;
    
//     // Process children recursively first
//     if ((*tree)->left)
//         process_nested_parentheses(&((*tree)->left));
//     if ((*tree)->right)
//         process_nested_parentheses(&((*tree)->right));
    
//     // Process this node
//     int passes = 0;
//     int max_passes = 5;  // Limit recursion to prevent infinite loops
    
//     while (passes < max_passes) {
//         process_all_parentheses(*tree);
//         fix_operation_tree_structure(*tree);
//         passes++;
//     }
    
//     // Special handling for pipe nodes - move heredoc to left child if needed
//     if ((*tree)->type && ft_strcmp((*tree)->type, "PIPE") == 0) {
//         if ((*tree)->command) {
//             // Check if the pipe command has a heredoc that should be moved to left child
//             char *heredoc = ft_strstr((*tree)->command, "<<");
//             if (heredoc && (*tree)->left && !(*tree)->left->redirections) {
//                 char *cmd_part = NULL;
//                 char *redir_part = NULL;
//                 extract_redirections((*tree)->command, &cmd_part, &redir_part);
                
//                 if (redir_part && ft_strstr(redir_part, "<<")) {
//                     // Move heredoc to left child
//                     (*tree)->left->redirections = redir_part;
                    
//                     // Update pipe node's command
//                     if (cmd_part) {
//                         t_free((*tree)->command, __LINE__, "process_nested_parentheses");
//                         (*tree)->command = cmd_part;
//                     }
//                 } else {
//                     if (cmd_part) t_free(cmd_part, __LINE__, "process_nested_parentheses");
//                     if (redir_part) t_free(redir_part, __LINE__, "process_nested_parentheses");
//                 }
//             }
//         }
//     }
    
//     // For operations, don't automatically move redirections from right child
//     // Only do this for specific cases like pipes where it makes sense
//     if ((*tree)->type && ft_strcmp((*tree)->type, "PIPE") == 0) {
//         // For pipes, move right child's redirections to parent if parent has none
//         if ((*tree)->right && (*tree)->right->redirections && !(*tree)->redirections) {
//             (*tree)->redirections = ft_strdup((*tree)->right->redirections);
//             t_free((*tree)->right->redirections, __LINE__, "process_nested_parentheses");
//             (*tree)->right->redirections = NULL;
//         }
//     }
// }

void process_nested_parentheses(t_tree **tree) {
    if (!tree || !(*tree))
        return;
        
    if ((*tree)->left)
        process_nested_parentheses(&((*tree)->left));
    if ((*tree)->right)
        process_nested_parentheses(&((*tree)->right));
        
    int passes = 0;
    int max_passes = 5;
    
    while (passes < max_passes) {
        process_all_parentheses(*tree);
        fix_operation_tree_structure(*tree);
        passes++;
    }
    
    if ((*tree)->type && ft_strcmp((*tree)->type, "PIPE") == 0) {
        if ((*tree)->right && (*tree)->right->redirections && !(*tree)->redirections) {
            (*tree)->redirections = ft_strdup((*tree)->right->redirections);
            t_free((*tree)->right->redirections, __LINE__, "process_nested_parentheses");
            (*tree)->right->redirections = NULL;
        }
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

// void process_all_redirections(t_tree **tree)
// {
//     char *cmd;
//     char *redir_out;
//     char *redir_in;
//     char *first_redir;
//     char *cmd_prefix;
//     char *command_part;
//     int cmd_length;

//     if (!tree || !*tree)
//         return;
//     if ((*tree)->left)
//         process_all_redirections(&(*tree)->left);
//     if ((*tree)->right)
//         process_all_redirections(&(*tree)->right);
//     if (*tree && (*tree)->command)
//     {
//         cmd = (*tree)->command;
// 		// printf("********%s\n", cmd);
// 		// if ((*tree)->redirections)
// 		// 	printf("+++++++++%s\n", (*tree)->redirections);
//         redir_out = ft_strchr(cmd, '>');
//         redir_in = ft_strchr(cmd, '<');
// 		// printf("AAAA\n");
//         if (redir_out || redir_in)
//         {
//             if (!redir_out)
//                 first_redir = redir_in;
//             else if (!redir_in)
//                 first_redir = redir_out;
//             else
//             {
//                 if (redir_out < redir_in)
//                     first_redir = redir_out;
//                 else
//                     first_redir = redir_in;
//             }
//             cmd_length = first_redir - cmd;
//             if (cmd_length == 0)
//             {
//                 (*tree)->redirections = ft_strdup(first_redir);
//                 char *after_redir = first_redir;
//                 while (*after_redir && (*after_redir == '>' || *after_redir == '<'))
//                     after_redir++;
//                 while (*after_redir && *after_redir == ' ')
//                     after_redir++;
//                 while (*after_redir && *after_redir != ' ')
//                     after_redir++;
//                 while (*after_redir && *after_redir == ' ')
//                     after_redir++;
//                 if (*after_redir)
//                     command_part = ft_strdup(after_redir);
//                 else
//                     command_part = ft_strdup("");
//                 t_free((*tree)->command, 1527, "parsing.c");
//                 (*tree)->command = command_part;
//             }
//             else
//             {
//                 cmd_prefix = t_malloc(sizeof(char) * (cmd_length + 1), 1562, "parsing.c");
//                 if (cmd_prefix)
//                 {
//                     ft_strncpy(cmd_prefix, cmd, cmd_length);
//                     cmd_prefix[cmd_length] = '\0';
//                     (*tree)->redirections = ft_strdup(first_redir);
//                     t_free((*tree)->command, 1568, "parsing.c");
//                     (*tree)->command = cmd_prefix;
//                 }
//             }
//         }
//     }
// }

// void process_all_redirections(t_tree **tree) {
//     char *cmd;
//     char *redir_out;
//     char *redir_in;
//     char *first_redir;
//     char *cmd_prefix;
//     char *command_part;
//     int cmd_length;
    
//     if (!tree || !*tree)
//         return;
    
//     // Process children first
//     if ((*tree)->left)
//         process_all_redirections(&(*tree)->left);
//     if ((*tree)->right)
//         process_all_redirections(&(*tree)->right);
    
//     // Process current node
//     if (*tree && (*tree)->command) {
//         cmd = (*tree)->command;
//         redir_out = ft_strchr(cmd, '>');
//         redir_in = ft_strchr(cmd, '<');
        
//         if (redir_out || redir_in) {
//             if (!redir_out)
//                 first_redir = redir_in;
//             else if (!redir_in)
//                 first_redir = redir_out;
//             else {
//                 if (redir_out < redir_in)
//                     first_redir = redir_out;
//                 else
//                     first_redir = redir_in;
//             }
            
//             cmd_length = first_redir - cmd;
            
//             // Special handling for commands that begin with redirections
//             if (cmd_length == 0) {
//                 (*tree)->redirections = ft_strdup(first_redir);
//                 char *after_redir = first_redir;
                
//                 // Skip redirection operators
//                 while (*after_redir && (*after_redir == '>' || *after_redir == '<'))
//                     after_redir++;
                
//                 // Skip spaces
//                 while (*after_redir && *after_redir == ' ')
//                     after_redir++;
                
//                 // Find the end of the filename
//                 while (*after_redir && *after_redir != ' ')
//                     after_redir++;
                
//                 // Skip spaces after filename
//                 while (*after_redir && *after_redir == ' ')
//                     after_redir++;
                
//                 // Extract the command part (if any)
//                 if (*after_redir)
//                     command_part = ft_strdup(after_redir);
//                 else
//                     command_part = ft_strdup("");
                
//                 t_free((*tree)->command, __LINE__, "parsing.c");
//                 (*tree)->command = command_part;
//             }
//             // Normal case: command followed by redirection
//             else {
//                 cmd_prefix = t_malloc(sizeof(char) * (cmd_length + 1), __LINE__, "parsing.c");
//                 if (cmd_prefix) {
//                     ft_strncpy(cmd_prefix, cmd, cmd_length);
//                     cmd_prefix[cmd_length] = '\0';
//                     (*tree)->redirections = ft_strdup(first_redir);
//                     t_free((*tree)->command, __LINE__, "parsing.c");
//                     (*tree)->command = cmd_prefix;
//                 }
//             }
//         }
//     }
    
//     // Special handling for compound expressions
//     if (*tree && (*tree)->type && (ft_strcmp((*tree)->type, "PIPE") == 0 || ft_strcmp((*tree)->type, "OPERATION") == 0)) {
//         // For pipe and operation nodes, check if right child contains redirection
//         // If it does, we need to ensure the redirection is correctly processed
//         if ((*tree)->right && (*tree)->right->command) {
//             redir_out = ft_strchr((*tree)->right->command, '>');
//             redir_in = ft_strchr((*tree)->right->command, '<');
            
//             if (redir_out || redir_in) {
//                 // Process the right child's redirections
//                 process_all_redirections(&(*tree)->right);
//             }
//         }
//     }
// }
// void process_all_redirections(t_tree **tree) {
//     char *cmd;
//     char *redir_out;
//     char *redir_in;
//     char *first_redir;
//     char *cmd_prefix;
//     char *command_part;
//     int cmd_length;
    
//     if (!tree || !*tree)
//         return;
    
//     // Process children first
//     if ((*tree)->left)
//         process_all_redirections(&(*tree)->left);
//     if ((*tree)->right)
//         process_all_redirections(&(*tree)->right);
    
//     // Check if this is a top-level OPERATION node
//     if ((*tree)->type && ft_strcmp((*tree)->type, "OPERATION") == 0) {
//         // For the top operation node, check if its right child or left child contains a redirection
//         if ((*tree)->right) {
//             // Process right child's redirections if any
//             if ((*tree)->right->command) {
//                 redir_out = ft_strchr((*tree)->right->command, '>');
//                 redir_in = ft_strchr((*tree)->right->command, '<');
                
//                 // If right child has redirections, handle them
//                 if (redir_out || redir_in) {
//                     cmd = (*tree)->right->command;
                    
//                     if (!redir_out)
//                         first_redir = redir_in;
//                     else if (!redir_in)
//                         first_redir = redir_out;
//                     else {
//                         if (redir_out < redir_in)
//                             first_redir = redir_out;
//                         else
//                             first_redir = redir_in;
//                     }
                    
//                     cmd_length = first_redir - cmd;
                    
//                     // Create a separate field for redirections on the OPERATION node itself
//                     if (!(*tree)->redirections) {
//                         (*tree)->redirections = ft_strdup(first_redir);
                        
//                         // Update the command in the right child to exclude the redirection
//                         if (cmd_length > 0) {
//                             cmd_prefix = t_malloc(sizeof(char) * (cmd_length + 1), __LINE__, "process_redirections.c");
//                             if (cmd_prefix) {
//                                 ft_strncpy(cmd_prefix, cmd, cmd_length);
//                                 cmd_prefix[cmd_length] = '\0';
//                                 t_free((*tree)->right->command, __LINE__, "process_redirections.c");
//                                 (*tree)->right->command = cmd_prefix;
//                             }
//                         } else {
//                             // If command starts with redirection, extract the command part
//                             char *after_redir = first_redir;
//                             while (*after_redir && (*after_redir == '>' || *after_redir == '<'))
//                                 after_redir++;
//                             while (*after_redir && *after_redir == ' ')
//                                 after_redir++;
//                             while (*after_redir && *after_redir != ' ')
//                                 after_redir++;
//                             while (*after_redir && *after_redir == ' ')
//                                 after_redir++;
                            
//                             if (*after_redir)
//                                 command_part = ft_strdup(after_redir);
//                             else
//                                 command_part = ft_strdup("");
                            
//                             t_free((*tree)->right->command, __LINE__, "process_redirections.c");
//                             (*tree)->right->command = command_part;
//                         }
//                     }
//                 }
//             }
//         }
        
//         // Also check the left child for redirections
//         if ((*tree)->left) {
//             // Similar logic for left child...
//             // (This is essentially the same code as above, but for the left child)
//         }
//     }
    
//     // Process normal command nodes with redirections
//     if (*tree && (*tree)->command) {
//         cmd = (*tree)->command;
//         redir_out = ft_strchr(cmd, '>');
//         redir_in = ft_strchr(cmd, '<');
        
//         if (redir_out || redir_in) {
//             if (!redir_out)
//                 first_redir = redir_in;
//             else if (!redir_in)
//                 first_redir = redir_out;
//             else {
//                 if (redir_out < redir_in)
//                     first_redir = redir_out;
//                 else
//                     first_redir = redir_in;
//             }
            
//             cmd_length = first_redir - cmd;
            
//             if (cmd_length == 0) {
//                 (*tree)->redirections = ft_strdup(first_redir);
//                 char *after_redir = first_redir;
                
//                 // Skip redirection operators
//                 while (*after_redir && (*after_redir == '>' || *after_redir == '<'))
//                     after_redir++;
                
//                 // Skip spaces
//                 while (*after_redir && *after_redir == ' ')
//                     after_redir++;
                
//                 // Skip the filename
//                 while (*after_redir && *after_redir != ' ')
//                     after_redir++;
                
//                 // Skip spaces after filename
//                 while (*after_redir && *after_redir == ' ')
//                     after_redir++;
                
//                 if (*after_redir)
//                     command_part = ft_strdup(after_redir);
//                 else
//                     command_part = ft_strdup("");
                
//                 t_free((*tree)->command, __LINE__, "parsing.c");
//                 (*tree)->command = command_part;
//             } else {
//                 cmd_prefix = t_malloc(sizeof(char) * (cmd_length + 1), __LINE__, "parsing.c");
//                 if (cmd_prefix) {
//                     ft_strncpy(cmd_prefix, cmd, cmd_length);
//                     cmd_prefix[cmd_length] = '\0';
//                     (*tree)->redirections = ft_strdup(first_redir);
//                     t_free((*tree)->command, __LINE__, "parsing.c");
//                     (*tree)->command = cmd_prefix;
//                 }
//             }
//         }
//     }
    
//     // Special case: check if the PARENTHASIS node contains redirections
//     if (*tree && (*tree)->type && ft_strcmp((*tree)->type, "PARENTHASIS") == 0 && (*tree)->command) {
//         cmd = (*tree)->command;
//         redir_out = ft_strchr(cmd, '>');
//         redir_in = ft_strchr(cmd, '<');
        
//         if (redir_out || redir_in) {
//             // Handle the redirections for the parenthesized expression
//             // similar to the processing above
//             if (!redir_out)
//                 first_redir = redir_in;
//             else if (!redir_in)
//                 first_redir = redir_out;
//             else {
//                 if (redir_out < redir_in)
//                     first_redir = redir_out;
//                 else
//                     first_redir = redir_in;
//             }
            
//             cmd_length = first_redir - cmd;
            
//             // Store redirection in the node's redirections field
//             (*tree)->redirections = ft_strdup(first_redir);
            
//             // Update the command to exclude the redirection part
//             if (cmd_length > 0) {
//                 cmd_prefix = t_malloc(sizeof(char) * (cmd_length + 1), __LINE__, "parsing.c");
//                 if (cmd_prefix) {
//                     ft_strncpy(cmd_prefix, cmd, cmd_length);
//                     cmd_prefix[cmd_length] = '\0';
//                     t_free((*tree)->command, __LINE__, "parsing.c");
//                     (*tree)->command = cmd_prefix;
//                 }
//             } else {
//                 // Handle case where command starts with redirection
//                 // (This is unlikely for a parenthesized expression)
//                 t_free((*tree)->command, __LINE__, "parsing.c");
//                 (*tree)->command = ft_strdup("");
//             }
//         }
//     }
// }
// void process_all_redirections(t_tree **tree)
// {
//     if (!tree || !*tree)
//         return;
    
//     // Process children first
//     if ((*tree)->left)
//         process_all_redirections(&(*tree)->left);
//     if ((*tree)->right)
//         process_all_redirections(&(*tree)->right);
    
//     // Process redirections in the current node's command
//     if (*tree && (*tree)->command) {
//         char *cmd = (*tree)->command;
//         char *redir_out = ft_strchr(cmd, '>');
//         char *redir_in = ft_strchr(cmd, '<');
        
//         if (redir_out || redir_in) {
//             char *cmd_part = NULL;
//             char *redir_part = NULL;
//             extract_redirections(cmd, &cmd_part, &redir_part);
            
//             if (cmd_part && redir_part) {
//                 t_free((*tree)->command, __LINE__, "parsing.c");
//                 (*tree)->command = cmd_part;
//                 (*tree)->redirections = redir_part;
//             } else {
//                 if (cmd_part)
//                     t_free(cmd_part, __LINE__, "parsing.c");
//                 if (redir_part)
//                     t_free(redir_part, __LINE__, "parsing.c");
//             }
//         }
//     }
    
//     // For OPERATION nodes, DON'T automatically move redirections from children
//     // Only move redirections up if they logically apply to the result of the operation
    
//     // Check if we need to move heredoc redirection from the left child of a PIPE
//     if (*tree && (*tree)->type && ft_strcmp((*tree)->type, "PIPE") == 0) {
//         if ((*tree)->left && (*tree)->left->redirections) {
//             char *redir = (*tree)->left->redirections;
//             if (ft_strstr(redir, "<<") && !ft_strstr(redir, ">>") && !ft_strstr(redir, ">")) {
//                 // This is a heredoc that should be handled at the pipeline level
//                 (*tree)->redirections = ft_strdup(redir);
//                 t_free((*tree)->left->redirections, __LINE__, "parsing.c");
//                 (*tree)->left->redirections = NULL;
//             }
//         }
//     }
// }
// void process_all_redirections(t_tree **tree)
// {
//     if (!tree || !*tree)
//         return;
    
//     // Process children first
//     if ((*tree)->left)
//         process_all_redirections(&(*tree)->left);
//     if ((*tree)->right)
//         process_all_redirections(&(*tree)->right);
    
//     // Process this node's command for redirections
//     if (*tree && (*tree)->command) {
//         char *cmd = (*tree)->command;
//         char *redir_out = ft_strchr(cmd, '>');
//         char *redir_in = ft_strchr(cmd, '<');
        
//         if (redir_out || redir_in) {
//             // Special check for heredoc
//             // int has_heredoc = (ft_strstr(cmd, "<<") != NULL);
            
//             char *cmd_part = NULL;
//             char *redir_part = NULL;
//             extract_redirections(cmd, &cmd_part, &redir_part);
            
//             if (cmd_part && redir_part) {
//                 t_free((*tree)->command, __LINE__, "parsing.c");
//                 (*tree)->command = cmd_part;
//                 (*tree)->redirections = redir_part;
//             } else {
//                 if (cmd_part)
//                     t_free(cmd_part, __LINE__, "parsing.c");
//                 if (redir_part)
//                     t_free(redir_part, __LINE__, "parsing.c");
//             }
//         }
//     }
    
//     // Special handling for PIPE nodes - check for heredoc that should be on left child
//     if (*tree && (*tree)->type && ft_strcmp((*tree)->type, "PIPE") == 0) {
//         if ((*tree)->redirections && ft_strstr((*tree)->redirections, "<<")) {
//             // If pipe has a heredoc redirection, move it to the left child
//             if ((*tree)->left && !(*tree)->left->redirections) {
//                 (*tree)->left->redirections = ft_strdup((*tree)->redirections);
//                 t_free((*tree)->redirections, __LINE__, "parsing.c");
//                 (*tree)->redirections = NULL;
//             }
//         }
//     }
    
//     // Don't automatically propagate redirections from children to operation parents
//     // This helps ensure heredocs remain with their specific commands
// }
void process_all_redirections(t_tree **tree) {
    if (!tree || !*tree)
        return;
        
    if ((*tree)->left)
        process_all_redirections(&(*tree)->left);
    if ((*tree)->right)
        process_all_redirections(&(*tree)->right);
        
    if (*tree && (*tree)->command) {
        char *cmd = (*tree)->command;
        char *redir_out = ft_strchr(cmd, '>');
        char *redir_in = ft_strchr(cmd, '<');
        
        if (redir_out || redir_in) {
            char *cmd_part = NULL;
            char *redir_part = NULL;
            
            extract_redirections(cmd, &cmd_part, &redir_part);
            
            if (cmd_part && redir_part) {
                t_free((*tree)->command, __LINE__, "parsing.c");
                (*tree)->command = cmd_part;
                (*tree)->redirections = redir_part;
            } else {
                if (cmd_part)
                    t_free(cmd_part, __LINE__, "parsing.c");
                if (redir_part)
                    t_free(redir_part, __LINE__, "parsing.c");
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

// void	redirections_opener(t_tree **tree, t_list_fd **head)
// {
// 	int i;
// 	int j;
// 	t_list_fd *tmp;
// 	t_list_fd *tmp2;
// 	t_list_fd *new_node;
// 	char *tmp_char;
// 	int flag;

// 	i = 0;
// 	flag = 0;
// 	if ((*tree)->left)
// 		redirections_opener(&(*tree)->left, head);
// 	if ((*tree)->right)
// 		redirections_opener(&(*tree)->right, head);
// 	if ((*tree)->redirections)
// 	{
// 		if (!*head)
// 		{
// 			while (((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<') && (*tree)->redirections[i] != '\0') 
// 				i++;
// 			if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == 32)
// 				flag = 1;
// 			if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == '>')
// 				flag = 2;
// 			if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == 32)
// 				flag = 3;
// 			if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == '<')
// 				flag = 4;
// 			while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && (*tree)->redirections[i] != '\0')
// 				i++;
// 			while ((*tree)->redirections[i] == 32 && (*tree)->redirections[i] != '\0')
// 				i++;
// 			j = i;
// 			while ((*tree)->redirections[i] != 32 && (*tree)->redirections[i] != '\0')
// 				i++;
// 			*head = t_malloc(sizeof(t_list_fd), 1768, "parsing.c");
// 			// tmp_char = (*head)->name;
// 			(*head)->name = ft_substr_leak((*tree)->redirections, j, i - j, 2055);
// 			// t_free(tmp_char, 1771, "parsing.c");
// 			(*head)->redir = NULL;
// 			(*head)->next = NULL;
// 			(*head)->fd = -1;
// 			(*head)->command = ft_strdup((*tree)->command);
// 			tmp_char = (*tree)->redirections;
// 			(*tree)->redirections = ft_substr_leak((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i, 2062);
// 			t_free(tmp_char, 1778, "parsing.c");
// 			tmp_char = (*tree)->redirections;
// 			(*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", 1779, "list_init");
// 			t_free(tmp_char, 1781, "parsing.c");
// 			if (flag == 1 && !check_empty((*head)->name))
// 			{
// 				(*head)->fd = open((*head)->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 				if ((*head)->fd == -1)
// 				{
// 					write(2, "invalid file\n", 13);
// 					new_node = NULL;
// 					if (!*head)
// 						*head = new_node;
// 					free_list_fd(head);
// 					exit (1);
// 				}
// 				(*head)->redir = ft_strdup(">");
// 			}
// 			if (flag == 2 && !check_empty((*head)->name))
// 			{
// 				(*head)->fd = open((*head)->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
// 				if ((*head)->fd == -1)
// 				{
// 					write(2, "invalid file\n", 13);
// 					new_node = NULL;
// 					if (!*head)
// 						*head = new_node;
// 					free_list_fd(head);
// 					exit (1);
// 				}
// 				(*head)->redir = ft_strdup(">>");
// 			}
// 			if (flag == 3)
// 			{
// 				(*head)->fd = open((*head)->name, O_RDONLY, 0644);
// 				if ((*head)->fd == -1)
// 				{
// 					write(2, "invalid file\n", 13);
// 					new_node = NULL;
// 					if (!*head)
// 						*head = new_node;
// 					free_list_fd(head);
// 					exit (1);
// 				}
// 				(*head)->redir = ft_strdup("<");
// 			}
// 			if (flag == 4)
// 			{
// 				(*head)->fd = open((*head)->name, O_RDONLY, 0644);
// 				if ((*head)->fd == -1)
// 				{
// 					write(2, "invalid file\n", 13);
// 					new_node = NULL;
// 					if (!*head)
// 						*head = new_node;
// 					free_list_fd(head);
// 					exit (1);
// 				}
// 				(*head)->redir = ft_strdup("<<");
// 			}
// 			(*head)->next = NULL;
// 			tmp = *head;
// 			while ((*tree)->redirections && !check_empty((*tree)->redirections))
// 			{
// 				i = 0;
// 				while ((*tree)->redirections != NULL && (*tree)->redirections[i] && ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
// 					i++;
// 				if (!(*tree)->redirections[i])
// 					break;
// 				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == 32)
// 					flag = 1;
// 				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == '>')
// 					flag = 2;
// 				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == 32)
// 					flag = 3;
// 				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == '<')
// 					flag = 4;
// 				while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && (*tree)->redirections[i] != '\0')
// 					i++;
// 				while ((*tree)->redirections[i] == 32 && (*tree)->redirections[i] != '\0')
// 					i++;
// 				j = i;
// 				while ((*tree)->redirections[i] != 32 && (*tree)->redirections[i] != '\0')
// 					i++;
// 				new_node = t_malloc(sizeof(t_list_fd), 1832, "parsing.c");
// 				tmp_char = new_node->name;
// 				new_node->name = ft_substr_leak((*tree)->redirections, j, i - j, 2149);
// 				t_free(tmp_char, 1865, "parsing.c");
// 				new_node->fd = -1;
// 				new_node->redir = NULL;
// 				new_node->next = NULL;
// 				new_node->command = ft_strdup((*tree)->command);
// 				tmp_char = (*tree)->redirections;
// 				(*tree)->redirections = ft_substr_leak((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i, 2156);
// 				t_free(tmp_char, 1872, "parsing.c");
// 				tmp_char = (*tree)->redirections;
// 				(*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", 1873, "list_init");
// 				t_free(tmp_char, 1875, "parsing.c");
// 				if (flag == 1 && !check_empty(new_node->name))
// 				{
// 					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 1886, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 1888, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 1890, "parsing.c");
// 						t_free(new_node, 1891, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup(">");
// 				}
// 				if (flag == 2 && !check_empty(new_node->name))
// 				{
// 					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 1904, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 1906, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 1908, "parsing.c");
// 						t_free(new_node, 1909, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup(">>");
// 				}
// 				if (flag == 3)
// 				{
// 					new_node->fd = open(new_node->name, O_RDONLY,  0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 1922, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 1924, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 1926, "parsing.c");
// 						t_free(new_node, 1927, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup("<");
// 				}
// 				if (flag == 4)
// 				{
// 					new_node->fd = open(new_node->name, O_RDONLY, 0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 1940, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 1942, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 1944, "parsing.c");
// 						t_free(new_node, 1945, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup("<<");
// 				}
// 				new_node->next = NULL;
// 				tmp->next = new_node;
// 				tmp = new_node;
// 				if (check_empty((*tree)->redirections) == 1)
// 					break ;
// 			}
// 		}
// 		else if (head && *head)
// 		{
// 			tmp2 = *head;
// 			while (tmp2->next)
// 				tmp2 = tmp2->next;
// 			tmp = tmp2;
// 			while ((*tree)->redirections && !check_empty((*tree)->redirections))
// 			{
// 				i = 0;
// 				while ((*tree)->redirections != NULL && (*tree)->redirections[i] && ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
// 					i++;
// 				if (!(*tree)->redirections[i])
// 					break;
// 				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == 32)
// 					flag = 1;
// 				if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i + 1] == '>')
// 					flag = 2;
// 				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == 32)
// 					flag = 3;
// 				if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i + 1] == '<')
// 					flag = 4;
// 				while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && (*tree)->redirections[i] != '\0')
// 					i++;
// 				while ((*tree)->redirections[i] == 32 && (*tree)->redirections[i] != '\0')
// 					i++;
// 				j = i;
// 				while ((*tree)->redirections[i] != 32 && (*tree)->redirections[i] != '\0')
// 					i++;
// 				new_node = t_malloc(sizeof(t_list_fd), 1956, "parsing.c");
// 				// tmp_char = new_node->name;
// 				new_node->name = ft_substr_leak((*tree)->redirections, j, i - j, 2270);
// 				// t_free(tmp_char, 1989, "parsing.c");
// 				new_node->fd = -1;
// 				new_node->redir = NULL;
// 				new_node->next = NULL;
// 				new_node->command = ft_strdup((*tree)->command);
// 				tmp_char = (*tree)->redirections;
// 				(*tree)->redirections = ft_substr_leak((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i, 2277);
// 				t_free(tmp_char, 1996, "parsing.c");
// 				tmp_char = (*tree)->redirections;
// 				(*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", 1994, "list_init");
// 				t_free(tmp_char, 1999, "parsing.c");
// 				if (flag == 1 && !check_empty(new_node->name))
// 				{
// 					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 2007, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 2009, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 2011, "parsing.c");
// 						t_free(new_node, 2012, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup(">");
// 				}
// 				if (flag == 2 && !check_empty(new_node->name))
// 				{
// 					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 2025, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 2027, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 2029, "parsing.c");
// 						t_free(new_node, 2030, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup(">>");
// 				}
// 				if (flag == 3)
// 				{
// 					new_node->fd = open(new_node->name, O_RDONLY,  0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 2043, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 2045, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 2047, "parsing.c");
// 						t_free(new_node, 2048, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup("<");
// 				}
// 				if (flag == 4)
// 				{
// 					new_node->fd = open(new_node->name, O_RDONLY, 0644);
// 					if (new_node->fd == -1)
// 					{
// 						write(2, "invalid file\n", 13);
// 						if (new_node->name)
// 							t_free(new_node->name, 2061, "parsing.c");
// 						if (new_node->command)
// 							t_free(new_node->command, 2063, "parsing.c");
// 						if (new_node->redir)
// 							t_free(new_node->redir, 2065, "parsing.c");
// 						t_free(new_node, 2066, "parsing.c");
// 						free_list_fd(head);
// 						exit (1);
// 					}
// 					new_node->redir = ft_strdup("<<");
// 				}
// 				new_node->next = NULL;
// 				tmp->next = new_node;
// 				tmp = new_node;
// 				if (check_empty((*tree)->redirections) == 1)
// 					break ;
// 			}
// 		}
// 	}
// }
void redirections_opener(t_tree **tree, t_list_fd **head)
{
    int i = 0;
    int j = 0;
    t_list_fd *tmp;
    t_list_fd *tmp2;
    t_list_fd *new_node;
    char *tmp_char;
    int flag = 0;
    
    // Process children first
    if ((*tree)->left)
        redirections_opener(&(*tree)->left, head);
    if ((*tree)->right)
        redirections_opener(&(*tree)->right, head);
    
    // Process redirections at this node
    if ((*tree)->redirections) {
        if (!*head) {
            // This is the first redirection node
            
            // Find first redirection operator
            while (((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<') && 
                   (*tree)->redirections[i] != '\0')
                i++;
            
            // Determine redirection type
            if ((*tree)->redirections[i] == '>' && ((*tree)->redirections[i+1] == ' ' || 
                (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                flag = 1;  // > (output redirection)
            if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i+1] == '>')
                flag = 2;  // >> (append redirection)
            if ((*tree)->redirections[i] == '<' && ((*tree)->redirections[i+1] == ' ' || 
                (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                flag = 3;  // < (input redirection)
            if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i+1] == '<')
                flag = 4;  // << (here document)
            
            // Skip redirection operators
            while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && 
                   (*tree)->redirections[i] != '\0')
                i++;
            
            // Skip whitespace
            while ((*tree)->redirections[i] == ' ' && (*tree)->redirections[i] != '\0')
                i++;
            
            // Extract the filename
            j = i;
            while ((*tree)->redirections[i] != ' ' && (*tree)->redirections[i] != '\0')
                i++;
            
            // Create new t_list_fd node
            *head = t_malloc(sizeof(t_list_fd), __LINE__, "parsing.c");
            if (!*head) return;
            
            // Extract filename
            (*head)->name = ft_substr_leak((*tree)->redirections, j, i-j, __LINE__);
            (*head)->redir = NULL;
            (*head)->next = NULL;
            (*head)->fd = -1;
            
            // Determine which command to use with this redirection
            if (ft_strcmp((*tree)->type, "OPERATION") == 0) {
                // For operation nodes, use the command from the right child
                if ((*tree)->right && (*tree)->right->command)
                    (*head)->command = ft_strdup((*tree)->right->command);
                else if ((*tree)->left && (*tree)->left->command)
                    (*head)->command = ft_strdup((*tree)->left->command);
                else
                    (*head)->command = ft_strdup("");
            } else {
                // For other nodes, use this node's command
                (*head)->command = ft_strdup((*tree)->command);
            }
            
            // Extract remaining redirection text after this one
            tmp_char = (*tree)->redirections;
            (*tree)->redirections = ft_substr_leak((*tree)->redirections, i, 
                                                 ft_strlen((*tree)->redirections)-i, __LINE__);
            t_free(tmp_char, __LINE__, "parsing.c");
            
            // Trim whitespace from remaining redirections
            tmp_char = (*tree)->redirections;
            (*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", __LINE__, "parsing.c");
            t_free(tmp_char, __LINE__, "parsing.c");
            
            // Handle different redirection types
            if (flag == 1 && !check_empty((*head)->name)) {
                // Output redirection (>)
                (*head)->fd = open((*head)->name, O_CREAT|O_WRONLY|O_TRUNC, 0644);
                if ((*head)->fd == -1) {
                    write(2, "invalid file\n", 13);
                    new_node = NULL;
                    if (!*head) *head = new_node;
                    free_list_fd(head);
                    exit(1);
                }
                (*head)->redir = ft_strdup(">");
            }
            
            if (flag == 2 && !check_empty((*head)->name)) {
                // Append redirection (>>)
                (*head)->fd = open((*head)->name, O_CREAT|O_WRONLY|O_APPEND, 0644);
                if ((*head)->fd == -1) {
                    write(2, "invalid file\n", 13);
                    new_node = NULL;
                    if (!*head) *head = new_node;
                    free_list_fd(head);
                    exit(1);
                }
                (*head)->redir = ft_strdup(">>");
            }
            
            if (flag == 3) {
                // Input redirection (<)
                (*head)->fd = open((*head)->name, O_RDONLY, 0644);
                if ((*head)->fd == -1) {
                    write(2, "invalid file\n", 13);
                    new_node = NULL;
                    if (!*head) *head = new_node;
                    free_list_fd(head);
                    exit(1);
                }
                (*head)->redir = ft_strdup("<");
            }
            
            if (flag == 4) {
                // Here document (<<)
                (*head)->fd = open((*head)->name, O_RDONLY, 0644);
                if ((*head)->fd == -1) {
                    write(2, "invalid file\n", 13);
                    new_node = NULL;
                    if (!*head) *head = new_node;
                    free_list_fd(head);
                    exit(1);
                }
                (*head)->redir = ft_strdup("<<");
            }
            
            (*head)->next = NULL;
            tmp = *head;
            
            // Process any additional redirections
            while ((*tree)->redirections && !check_empty((*tree)->redirections)) {
                i = 0;
                while ((*tree)->redirections != NULL && (*tree)->redirections[i] && 
                       ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
                    i++;
                
                if (!(*tree)->redirections[i])
                    break;
                
                // Determine redirection type
                if ((*tree)->redirections[i] == '>' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 1;  // >
                if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i+1] == '>')
                    flag = 2;  // >>
                if ((*tree)->redirections[i] == '<' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 3;  // <
                if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i+1] == '<')
                    flag = 4;  // <<
                
                // Skip redirection operators
                while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && 
                       (*tree)->redirections[i] != '\0')
                    i++;
                
                // Skip whitespace
                while ((*tree)->redirections[i] == ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                
                // Extract filename
                j = i;
                while ((*tree)->redirections[i] != ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                
                // Create new node
                new_node = t_malloc(sizeof(t_list_fd), __LINE__, "parsing.c");
                if (!new_node)
                    break;
                
                // Extract filename
                new_node->name = ft_substr_leak((*tree)->redirections, j, i-j, __LINE__);
                new_node->fd = -1;
                new_node->redir = NULL;
                new_node->next = NULL;
                
                // Determine command
                if (ft_strcmp((*tree)->type, "OPERATION") == 0) {
                    // For operation nodes, use right child's command
                    if ((*tree)->right && (*tree)->right->command)
                        new_node->command = ft_strdup((*tree)->right->command);
                    else if ((*tree)->left && (*tree)->left->command)
                        new_node->command = ft_strdup((*tree)->left->command);
                    else
                        new_node->command = ft_strdup("");
                } else {
                    // For other nodes, use this node's command
                    new_node->command = ft_strdup((*tree)->command);
                }
                
                // Extract remaining redirections
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_substr_leak((*tree)->redirections, i, 
                                                     ft_strlen((*tree)->redirections)-i, __LINE__);
                t_free(tmp_char, __LINE__, "parsing.c");
                
                // Trim whitespace
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", __LINE__, "parsing.c");
                t_free(tmp_char, __LINE__, "parsing.c");
                
                // Handle different redirection types
                if (flag == 1 && !check_empty(new_node->name)) {
                    // Output redirection (>)
                    new_node->fd = open(new_node->name, O_CREAT|O_WRONLY|O_TRUNC, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup(">");
                }
                
                if (flag == 2 && !check_empty(new_node->name)) {
                    // Append redirection (>>)
                    new_node->fd = open(new_node->name, O_CREAT|O_WRONLY|O_APPEND, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup(">>");
                }
                
                if (flag == 3) {
                    // Input redirection (<)
                    new_node->fd = open(new_node->name, O_RDONLY, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup("<");
                }
                
                if (flag == 4) {
                    // Here document (<<)
                    new_node->fd = open(new_node->name, O_RDONLY, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup("<<");
                }
                
                new_node->next = NULL;
                tmp->next = new_node;
                tmp = new_node;
                
                // Break if no more redirections
                if (check_empty((*tree)->redirections) == 1)
                    break;
            }
        } else {
            // Head already exists, add additional redirections
            tmp2 = *head;
            while (tmp2->next)
                tmp2 = tmp2->next;
            tmp = tmp2;
            
            // Process each redirection
            while ((*tree)->redirections && !check_empty((*tree)->redirections)) {
                i = 0;
                while ((*tree)->redirections != NULL && (*tree)->redirections[i] && 
                       ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
                    i++;
                
                if (!(*tree)->redirections[i])
                    break;
                
                // Determine redirection type
                if ((*tree)->redirections[i] == '>' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 1;  // >
                if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i+1] == '>')
                    flag = 2;  // >>
                if ((*tree)->redirections[i] == '<' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 3;  // <
                if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i+1] == '<')
                    flag = 4;  // <<
                
                // Skip redirection operators
                while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && 
                       (*tree)->redirections[i] != '\0')
                    i++;
                
                // Skip whitespace
                while ((*tree)->redirections[i] == ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                
                // Extract filename
                j = i;
                while ((*tree)->redirections[i] != ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                
                // Create new node
                new_node = t_malloc(sizeof(t_list_fd), __LINE__, "parsing.c");
                if (!new_node)
                    break;
                
                // Extract filename
                new_node->name = ft_substr_leak((*tree)->redirections, j, i-j, __LINE__);
                new_node->fd = -1;
                new_node->redir = NULL;
                new_node->next = NULL;
                
                // Determine command
                if (ft_strcmp((*tree)->type, "OPERATION") == 0) {
                    // For operation nodes, use right child's command
                    if ((*tree)->right && (*tree)->right->command)
                        new_node->command = ft_strdup((*tree)->right->command);
                    else if ((*tree)->left && (*tree)->left->command)
                        new_node->command = ft_strdup((*tree)->left->command);
                    else
                        new_node->command = ft_strdup("");
                } else {
                    // For other nodes, use this node's command
                    new_node->command = ft_strdup((*tree)->command);
                }
                
                // Extract remaining redirections
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_substr_leak((*tree)->redirections, i, 
                                                     ft_strlen((*tree)->redirections)-i, __LINE__);
                t_free(tmp_char, __LINE__, "parsing.c");
                
                // Trim whitespace
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", __LINE__, "parsing.c");
                t_free(tmp_char, __LINE__, "parsing.c");
                
                // Handle different redirection types
                if (flag == 1 && !check_empty(new_node->name)) {
                    // Output redirection (>)
                    new_node->fd = open(new_node->name, O_CREAT|O_WRONLY|O_TRUNC, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup(">");
                }
                
                if (flag == 2 && !check_empty(new_node->name)) {
                    // Append redirection (>>)
                    new_node->fd = open(new_node->name, O_CREAT|O_WRONLY|O_APPEND, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup(">>");
                }
                
                if (flag == 3) {
                    // Input redirection (<)
                    new_node->fd = open(new_node->name, O_RDONLY, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup("<");
                }
                
                if (flag == 4) {
                    // Here document (<<)
                    new_node->fd = open(new_node->name, O_RDONLY, 0644);
                    if (new_node->fd == -1) {
                        write(2, "invalid file\n", 13);
                        if (new_node->name)
                            t_free(new_node->name, __LINE__, "parsing.c");
                        if (new_node->command)
                            t_free(new_node->command, __LINE__, "parsing.c");
                        if (new_node->redir)
                            t_free(new_node->redir, __LINE__, "parsing.c");
                        t_free(new_node, __LINE__, "parsing.c");
                        free_list_fd(head);
                        exit(1);
                    }
                    new_node->redir = ft_strdup("<<");
                }
                
                new_node->next = NULL;
                tmp->next = new_node;
                tmp = new_node;
                
                // Break if no more redirections
                if (check_empty((*tree)->redirections) == 1)
                    break;
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
                tmp->data = ft_strtrim_leak(tmp->data, "=", 2146, "list_init");
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
							tmp_word = ft_strtrim_leak(tmp_word, "=", 2252, "list_init");
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
		head = list_init_leak(str, 2952, "command_arr_fill");
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

// void pipe_in_command_check(t_tree **tree)
// {
// 	if (!tree || !*tree)
// 		return ;
// 	if((*tree)->left)
// 		pipe_in_command
// }

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
	tree = NULL;
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
		str = ft_strtrim_leak(str, " ", 2509, "main");
		t_free(tmp_str, 2514, "parsing.c");
		if (check_quotes(str) == 1)
			return (1);
		head = list_init_leak(str, 3072, "main");
		lexer(&head);
		join_redirections_with_commands(&head);
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
		// printf("\n");
		// print_tree_visual(tree, 1, 1);
		process_nested_parentheses(&tree);
		// printf("\n");
		// print_tree_visual(tree, 1, 1);
		// printf("\n");
		// printf("\n");
		// print_tree_visual(tree, 1, 1);
		// printf("\n");
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
		// if (head)
		// 	free_list(&head);
		// t_free(str, 2567, "parsing.c");
	}
}//ls > file | sl > fiel && ls >fule | sl > eluf
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt