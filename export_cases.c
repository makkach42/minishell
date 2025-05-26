/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:11:30 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 10:12:29 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_cases(t_tree **tree)
{
	int		i;
	int		j;
	int		flag;
	int		in_quotes;
	char	quote_type;
	char	*old_cmd;
	char	*tmp_char;
	char	*new_str;
	t_list	*head;
	t_list	*tmp;
	t_list	*tmp2;

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
				old_cmd = ft_strdup((*tree)->command_arr[i]);
				head = new_list_init(old_cmd);
				tmp = head;
				new_str = NULL;
				while (tmp)
				{
					if (!ft_strcmp(tmp->data, "="))
					{
						tmp2 = tmp;
						if (!isnt_valid(tmp2->prev->data))
						{
							tmp2 = tmp->next;
							while (tmp2)
							{
								j = 0;
								flag = 0;
								in_quotes = 0;
								quote_type = 0;
								while (tmp2->data[j])
								{
									if (!in_quotes && (tmp2->data[j] == '"' || tmp2->data[j] == '\''))
									{
										in_quotes = 1;
										quote_type = tmp2->data[j];
									}
									else if (in_quotes && tmp2->data[j] == quote_type)
										in_quotes = 0;
									if (tmp2->data[j] == '$' && !in_quotes)
										flag = 1;
									j++;
								}
								if (flag == 1)
								{
									tmp_char = tmp2->data;
									tmp2->data = ft_strjoin_three("\"", tmp2->data, "\"");
									free(tmp_char);
								}
								tmp2 = tmp2->next;
							}
						}
					}
					tmp = tmp->next;
				}
				tmp = head;
				while (tmp)
				{
					tmp_char = new_str;
					new_str = ft_strjoin(new_str, tmp->data);
					free(tmp_char);
					tmp = tmp->next;
				}
				free((*tree)->command_arr[i]);
				(*tree)->command_arr[i] = new_str;
				free_list(&head);
				i++;
			}
		}
	}
}
