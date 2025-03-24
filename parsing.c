/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/03/24 16:27:47 by makkach          ###   ########.fr       */
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

void	tokenizer(t_list *tmp)
{
	if (command_recognizer(tmp->data) == 1)
		tmp->token = "COMMAND";
	else if (variable_recognizer(tmp->data) == 1)
		tmp->token = "VARIABLE";
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
char *extract_variable(char *str)
{
	int i;

	i = 0;
	while (str[i] != 32 && str[i] != '\0')
		i++;
	return (ft_substr(str, 0, i + 1));
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
            command->type = "COMMAND";
            (*tree)->right = command;
            (*tree)->left = NULL;
            (*tree)->command = NULL;
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
        command->right = NULL;
        command->type = "COMMAND";
        
        (*tree)->left = command;
        (*tree)->right = NULL;
        (*tree)->command = NULL;
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
    printf("%s", tree->type);
    if (tree->command)
    {
        printf(" (Command: %s)\n", tree->command);
    }
    else
    {
        printf(" (Command: NULL)\n");
    }
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
int main(void)
{
	char *str;
	t_list *head;
	t_tree *tree;
	t_list *tmp;
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
		tree_maker(&head, &tree);
		process_pipe_trees(tree);
		print_tree_visual(tree, 1, 1);
	}
}
