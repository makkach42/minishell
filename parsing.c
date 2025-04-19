/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 11:10:36 by makkach          ###   ########.fr       */
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

t_tree *create_tree_node(void *command, char *type)////////////////////
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
    int i;
    int cmd_len;
    int in_quotes;
    int paren_count;
    char quote_type;
    char *redir_start;
    
    *cmd_part = NULL;
    *redir_part = NULL;
    
    if (!cmd_str)
        return;
    
    i = 0;
    in_quotes = 0;
    paren_count = 0;
    quote_type = 0;
    redir_start = NULL;
    while (cmd_str[i]) {
        if ((cmd_str[i] == '\'' || cmd_str[i] == '\"') && !in_quotes) {
            in_quotes = 1;
            quote_type = cmd_str[i];
        } else if (in_quotes && cmd_str[i] == quote_type) {
            in_quotes = 0;
        }
        if (!in_quotes) {
            if (cmd_str[i] == '(') {
                paren_count++;
            } else if (cmd_str[i] == ')') {
                paren_count--;
            }
            if ((cmd_str[i] == '>' || cmd_str[i] == '<') && paren_count == 0 && !redir_start) {
                redir_start = &cmd_str[i];
                break;
            }
        }
        
        i++;
    }
    if (!redir_start) {
        *cmd_part = ft_strdup(cmd_str);
        return;
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
int	check_if_in_string(char *cmd)
{
	int i;
	int	in_quotes;
	char quote_type;

	i = 0;
	in_quotes = 0;
	while (cmd[i])
	{
		if (!in_quotes && (cmd[i] == '\'' || cmd[i] == '"'))
		{
			in_quotes = 1;
			quote_type = cmd[i];
		}
		else if (in_quotes && cmd[i] == quote_type)
		{
			in_quotes = 0;
			quote_type = '\0';
		}
		if ((cmd[i] == '>' || cmd[i] == '<') && !in_quotes)
			break;
		i++;
	}
	if (cmd[i] == '\0')
		return (0);
	return (1);
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
        if ((redir_out || redir_in) && check_if_in_string(cmd)) {
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
    if (tmp && (ft_strcmp(tmp->prev->data, "<<")))
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
		tmp = head;
		while (tmp)
		{
			printf("%s\n", tmp->data);
			printf("%s\n", tmp->token);
			tmp = tmp->next;
		}
		// join_redirections_with_commands(&head);
		tmp = head;
		if (variable_search(&head))
            variable_expantion(&head, argev);
		variable_in_word(&head, argev);
		tmp = head;
		syntax_error(&head);
		tree_maker(&head, &tree);
		// print_tree_visual(tree, 1, 1);
		// printf("\n");
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