/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:11:30 by makkach           #+#    #+#             */
/*   Updated: 2025/05/29 09:32:47 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_quotes(t_list **tmp2)
{
	char	*tmp_char;

	tmp_char = (*tmp2)->data;
	(*tmp2)->data = ft_strjoin_three(
			"\"", (*tmp2)->data, "\"");
	free(tmp_char);
}

void	while_tmp2(t_list **tmp2)
{
	int		j;
	int		flag;
	int		in_quotes;
	char	quote_type;

	j = -1;
	flag = 0;
	in_quotes = 0;
	while ((*tmp2)->data[++j])
	{
		if (!in_quotes && (((*tmp2)->data[j] == '"'
				) || (*tmp2)->data[j] == '\''))
		{
			in_quotes = 1;
			quote_type = (*tmp2)->data[j];
		}
		else if (in_quotes && (
				(*tmp2)->data[j] == quote_type))
			in_quotes = 0;
		if ((*tmp2)->data[j] == '$' && !in_quotes)
			flag = 1;
	}
	if (flag == 1)
		add_quotes(tmp2);
}

// void	while_tmp(t_list **tmp)
// {
// 	t_list	*tmp2;

// 	while ((*tmp))
// 	{
// 		if (!ft_strcmp((*tmp)->data, "="))
// 		{
// 			tmp2 = (*tmp);
// 			if (!isnt_valid(tmp2->prev->data))
// 			{
// 				tmp2 = (*tmp)->next;
// 				while (tmp2)
// 				{
// 					while_tmp2(&tmp2);
// 					tmp2 = tmp2->next;
// 				}
// 			}
// 		}
// 		(*tmp) = (*tmp)->next;
// 	}
// }

void	while_tmp(t_list **tmp)
{
	t_list	*tmp2;
	t_list	*first_eq_pos;
	int		valid_key;

	first_eq_pos = NULL;
	valid_key = 0;
	while ((*tmp))
	{
		if (!ft_strcmp((*tmp)->data, "=") && !first_eq_pos)
		{
			first_eq_pos = (*tmp);
			if (!isnt_valid(first_eq_pos->prev->data))
				valid_key = 1;
			if (valid_key)
			{
				tmp2 = first_eq_pos->next;
				while (tmp2)
					(while_tmp2(&tmp2), tmp2 = tmp2->next);
			}
			while ((*tmp) && (*tmp)->next)
				(*tmp) = (*tmp)->next;
		}
		(*tmp) = (*tmp)->next;
	}
}

void	change_current_string(char **str)
{
	char	*old_cmd;
	char	*new_str;
	char	*tmp_char;
	t_list	*head;
	t_list	*tmp;

	old_cmd = ft_strdup(*str);
	head = new_list_init(old_cmd);
	tmp = head;
	new_str = NULL;
	while_tmp(&tmp);
	tmp = head;
	while (tmp)
	{
		tmp_char = new_str;
		new_str = ft_strjoin(new_str, tmp->data);
		free(tmp_char);
		tmp = tmp->next;
	}
	free(*str);
	*str = new_str;
	free_list(&head);
}

void	export_cases(t_tree **tree)
{
	int		i;

	if ((*tree) && (*tree)->left)
		export_cases(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		export_cases(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		i = 1;
		if (!ft_strcmp((*tree)->command_arr[0], "export"))
		{
			while ((*tree)->command_arr[i])
			{
				change_current_string(&(*tree)->command_arr[i]);
				i++;
			}
		}
	}
}
