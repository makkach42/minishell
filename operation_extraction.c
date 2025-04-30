/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_extraction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:59:35 by makkach           #+#    #+#             */
/*   Updated: 2025/04/30 16:40:09 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_operator(char *str)
{
	char	*operator;

	if (!str | !*str)
		return (NULL);
	operator = NULL;
	if (str[0] == '&' && ((str[1] && str[1] != '&') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '*' && ((str[1] && str[1] != '*') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '|' && ((str[1] && str[1] != '|') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '>' && ((str[1] && str[1] != '>') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '<' && ((str[1] && str[1] != '<') || !str[1]))
		operator = ft_substr(str, 0, 1);
	else if (str[0] == '&' && str[1] && str[1] == '&')
		operator = ft_substr(str, 0, 2);
	else if (str[0] == '|' && str[1] && str[1] == '|')
		operator = ft_substr(str, 0, 2);
	else if (str[0] == '>' && str[1] && str[1] == '>')
		operator = ft_substr(str, 0, 2);
	else if (str[0] == '<' && str[1] && str[1] == '<')
		operator = ft_substr(str, 0, 2);
	return (operator);
}

char	*remove_operator(char *str, char *word)
{
	int		wordlen;
	int		str_len;
	char	*str_word;

	if (!str || !word)
		return (NULL);
	wordlen = ft_strlen(word);
	str_len = ft_strlen(str);
	str_word = ft_substr(str, wordlen, str_len - wordlen + 1);
	return (str_word);
}

// char	*extract_variable(char *str)
// {
// 	int		i;
// 	char	*word;

// 	i = 0;
// 	while (str[i] && str[i] != 32 && (!is_operator(str[i]) || str[i] != '$'))
// 		i++;
// 	word = ft_substr(str, 0, i);
// 	return (word);
// }

char	*extract_variable(char *str)
{
	int		i;
	char	*word;

	i = 1;
	while (str[i] && str[i] != 32 && !is_operator(str[i]) && str[i] != '$')
	{
		if (str[i] >= '0' && str[i] <= '9' && str[i - 1] == '$')
		{
			i++;
			break ;
		}
		i++;
	}
	word = ft_substr(str, 0, i);
	return (word);
}

// char	*extract_variable(char *str)
// {
// 	int		i;
// 	int		flag;
// 	int		flag2;
// 	int		flag3;
// 	char	*word;

// 	i = 0;
// 	flag = 0;
// 	flag2 = 0;
// 	flag3 = 0;
// 	while (str[i] && str[i] != 32 && !is_operator(str[i]))
// 	{
// 		if (str[i] == '$')
// 		{
// 			flag2 = 1;
// 			flag3 = 1;
// 		}
// 		if (flag2 == 1 && !(str[i] >= '0' && str[i] <= '9') && !(str[i] == '$'))
// 			flag2 = 0;
// 		if (flag2 == 1 && str[i] >= '0' && str[i] <= '9')
// 			flag = 1;
// 		if (flag == 1)
// 			break ;
// 		i++;
// 	}
// 	word = ft_substr(str, 0, i + 1);
// 	return (word);
// }








// void	if_variable_instr(t_tree **tree)
// {
// 	int		i;
// 	char	*left_side;
// 	char	*right_side;
// 	char	*new_command;

// 	i = 0;
// 	while ((*tree)->command[i] && (*tree)->command[i] != '$')
// 		i++;
// 	left_side = ft_substr((*tree)->command, 0, i);
// 	while ((*tree)->command[i] == '$')
// 		i++;
// 	while ((*tree)->command[i] == 32)
// 		i++;
// 	while ((*tree)->command[i] != 32)
// 		i++;
// 	while ((*tree)->command[i] == 32)
// 		i++;
// 	right_side = ft_substr((*tree)->command, i, ft_strlen((*tree)->command) - i);
// 	new_command = ft_strjoin(left_side, right_side);
// 	(free((*tree)->command), free(right_side), free(left_side));
// 	(*tree)->command = new_command;
// }

// void	remove_variables_instr(t_tree **tree)
// {
// 	if ((*tree)->left)
// 		remove_variables_instr(&(*tree)->left);
// 	if ((*tree)->right)
// 		remove_variables_instr(&(*tree)->right);
// 	if ((*tree)->command)
// 	{
// 		if (variable_search_instr((*tree)->command))
// 			if_variable_instr(tree);
// 	}
// }


void	if_variable_innode(t_list **head)
{
	t_list *tmp;
	t_list *next;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		next = tmp->next;
		if (!ft_strcmp(tmp->token, "VARIABLE"))
		{
			if (ft_strlen(tmp->data) > 1 && tmp->data[1] >= '0' && tmp->data[1] <= '9')	
			{
				if (*head == tmp)
					*head = tmp->next;
				if (tmp->prev)
					tmp->prev->next = tmp->next;
				if (tmp->next)
					tmp->next->prev = tmp->prev;
				if (tmp->data)
					free(tmp->data);
				free(tmp);
			}
		}
		tmp = next;
	}
}
