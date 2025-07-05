/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_maker_helper_three.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:12:02 by makkach           #+#    #+#             */
/*   Updated: 2025/07/05 19:56:46 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_operation_node(t_list **head, t_tree **tree, t_list *tmp,
							int *node_info)
{
	t_list	*prev_part;

	init_tree_node(tree, tmp->token);
	if (!(*tree))
		return ;
	handle_operation_command(tree, tmp, node_info[0], node_info[1]);
	if (!(*tree))
		return ;
	if_prev(tmp, &prev_part, tree, head);
	*head = NULL;
}

void	process_pipe_node(t_list **head, t_tree **tree, t_list *tmp,
						int *node_info)
{
	t_list	*prev_part;

	init_tree_node(tree, "PIPE");
	if (!(*tree))
		return ;
	handle_operation_command(tree, tmp, node_info[0], node_info[1]);
	if (!(*tree))
		return ;
	if_prev(tmp, &prev_part, tree, head);
	*head = NULL;
}

// int	in_para_check(char *str)
// {
// 	int	i;
// 	int	flag;

// 	i = 0;
// 	flag = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == ')')
// 		{
// 			i++;
// 			while (str[i] == 32)
// 				i++;
// 			if (str[i] && !is_operator(str[i]))
// 				return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

void	handle_quotes_two(char **word, char **str)
{
	int		i;
	int		in_quotes;
	char	quote_type;
	char	*tmp_char;

	i = 0;
	in_quotes = 0;
	while ((*str)[i])
	{
		if (!in_quotes && ((*str)[i] == '"' || (*str)[i] == '\''))
		{
			in_quotes = 1;
			quote_type = (*str)[i];
		}
		else if (in_quotes && (*str)[i] == quote_type)
			in_quotes = 0;
		if (!in_quotes)
		{
			i++;
			break ;
		}
		i++;
	}
	*word = ft_substr(*str, 0, i);
	if (!*word)
		return ;
	tmp_char = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp_char);
	if (!*word)
		return ;
	tmp_char = *str;
	*str = ft_substr(*str, i, ft_strlen(*str) - i);
	free(tmp_char);
	if (!*str)
		return ;
	tmp_char = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp_char);
	if (!*str)
		return ;
}

void	handle_else_two(char **str, char **word)
{
	int		i;
	char	*tmp;

	i = 0;
	while ((*str)[i] && !is_operator((*str)[i]) && (*str)[i] != '(' && (*str)[i] != ')' && (*str)[i] != '$' && (*str)[i] != 32)
		i++;
	*word = ft_substr(*str, 0, i);
	if (!*word)
		return ;
	tmp = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp);
	if (!*word)
		return ;
	tmp = *str;
	*str = ft_substr(*str, i, ft_strlen(*str) - i);
	free(tmp);
	if (!*str)
		return ;
	tmp = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp);
	if (!*str)
		return ;
}

void	handle_parenthases_two(char **str, char **word)
{
	char	*tmp;
	int		i;

	i = 0;
	*word = ft_substr(*str, 0, 1);
	if (!*word)
		return ;
	tmp = *word;
	*word = ft_strtrim(*word, " ");
	free(tmp);
	if (!*word)
		return ;
	tmp = *str;
	*str = ft_substr(*str, 1, ft_strlen(*str) - 1);
	free(tmp);
	if (!*str)
		return ;
	tmp = *str;
	*str = ft_strtrim(*str, " ");
	free(tmp);
	if (!*str)
		return ;
}

void	cut_and_slice_two(char **word, char **str)
{
	if (**str == '\"' || **str == '\'')
		handle_quotes_two(word, str);
	else if (is_operator(**str))
		handle_operator(word, str);
	else if (**str == '$')
		variable_handle(word, str);
	else if (**str == '(' || **str == ')')
		handle_parenthases_two(str, word);
	else
		handle_else_two(str, word);
}

t_list	*new_list_init_two(char *str)
{
	char	*word;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	word = NULL;
	head = NULL;
	new_node = NULL;
	tmp = NULL;
	if (!str || !*str)
		return (NULL);
	cut_and_slice_two(&word, &str);
	head = node_maker(word);
	if (!head)
		return (NULL);
	tmp = head;
	while (str && *str)
	{
		cut_and_slice_two(&word, &str);
		handle_new_node(&new_node, &tmp, &word);
	}
	if (str)
		free(str);
	return (head);
}

int	parenthasis_recognizer_two(char *str)
{
	if (!ft_strcmp("(", str))
		return (1);
	else if (!ft_strcmp(")", str))
		return (2);
	return (0);
}

void	tokenizer_two(t_list *tmp)
{
	if (command_recognizer(tmp->data) == 1)
		tmp->token = "COMMAND";
	else if (variable_recognizer(tmp->data) == 1)
		tmp->token = "VARIABLE";
	else if (parenthasis_recognizer_two(tmp->data) == 1)
		tmp->token = "OPEN_PAR";
	else if (parenthasis_recognizer_two(tmp->data) == 2)
		tmp->token = "CLOSED_PAR";
	else if (word_recognizer(tmp->data) == 1)
		tmp->token = "WORD";
	else if (pipe_recognizer(tmp->data) == 1)
		tmp->token = "PIPE";
	else if (pipe_recognizer(tmp->data) == 2)
		tmp->token = "OPERATION_||";
	else if (redirection_recognizer(tmp->data) == 1)
		tmp->token = "REDIRECTION";
	else if (operation_recognizer(tmp->data) == 1)
		tmp->token = "OPERATION_&&";
	else
		tmp->token = "WORD";
}

void	lexer_two(t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (tmp)
	{
		tokenizer_two(tmp);
		tmp = tmp->next;
	}
}

int	par_count(t_list *head)
{
	int	open_par;
	int	closed_par;
	t_list	*tmp;

	open_par = 0;
	closed_par = 0;
	tmp = head;
	while (tmp)
	{
		if (tmp && !ft_strcmp(tmp->token, "OPEN_PAR"))
			open_par++;
		else if (tmp && !ft_strcmp(tmp->token, "CLOSED_PAR"))
			closed_par++;
		tmp = tmp->next;
	}
	if (open_par == closed_par)
		return (0);
	return (1);
}

int	parenthasis_syntax_error(t_list *head)
{
	t_list	*tmp;
	char	*prev_data;
	char	*prev_token;

	tmp = head;
	while (tmp)
	{
		prev_data = tmp->data;
		prev_token = tmp->token;
		tmp = tmp->next;
		// printf("%s\n", prev_token);
		// printf("%s\n", prev_data);
		// if (tmp)
		// {
		// 	// printf("%s\n", tmp->token);
		// 	printf("%s\n\n", tmp->data);

		// }
	}
	tmp = head;
	while (tmp)
	{
		prev_data = tmp->data;
		prev_token = tmp->token;
		tmp = tmp->next;
		if (!ft_strcmp("&", prev_data))
			return (1);
		else if (tmp && !ft_strcmp(prev_token, "OPEN_PAR") && (!ft_strcmp(tmp->token, "CLOSED_PAR")))
			return (1);
		else if (tmp && !ft_strcmp(prev_token, "OPEN_PAR") && ((!ft_strcmp(tmp->token, "OPERATION_||") || !ft_strcmp(tmp->token, "OPERATION_&&"))))
			return (1);
		else if (tmp && !ft_strcmp(prev_token, "OPEN_PAR") && !ft_strcmp(tmp->token, "PIPE"))
			return (1);
		else if (tmp && !ft_strcmp(prev_token, "PIPE") && !ft_strcmp(tmp->token, "CLOSED_PAR"))
			return (1);
		else if (tmp && !ft_strcmp(prev_token, "REDIRECTION") && !ft_strcmp(tmp->token, "CLOSED_PAR"))
			return (1);
		else if (tmp && !ft_strcmp(prev_token, "REDIRECTION") && !ft_strcmp(tmp->token, "PIPE"))
			return (1);
		else if (tmp && !ft_strcmp(prev_token, "REDIRECTION") && (!ft_strcmp(tmp->token, "OPERATION_&&") || !ft_strcmp(tmp->token, "OPERATION_||")))
			return (1);
		else if (tmp && ((!ft_strcmp(prev_token, "OPERATION_||") || !ft_strcmp(prev_token, "OPERATION_&&"))) && (!ft_strcmp(tmp->token, "CLOSED_PAR")))
			return (1);
		else if (tmp && ((!ft_strcmp(prev_token, "OPERATION_||") || !ft_strcmp(prev_token, "OPERATION_&&"))) && ((!ft_strcmp(tmp->token, "OPERATION_||") || !ft_strcmp(tmp->token, "OPERATION_&&"))))
			return (1);
		else if (tmp && (!ft_strcmp(prev_token, "CLOSED_PAR") && (!ft_strcmp(tmp->token, "WORD") || !ft_strcmp(tmp->token, "COMMAND"))))
			return (1);
		else if (tmp && ((!ft_strcmp(prev_token, "WORD") || !ft_strcmp(prev_token, "COMMAND")) && !ft_strcmp(tmp->token, "OPEN_PAR")))
			return (1);
		else if (tmp && (!ft_strcmp(prev_token, "REDIRECTION") && !ft_strcmp(tmp->token, "OPEN_PAR")))
			return (1);
		else if (par_count(head))
			return (1);
	}
	return (0);
}

int	in_para_check(char *str)
{
	t_list	*head;

	char	*new_str;

	new_str = ft_strdup(str);
	head = new_list_init_two(new_str);
	lexer_two(&head);
	if (parenthasis_syntax_error(head))
		return (free_list(&head), 1);
	free_list(&head);
	return (0);
}
