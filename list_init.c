/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:06:02 by makkach           #+#    #+#             */
/*   Updated: 2025/04/19 11:06:27 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list *node_maker(char *word)
{
	t_list *node;
	node = t_malloc(sizeof(t_list), 423, "parsing.c");
	node->data = word;
	node->next = NULL;
	node->prev = NULL;
	return (node);
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