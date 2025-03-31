/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/03/31 13:59:23 by makkach          ###   ########.fr       */
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
		return (ft_substr(str, 0, i));
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
		return (ft_substr(str, 0, i));
	}
	else
	{
		while (str[++i] == 32){}
		while (str[i] != 32 && !is_operator(str[i]) && str[i] != '\0')
			i++;
		word = ft_substr(str, 0, i);
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
char *parenthesis_remover(char *str, char *word)
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
char *extract_variable(char *str)
{
	int i;

	i = 0;
	while (str[i] != 32 && str[i] != '\0')
		i++;
	return (ft_substr(str, 0, i + 1));
}
// int	check_if_still()
// {

// }

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
		// printf("%s\n", str);
		// printf("%d\n", open_par);
		// printf("%d\n", closed_par);
	// if (open_par == closed_par && (str[i + 1] && (str[i + 1] == 32 || str[i + 1] == '\0' || str[i + 1] == ')')) && (open_par == closed_par || (open_par > closed_par && str[i + 1] == ')')))
	// {
	// 	printf("AAA\n");
	// 	while (open_par > closed_par)
	// 	{
	// 		if (str[i] == ')')
	// 			closed_par++;
	// 		i++;
	// 	}
	// 	return (ft_substr(str, 0, i + 1));
	// }
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
		return (ft_substr(str, 0, i));
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
		word = ft_substr(str, 0, i + 1);
	}
	return (word);
}

t_list *list_init(char *str)
{
	char *word;
	char *tmp_char;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if (*str == '\"')
	{
		word = str_extractor(str);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = str_remover(str, word);
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (is_operator(*str))
	{
		word = extract_operator(str);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = remove_operator(str, word);
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (*str == '$')
	{
		word = extract_variable(str);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = first_word_remover(str, word);
		free(tmp_char);
		tmp_char = str;
		str= ft_strtrim(str, " ");
		free(tmp_char);
	}
	else if (*str == '(')
	{
		word = extract_parenthesis(str);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = parenthesis_remover(str, word);
		free(tmp_char);
		tmp_char = str;
		str= ft_strtrim(str, " ");
		free(tmp_char);
	}
	else
	{
		word = word_extractor(str);
		tmp_char = word;
		word = ft_strtrim(word, " ");
		free(tmp_char);
		tmp_char = str;
		str = first_word_remover(str, word);
		free(tmp_char);
		tmp_char = str;
		str = ft_strtrim(str, " ");
		free(tmp_char);
	}
	head = node_maker(word);
	tmp = head;
	while (str && *str)
	{
		if (*str == '\"')
		{
			word = str_extractor(str);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = str_remover(str, word);
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (is_operator(*str))
		{
			word = extract_operator(str);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = remove_operator(str, word);
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (*str == '$')
		{
			word = extract_variable(str);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = first_word_remover(str, word);
			free(tmp_char);
			tmp_char = str;
			str= ft_strtrim(str, " ");
			free(tmp_char);
		}
		else if (*str == '(')
		{
			word = extract_parenthesis(str);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = parenthesis_remover(str, word);
			free(tmp_char);
			tmp_char = str;
			str= ft_strtrim(str, " ");
			free(tmp_char);
		}
		else
		{
			word = word_extractor(str);
			tmp_char = word;
			word = ft_strtrim(word, " ");
			free(tmp_char);
			tmp_char = str;
			str = first_word_remover(str, word);
			free(tmp_char);
			tmp_char = str;
			str = ft_strtrim(str, " ");
			free(tmp_char);
		}
		new_node = malloc(sizeof(t_list));
		new_node->data = word;
		new_node->next = NULL;
		new_node->prev = tmp;
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

void	free_arr(char **arr)
{
	int i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*side_maker(t_list **head, int number, int j)
{
	t_list *tmp;
	char *tmp2;
	int i;

	tmp = *head;
	i = 0;
	tmp2 = NULL;
	while (i < number - j)
	{
		tmp2 = ft_strjoin(tmp2, tmp->data);
		tmp2 = ft_strjoin(tmp2, " ");
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
    *tree = malloc(sizeof(t_tree));
    if (!(*tree))
        return;
	last = NULL;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->command = NULL;
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
            command = malloc(sizeof(t_tree));
            if (!command)
                return;
            if (tmp->next)
                command->command = side_maker(&(tmp->next), total_nodes - i, 0);
            else
                command->command = NULL;
                
            command->left = NULL;
            command->right = NULL;
            command->redirections = NULL;
            command->type = "COMMAND";
            (*tree)->right = command;
            (*tree)->left = NULL;
            (*tree)->command = NULL;
            (*tree)->redirections = NULL;
            (*tree)->type = "OPERATION";
            if (tmp->prev)
            {
                prev_part = *head;
                tmp->prev->next = NULL;
                if (tmp->data)
                    free(tmp->data);
                free(tmp);
                if (prev_part)
                    tree_maker(&prev_part, &(*tree)->left);
            }
            else
            {
                if (tmp->data)
                    free(tmp->data);
                free(tmp);
                *head = NULL;
            }
            return;
        }
        tmp = tmp->prev;
        i--;
    }
    if (*head)
    {
        command = malloc(sizeof(t_tree));
        if (!command)
            return;
        
        command->command = side_maker(head, total_nodes, 0);
        command->left = NULL;
        command->redirections = NULL;
        command->right = NULL;
        command->type = "COMMAND";
        (*tree)->left = command;
        (*tree)->right = NULL;
        (*tree)->command = NULL;
        (*tree)->redirections= NULL;
        (*tree)->type = "COMMAND";
        current = *head;
        while (current)
        {
            to_free = current;
            current = current->next;
            if (to_free->data)
                free(to_free->data);
            free(to_free);
        }
        *head = NULL;
    }
}

t_tree *create_tree_node(void *command, char *type)
{
    t_tree *node = malloc(sizeof(t_tree));
    if (!node)
        return NULL;
    
    node->command = command;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->redirections = NULL;
    
    return node;
}

char *extract_command_until_pipe(t_list **head, t_list **pipe_pos)
{
    t_list *current = *head;
    char *command_str = NULL;
    char *temp;
    
    *pipe_pos = NULL;
    
    while (current)
    {
        if (current->token && ft_strcmp(current->token, "PIPE") == 0)
        {
            *pipe_pos = current;
            break;
        }
        
        if (command_str)
        {
            temp = command_str;
            command_str = ft_strjoin(command_str, " ");
            free(temp);
            
            if (command_str) {
                temp = command_str;
                command_str = ft_strjoin(command_str, current->data);
                free(temp);
            }
        }
        else
        {
            command_str = ft_strdup(current->data);
        }
        
        current = current->next;
    }
    
    return command_str;
}

t_tree *build_pipe_tree(t_list **head)
{
    t_list *pipe_pos;
    t_tree *root = NULL;
    t_tree *command_node;
    char *command_str;
    t_list *next_part;
    
    if (!head || !(*head))
        return NULL;
    
    command_str = extract_command_until_pipe(head, &pipe_pos);
    
    if (!pipe_pos)
    {
        root = create_tree_node(command_str, "COMMAND");
        return root;
    }
    
    root = create_tree_node(NULL, "PIPE");
    
    command_node = create_tree_node(command_str, "COMMAND");
    root->left = command_node;
    
    if (pipe_pos->next)
    {
        next_part = pipe_pos->next;
        pipe_pos->next = NULL;
        
        root->right = build_pipe_tree(&next_part);
    }
    
    return root;
}

void process_command_with_pipes(char *command_str, t_tree **command_tree)
{
    t_list *cmd_list;
    char *cmd_copy;
    
    if (!command_str || !(*command_str))
    {
        *command_tree = NULL;
        return;
    }
    cmd_copy = ft_strdup(command_str);
    if (!cmd_copy)
    {
        *command_tree = NULL;
        return;
    }
    cmd_list = list_init(cmd_copy);
    if (cmd_list)
        lexer(&cmd_list);
    
    *command_tree = build_pipe_tree(&cmd_list);
}

void free_tree(t_tree *tree)
{
    if (!tree)
        return;
    if (tree->left)
        free_tree(tree->left);
    if (tree->right)
        free_tree(tree->right);
    
    if (tree->command)
        free(tree->command);
    free(tree);
}

void process_pipe_trees(t_tree *tree)
{
    t_tree *cmd_tree = NULL;
    
    if (!tree)
        return;
    if (tree->command && tree->type && ft_strcmp(tree->type, "COMMAND") == 0)
    {
        char *command_str = (char *)tree->command;
        
        if (ft_strchr(command_str, '|'))
        {
            process_command_with_pipes(command_str, &cmd_tree);
            if (cmd_tree)
            {
                if (cmd_tree->type && ft_strcmp(cmd_tree->type, "PIPE") == 0)
                {
                    free(tree->command);
                    tree->command = NULL;
                    tree->type = cmd_tree->type;
                    if (tree->left)
                        free_tree(tree->left);
                    tree->left = cmd_tree->left;
                    tree->right = cmd_tree->right;
                    cmd_tree->left = NULL;
                    cmd_tree->right = NULL;
                    free(cmd_tree);
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
            t_tree *pipe_node = tree->left;
            t_tree *right_child = pipe_node->right;
            tree->type = pipe_node->type;
            t_tree *left_child = pipe_node->left;
            tree->left = left_child;
            if (tree->right)
                free_tree(tree->right);
            tree->right = right_child;
            pipe_node->left = NULL;
            pipe_node->right = NULL;
            free(pipe_node);
        }
    }
    if (tree->left)
        process_pipe_trees(tree->left);
    if (tree->right)
        process_pipe_trees(tree->right);
}

// void print_tree_visual(t_tree *tree, int level, int is_left)
// {
//     if (!tree)
//         return;
//     for (int i = 0; i < level; i++)
//     {
//         if (i == level - 1)
//             printf("%s", is_left ? "----" : "    ");
//         else
//             printf("    ");
//     }
//     printf("%s", tree->type);
//     if (tree->command)
//     {
//         printf(" (Command: %s)\n", tree->command);
//     }
//     else
//     {
//         printf(" (Command: NULL)\n");
//     }
//     if (tree->left || tree->right)
//     {
//         for (int i = 0; i < level; i++)
//             printf("    ");
//         printf("---left;\n");
//     }
//     if (tree->left)
//     {
//         print_tree_visual(tree->left, level + 1, 1);
//     }
//     if (tree->right)
//     {
//         for (int i = 0; i < level; i++)
//             printf("    ");
//         printf("---right;\n");
//     }
//     if (tree->right)
//     {
//         print_tree_visual(tree->right, level + 1, 0);
//     }
// }

void print_tree_visual(t_tree *tree, int level, int is_left)
{
    if (!tree)
        return;
    
    // Print indentation and node type
    for (int i = 0; i < level; i++)
    {
        if (i == level - 1)
            printf("%s", is_left ? "----" : "    ");
        else
            printf("    ");
    }
    
    // Print node type safely
    printf("%s", tree->type ? tree->type : "NULL");
    
    // Print command if it exists (safely)
    if (tree->command)
    {
        printf(" (Command: %s)", tree->command);
    }
    else
    {
        printf(" (Command: NULL)");
    }
    
    // Print redirections if they exist (safely)
    if (tree->redirections)
    {
        printf(" [Redirections: %s]", tree->redirections);
    }
    
    // End the line
    printf("\n");
    
    // Print left child indication
    if (tree->left || tree->right)
    {
        for (int i = 0; i < level; i++)
            printf("    ");
        printf("---left;\n");
    }
    
    // Print left child recursively
    if (tree->left)
    {
        print_tree_visual(tree->left, level + 1, 1);
    }
    
    // Print right child indication
    if (tree->right)
    {
        for (int i = 0; i < level; i++)
            printf("    ");
        printf("---right;\n");
    }
    
    // Print right child recursively
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
	content = ft_substr(command, start, end - start);
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
                    cmd_list = list_init(inner_cmd_copy);
                    if (cmd_list)
                    {
                        lexer(&cmd_list);
                        tree_maker(&cmd_list, &subtree);
                        if (subtree)
                        {
                            process_pipe_trees(subtree);
                            free(tree->command);
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
                            free(subtree);
                        }
                    }
                }
                free(inner_cmd);
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
    if (!tree)
        return;
    if (tree->command && tree->type && ft_strcmp(tree->type, "COMMAND") == 0) {
        if (has_outer_parentheses(tree->command)) {
            t_tree *subtree = NULL;
            char *inner_cmd = extract_content_from_parentheses(tree->command);
            if (inner_cmd) {
                t_list *cmd_list = list_init(ft_strdup(inner_cmd));
                if (cmd_list) {
                    lexer(&cmd_list);
                    tree_maker(&cmd_list, &subtree);
                    if (subtree) {
                        process_pipe_trees(subtree);
                        free(tree->command);
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
                        free(subtree);
                    }
                }
                free(inner_cmd);
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
	new_str = malloc(sizeof(char) * (strlenth - wordlen + 1));
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
void process_redirections(t_tree **tree)
{
    if (!tree || !*tree)
        return;
    if ((*tree)->left)
        process_redirections(&(*tree)->left);
    if ((*tree)->right)
        process_redirections(&(*tree)->right);
    if (tree && *tree && (*tree)->command)
    {
        char *redir_pos = ft_strchr((*tree)->command, '>');
        if (redir_pos)
        {
            int cmd_length = redir_pos - (*tree)->command;
            char *cmd_prefix = malloc(sizeof(char) * (cmd_length + 1));
            if (cmd_prefix)
            {
                ft_strncpy(cmd_prefix, (*tree)->command, cmd_length);
                cmd_prefix[cmd_length] = '\0';
                char *params_start = NULL;
                char *current_pos = redir_pos;
                while (*current_pos)
                {
                    if (*current_pos == '>')
                    {
                        while (*current_pos == '>')
                            current_pos++;
                        while (*current_pos == ' ')
                            current_pos++;
                        while (*current_pos && *current_pos != ' ' && *current_pos != '>')
                            current_pos++;
                        while (*current_pos == ' ')
                            current_pos++;
                        if (*current_pos == '>')
                            continue;
                        if (*current_pos && *current_pos != '>')
                        {
                            params_start = current_pos;
                            break;
                        }
                    }
                    else
                        current_pos++;
                }
                int redir_length;
                if (params_start)
                    redir_length = params_start - redir_pos;
                else
                    redir_length = ft_strlen(redir_pos);
                (*tree)->redirections = malloc(sizeof(char) * (redir_length + 1));
                if ((*tree)->redirections)
                {
                    ft_strncpy((*tree)->redirections, redir_pos, redir_length);
                    (*tree)->redirections[redir_length] = '\0';
                }
                char *new_command = NULL;
                if (params_start)
                {
                    new_command = malloc(sizeof(char) * (cmd_length + ft_strlen(params_start) + 2));
                    if (new_command)
                    {
                        ft_strcpy(new_command, cmd_prefix);
                        ft_strcat(new_command, " ");
                        ft_strcat(new_command, params_start);
                    }
                }
                else
                {
                    new_command = ft_strdup(cmd_prefix);
                }
                
                free((*tree)->command);
                (*tree)->command = new_command;
                
                free(cmd_prefix);
            }
        }
    }
}
void process_redirections_two(t_tree **tree)
{
    if (!tree || !*tree)
        return;
    if ((*tree)->left)
        process_redirections_two(&(*tree)->left);
    if ((*tree)->right)
        process_redirections_two(&(*tree)->right);
    if (*tree && (*tree)->command)
    {
        char *redir_pos = ft_strchr((*tree)->command, '<');
        if (redir_pos)
        {
            int cmd_length = redir_pos - (*tree)->command;
            char *cmd_prefix = malloc(sizeof(char) * (cmd_length + 1));
            if (cmd_prefix)
            {
                ft_strncpy(cmd_prefix, (*tree)->command, cmd_length);
                cmd_prefix[cmd_length] = '\0';
                char *params_start = NULL;
                char *current_pos = redir_pos;
                while (*current_pos)
                {
                    if (*current_pos == '<')
                    {
                        while (*current_pos == '<')
                            current_pos++;
                        while (*current_pos == ' ')
                            current_pos++;
                        while (*current_pos && *current_pos != ' ' && *current_pos != '<')
                            current_pos++;
                        while (*current_pos == ' ')
                            current_pos++;
                        if (*current_pos == '<')
                            continue;
                        if (*current_pos && *current_pos != '<')
                        {
                            params_start = current_pos;
                            break;
                        }
                    }
                    else
                        current_pos++;
                }
                int redir_length;
                if (params_start)
                    redir_length = params_start - redir_pos;
                else
                    redir_length = ft_strlen(redir_pos);
                (*tree)->redirections = malloc(sizeof(char) * (redir_length + 1));
                if ((*tree)->redirections)
                {
                    ft_strncpy((*tree)->redirections, redir_pos, redir_length);
                    (*tree)->redirections[redir_length] = '\0';
                }
                char *new_command = NULL;
                if (params_start)
                {
                    new_command = malloc(sizeof(char) * (cmd_length + ft_strlen(params_start) + 2));
                    if (new_command)
                    {
                        ft_strcpy(new_command, cmd_prefix);
                        ft_strcat(new_command, " ");
                        ft_strcat(new_command, params_start);
                    }
                }
                else
                {
                    new_command = ft_strdup(cmd_prefix);
                }
                
                free((*tree)->command);
                (*tree)->command = new_command;
                
                free(cmd_prefix);
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
		if (str[i] == '\'' || str[i] == '\"')
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

// int check_redirs(char *str)
// {
// 	int i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		printf("-------%c\n", str[i]);
// 		printf("-------%d\n", i);
// 		if (str[i] == '\'' || str[i] == '\"')
// 			i = skip_quotes(str, i);
// 		else if (str[i] == '>' || str[i] == '<')
// 		{
// 			if (str[i + 1] == str[i])
// 				i++;
// 			i++;
// 			printf("-------%d\n", str[i]);
// 			while (str[i] == 32)
// 				i++;
// 			if (!str[i] || str[i] == '>' || str[i] == '<' || str[i] == '&' || str[i] == '|')
// 			{
// 				if (!str[i])
// 					return (write(2, "syntax error near unexpected token `newline'\n", 46), 1);
// 				else
// 					return (print_syntax_error(&str[i]), 1);
// 			}
// 		}
// 		else
// 			i++;
// 	}
// 	return (0);
// }

void	syntax_error(t_list **head)
{
	t_list	*tmp;
	char	*prev_token;
	char	*prev_data;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		prev_token = tmp->token;
		prev_data = tmp->data;
		tmp = tmp->next;
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
		if (ft_strcmp("REDIRECTION", prev_token) == 0 && ft_strcmp(tmp->token, "PIPE") == 0)
		{
			print_syntax_error(prev_data);
			exit(1);
		}
		if (ft_strcmp("PIPE", prev_token) == 0 && ft_strcmp(tmp->token, "REDIRECTION") == 0)
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
		// if (check_redirs((*tree)->redirections) == 1)
		// 	exit (1);
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
			*head = malloc(sizeof(t_list_fd));
			(*head)->name = ft_substr((*tree)->redirections, j, i - j);
			(*head)->command = ft_strdup((*tree)->command);
			(*tree)->redirections = ft_substr((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i);
			(*tree)->redirections = ft_strtrim((*tree)->redirections, " ");
			if (flag == 1)
			{
				(*head)->fd = open((*head)->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				(*head)->redir = ft_strdup(">");
			}
			if (flag == 2)
			{
				(*head)->fd = open((*head)->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
				(*head)->redir = ft_strdup(">>");
			}
			if (flag == 3)
			{
				(*head)->fd = open((*head)->name, O_RDONLY, 0644);
				(*head)->redir = ft_strdup("<");
			}
			if (flag == 4)
			{
				(*head)->fd = open((*head)->name, O_RDONLY, 0644);
				ft_strdup("<<");
			}
			(*head)->next = NULL;
			tmp = *head;
			while ((*tree)->redirections)
			{
				i = 0;
				while ((*tree)->redirections != NULL && (*tree)->redirections[i] && ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
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
				new_node = malloc(sizeof(t_list_fd));
				new_node->name = ft_substr((*tree)->redirections, j, i - j);
				new_node->command = ft_strdup((*tree)->command);
				(*tree)->redirections = ft_substr((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i);
				(*tree)->redirections = ft_strtrim((*tree)->redirections, " ");
				if (flag == 1)
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
					new_node->redir = ft_strdup(">");
				}
				if (flag == 2)
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
					new_node->redir = ft_strdup(">>");
				}
				if (flag == 3)
				{
					new_node->fd = open(new_node->name, O_RDONLY,  0644);
					new_node->redir = ft_strdup("<");
				}
				if (flag == 4)
				{
					new_node->fd = open(new_node->name, O_RDONLY, 0644);
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
			while ((*tree)->redirections)
			{
				i = 0;
				while ((*tree)->redirections != NULL && (*tree)->redirections[i] && ((*tree)->redirections[i] != '>' && (*tree)->redirections[i] != '<'))
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
				new_node = malloc(sizeof(t_list_fd));
				new_node->name = ft_substr((*tree)->redirections, j, i - j);
				new_node->command = ft_strdup((*tree)->command);
				(*tree)->redirections = ft_substr((*tree)->redirections, i, ft_strlen((*tree)->redirections) - i);
				(*tree)->redirections = ft_strtrim((*tree)->redirections, " ");
				if (flag == 1)
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
					new_node->redir = ft_strdup(">");
				}
				if (flag == 2)
				{
					new_node->fd = open(new_node->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
					new_node->redir = ft_strdup(">>");
				}
				if (flag == 3)
				{
					new_node->fd = open(new_node->name, O_RDONLY,  0644);
					new_node->redir = ft_strdup("<");
				}
				if (flag == 4)
				{
					new_node->fd = open(new_node->name, O_RDONLY, 0644);
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

int main(void)
{
	char *str;
	t_list *head;
	t_tree *tree;
	t_list *tmp;
	t_list_fd *tmp_fd;
	t_list_fd *head_fd;
	int i;

	while (1)
	{
		str = readline("minishell$> ");
		i = 0;
		if (!str)
			break;
		add_history(str);
		str = replace_whites_spaces(str);
		head = list_init(str);
		lexer(&head);
		tmp = head;
		// while (tmp)
		// {
		// 	printf("%s\n", tmp->data);
		// 	printf("%s\n", tmp->token);
		// 	printf("\n");
		// 	tmp = tmp->next;
		// }
		syntax_error(&head);
		tree_maker(&head, &tree);
		process_pipe_trees(tree);
		process_nested_parentheses(&tree);
		process_redirections(&tree);
		process_redirections_two(&tree);
		print_tree_visual(tree, 1, 1);
		syntax_error_two(&tree);
		redirections_opener(&tree, &head_fd);
		tmp_fd = head_fd;
		while (tmp_fd)
		{
			printf("filename == %s\n", tmp_fd->name);
			printf("filedescriptor == %d\n", tmp_fd->fd);
			printf("command == %s\n", tmp_fd->command);
			printf("redir == %s\n", tmp_fd->redir);
			printf("\n");
			tmp_fd = tmp_fd->next;
		}
	}
}
