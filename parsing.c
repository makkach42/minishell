/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/18 13:47:22 by makkach          ###   ########.fr       */
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
char *side_maker(t_list **head, int number, int j) {
    t_list *tmp;
    char *tmp2;
    char *tmp_char;
    int i;
    tmp = *head;
    i = 0;
    tmp2 = NULL;
    while (i < number - j) {
        tmp_char = tmp2;
        tmp2 = ft_strjoin_leak(tmp2, tmp->data, 1441);
        t_free(tmp_char, 1442, "parsing.c");
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
void join_redirections_with_commands(t_list **head)
{
    t_list *current;
    t_list *next;
    t_list *target;
    char *combined;
    char *temp;
    if (!head || !(*head))
        return;
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
    tmp = *head;
    total_nodes = 0;
    while (tmp) {
        last = tmp;
        tmp = tmp->next;
        total_nodes++;
    }
    tmp = last;
    i = total_nodes;
    while (tmp) {
        if (ft_strcmp(tmp->token, "OPERATION") == 0) {
            *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
            if (!(*tree))
                return;
            (*tree)->type = "OPERATION";
            (*tree)->command = NULL;
            (*tree)->command_arr = NULL;
            (*tree)->redirections = NULL;
            command = NULL;
            if (tmp->next) {
                command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
                if (!command) {
                    t_free(*tree, __LINE__, "parsing.c");
                    *tree = NULL;
                    return;
                }
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
            if (tmp->prev) {
                prev_part = *head;
                tmp->prev->next = NULL;
                if (tmp->data)
                    t_free(tmp->data, __LINE__, "parsing.c");
                t_free(tmp, __LINE__, "parsing.c");
                if (prev_part) {
                    tree_maker(&prev_part, &(*tree)->left);
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
    tmp = last;
    i = total_nodes;
    while (tmp) {
        if (ft_strcmp(tmp->token, "PIPE") == 0) {
            *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
            if (!(*tree))
                return;
            (*tree)->type = "PIPE";
            (*tree)->command = NULL;
            (*tree)->command_arr = NULL;
            (*tree)->redirections = NULL;
            command = NULL;
            if (tmp->next) {
                command = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
                if (!command) {
                    t_free(*tree, __LINE__, "parsing.c");
                    *tree = NULL;
                    return;
                }
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
            if (tmp->prev) {
                prev_part = *head;
                tmp->prev->next = NULL;
                if (tmp->data)
                    t_free(tmp->data, __LINE__, "parsing.c");
                t_free(tmp, __LINE__, "parsing.c");
                if (prev_part) {
                    tree_maker(&prev_part, &(*tree)->left);
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
    if (*head) {
        *tree = t_malloc(sizeof(t_tree), __LINE__, "parsing.c");
        if (!(*tree))
            return;
        if ((*head)->token && (*head)->token[0] != '\0') {
            (*tree)->type = (*head)->token;
        } else {
            (*tree)->type = "WORD";
        }
		(*tree)->command = side_maker(head, total_nodes - i, 0);
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->command_arr = NULL;
        (*tree)->redirections = NULL;
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
char *extract_command_with_redirects(t_list **head, t_list **pipe_pos) {
    t_list *current = *head;
    char *command_str = NULL;
    char *temp_str = NULL;
    *pipe_pos = NULL;
    while (current) {
        if ((current->token && ft_strcmp(current->token, "PIPE") == 0) || 
            (current->token && ft_strcmp(current->token, "OPERATION") == 0)) {
            *pipe_pos = current;
            break;
        }
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
    if (!command_str) {
        command_str = ft_strdup("");
    }
    return command_str;
}
t_tree *build_pipe_tree(t_list **head) {
    t_list *pipe_pos = NULL;
    t_tree *root = NULL;
    t_tree *command_node = NULL;
    char *left_cmd = NULL;
    if (!head || !(*head))
        return NULL;
    left_cmd = extract_command_with_redirects(head, &pipe_pos);
    if (!pipe_pos) {
        root = create_tree_node(left_cmd, "COMMAND");
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
        t_list *current = *head;
        t_list *next = NULL;
        while (current && current != pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        if (current == pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        if (current) {
            t_list *right_head = current;
            root->right = build_pipe_tree(&right_head);
        }
        *head = NULL;  
    } else if (ft_strcmp(pipe_pos->token, "OPERATION") == 0) {
        root = create_tree_node(NULL, "OPERATION");
        t_list *left_head = *head;
        t_list *tmp_list = NULL;
        while (left_head && left_head != pipe_pos) {
            t_list *new_node = t_malloc(sizeof(t_list), __LINE__, "parsing.c");
            new_node->data = ft_strdup(left_head->data);
            new_node->token = left_head->token;
            new_node->next = NULL;
            new_node->prev = NULL;
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
        root->left = build_pipe_tree(&tmp_list);
        t_list *current = *head;
        t_list *next = NULL;
        while (current && current != pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        if (current == pipe_pos) {
            next = current->next;
            if (current->data)
                t_free(current->data, __LINE__, "parsing.c");
            t_free(current, __LINE__, "parsing.c");
            current = next;
        }
        if (current) {
            t_list *right_head = current;
            root->right = build_pipe_tree(&right_head);
        }
        *head = NULL;  
    }
    return root;
}
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
    lexer(&cmd_list);
    *command_tree = build_pipe_tree(&cmd_list);
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
}
void free_tree(t_tree *tree) {
    int i;
    if (!tree)
        return;
    if (tree->left)
        free_tree(tree->left);
    if (tree->right)
        free_tree(tree->right);
    if (tree->command) {
        t_free(tree->command, __LINE__, "parsing.c");
        tree->command = NULL;  
    }
    if (tree->redirections) {
        t_free(tree->redirections, __LINE__, "parsing.c");
        tree->redirections = NULL;
    }
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
void process_pipe_trees(t_tree *tree) {
    if (!tree)
        return;
    if (tree->command && tree->type && 
        ft_strcmp(tree->type, "PARENTHASIS") != 0 && 
        (ft_strchr(tree->command, '|') || ft_strchr(tree->command, '&'))) {
        t_tree *cmd_tree = NULL;
        process_command_with_pipes(tree->command, &cmd_tree);
        if (cmd_tree) {
            t_free(tree->command, __LINE__, "parsing.c");
            tree->command = NULL;
            tree->type = cmd_tree->type;
            if (tree->left)
                free_tree(tree->left);
            if (tree->right)
                free_tree(tree->right);
            tree->left = cmd_tree->left;
            tree->right = cmd_tree->right;
            cmd_tree->left = NULL;
            cmd_tree->right = NULL;
            t_free(cmd_tree, __LINE__, "parsing.c");
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
void redirections_opener(t_tree **tree, t_list_fd **head)
{
    int i = 0;
    int j = 0;
    t_list_fd *tmp;
    t_list_fd *tmp2;
    t_list_fd *new_node;
    char *tmp_char;
    int flag = 0;
    if ((*tree)->left)
        redirections_opener(&(*tree)->left, head);
    if ((*tree)->right)
        redirections_opener(&(*tree)->right, head);
    if ((*tree)->redirections) {
        if (!*head) {
            while (((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<') && 
                   (*tree)->redirections[i] != '\0')
                i++;
            if ((*tree)->redirections[i] == '>' && ((*tree)->redirections[i+1] == ' ' || 
                (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                flag = 1;  
            if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i+1] == '>')
                flag = 2;  
            if ((*tree)->redirections[i] == '<' && ((*tree)->redirections[i+1] == ' ' || 
                (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                flag = 3;  
            if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i+1] == '<')
                flag = 4;  
            while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && 
                   (*tree)->redirections[i] != '\0')
                i++;
            while ((*tree)->redirections[i] == ' ' && (*tree)->redirections[i] != '\0')
                i++;
            j = i;
            while ((*tree)->redirections[i] != ' ' && (*tree)->redirections[i] != '\0')
                i++;
            *head = t_malloc(sizeof(t_list_fd), __LINE__, "parsing.c");
            if (!*head) return;
            (*head)->name = ft_substr_leak((*tree)->redirections, j, i-j, __LINE__);
            (*head)->redir = NULL;
            (*head)->next = NULL;
            (*head)->fd = -1;
            if (ft_strcmp((*tree)->type, "OPERATION") == 0) {
                if ((*tree)->right && (*tree)->right->command)
                    (*head)->command = ft_strdup((*tree)->right->command);
                else if ((*tree)->left && (*tree)->left->command)
                    (*head)->command = ft_strdup((*tree)->left->command);
                else
                    (*head)->command = ft_strdup("");
            } else {
                (*head)->command = ft_strdup((*tree)->command);
            }
            tmp_char = (*tree)->redirections;
            (*tree)->redirections = ft_substr_leak((*tree)->redirections, i, 
                                                 ft_strlen((*tree)->redirections)-i, __LINE__);
            t_free(tmp_char, __LINE__, "parsing.c");
            tmp_char = (*tree)->redirections;
            (*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", __LINE__, "parsing.c");
            t_free(tmp_char, __LINE__, "parsing.c");
            if (flag == 1 && !check_empty((*head)->name)) {
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
            while ((*tree)->redirections && !check_empty((*tree)->redirections)) {
                i = 0;
                while ((*tree)->redirections != NULL && (*tree)->redirections[i] && 
                       ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
                    i++;
                if (!(*tree)->redirections[i])
                    break;
                if ((*tree)->redirections[i] == '>' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 1;  
                if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i+1] == '>')
                    flag = 2;  
                if ((*tree)->redirections[i] == '<' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 3;  
                if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i+1] == '<')
                    flag = 4;  
                while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && 
                       (*tree)->redirections[i] != '\0')
                    i++;
                while ((*tree)->redirections[i] == ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                j = i;
                while ((*tree)->redirections[i] != ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                new_node = t_malloc(sizeof(t_list_fd), __LINE__, "parsing.c");
                if (!new_node)
                    break;
                new_node->name = ft_substr_leak((*tree)->redirections, j, i-j, __LINE__);
                new_node->fd = -1;
                new_node->redir = NULL;
                new_node->next = NULL;
                if (ft_strcmp((*tree)->type, "OPERATION") == 0) {
                    if ((*tree)->right && (*tree)->right->command)
                        new_node->command = ft_strdup((*tree)->right->command);
                    else if ((*tree)->left && (*tree)->left->command)
                        new_node->command = ft_strdup((*tree)->left->command);
                    else
                        new_node->command = ft_strdup("");
                } else {
                    new_node->command = ft_strdup((*tree)->command);
                }
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_substr_leak((*tree)->redirections, i, 
                                                     ft_strlen((*tree)->redirections)-i, __LINE__);
                t_free(tmp_char, __LINE__, "parsing.c");
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", __LINE__, "parsing.c");
                t_free(tmp_char, __LINE__, "parsing.c");
                if (flag == 1 && !check_empty(new_node->name)) {
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
                if (check_empty((*tree)->redirections) == 1)
                    break;
            }
        } else {
            tmp2 = *head;
            while (tmp2->next)
                tmp2 = tmp2->next;
            tmp = tmp2;
            while ((*tree)->redirections && !check_empty((*tree)->redirections)) {
                i = 0;
                while ((*tree)->redirections != NULL && (*tree)->redirections[i] && 
                       ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
                    i++;
                if (!(*tree)->redirections[i])
                    break;
                if ((*tree)->redirections[i] == '>' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 1;  
                if ((*tree)->redirections[i] == '>' && (*tree)->redirections[i+1] == '>')
                    flag = 2;  
                if ((*tree)->redirections[i] == '<' && ((*tree)->redirections[i+1] == ' ' || 
                    (*tree)->redirections[i+1] == '\0' || (*tree)->redirections[i+1] == '\t'))
                    flag = 3;  
                if ((*tree)->redirections[i] == '<' && (*tree)->redirections[i+1] == '<')
                    flag = 4;  
                while (((*tree)->redirections[i] == '>' || (*tree)->redirections[i] == '<') && 
                       (*tree)->redirections[i] != '\0')
                    i++;
                while ((*tree)->redirections[i] == ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                j = i;
                while ((*tree)->redirections[i] != ' ' && (*tree)->redirections[i] != '\0')
                    i++;
                new_node = t_malloc(sizeof(t_list_fd), __LINE__, "parsing.c");
                if (!new_node)
                    break;
                new_node->name = ft_substr_leak((*tree)->redirections, j, i-j, __LINE__);
                new_node->fd = -1;
                new_node->redir = NULL;
                new_node->next = NULL;
                if (ft_strcmp((*tree)->type, "OPERATION") == 0) {
                    if ((*tree)->right && (*tree)->right->command)
                        new_node->command = ft_strdup((*tree)->right->command);
                    else if ((*tree)->left && (*tree)->left->command)
                        new_node->command = ft_strdup((*tree)->left->command);
                    else
                        new_node->command = ft_strdup("");
                } else {
                    new_node->command = ft_strdup((*tree)->command);
                }
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_substr_leak((*tree)->redirections, i, 
                                                     ft_strlen((*tree)->redirections)-i, __LINE__);
                t_free(tmp_char, __LINE__, "parsing.c");
                tmp_char = (*tree)->redirections;
                (*tree)->redirections = ft_strtrim_leak((*tree)->redirections, " ", __LINE__, "parsing.c");
                t_free(tmp_char, __LINE__, "parsing.c");
                if (flag == 1 && !check_empty(new_node->name)) {
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
int main(int argc, char **argv, char **argev)
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
		if (variable_search(&head))
            variable_expantion(&head, argev);
		variable_in_word(&head, argev);
		tmp = head;
		syntax_error(&head);
		tree_maker(&head, &tree);
		process_pipe_trees(tree);
		process_nested_parentheses(&tree);
		process_all_redirections(&tree);
		command_arr_fill(&tree);
		quote_remove_two(&tree);
		print_tree_visual(tree, 1, 1);
		syntax_error_two(&tree);
		redirections_opener(&tree, &head_fd);
		tmp_fd = head_fd;
		free_env(&env);
		if (tree)
			free_tree(tree);
		if (head_fd)
			free_list_fd(&head_fd);
	}
}//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt