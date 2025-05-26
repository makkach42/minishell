/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:08:07 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 10:08:41 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_vars(t_tree **tree, t_env **env)
{
	int	i;
	int	j;
	int	m;
	int	in_quotes;
	int	final_len;
	int	list_size;
	char quote_type;
	char *new_str;
	char *old_cmd;
	char **cmd;
	char ***cmd2;
	t_list	*head;
	t_list	*tmp;

	if ((*tree) && (*tree)->left)
		reset_vars(&(*tree)->left, env);
	if ((*tree) && (*tree)->right)
		reset_vars(&(*tree)->right, env);
	if ((*tree) && (*tree)->command_arr)
	{
		list_size = 0;
		final_len = 0;
		i = 0;
		head = NULL;
		while ((*tree)->command_arr[i])
		{
			if (ft_strchr((*tree)->command_arr[i], '"') || ft_strchr((*tree)->command_arr[i], '\'') || ft_strchr((*tree)->command_arr[i], '$'))
			{
				old_cmd = ft_strdup((*tree)->command_arr[i]);
				head = list_init(old_cmd);
				list_size = lst_size(&head);
				j = 0;
				tmp = head;
				while (tmp)
				{
					in_quotes = 0;
					quote_type = 0;
					j = 0;
					while (tmp->data && tmp->data[j])
					{
						if (!in_quotes && (tmp->data[j] == '"' || tmp->data[j] == '\''))
						{
							in_quotes = 1;
							quote_type = tmp->data[j];
						}
						else if (in_quotes && tmp->data[j] == quote_type)
							in_quotes = 0;
						if (tmp->data[j] == '$' && (!in_quotes || (in_quotes && quote_type == '"')))
						{
							if (tmp->data[j] == '$' && ((in_quotes && tmp->data[j + 1] && (tmp->data[j + 1] == '"' || tmp->data[j + 1] == '\'')) || (!in_quotes && !tmp->data[j + 1])))
								j++;
							else
							{
								process_array_variable(&tmp->data, 0, &j, env);
								j = -1;
							}
						}
						j++;
					}
					tmp = tmp->next;
				}
				tmp = head;
				new_str = ft_strdup(tmp->data);
				free((*tree)->command_arr[i]);
				(*tree)->command_arr[i] = new_str;
				free_list(&head);
			}
			i++;
		}
		list_size = 0;
		final_len = 0;
		i = 0;
		head = NULL;
		while ((*tree)->command_arr[i])
			i++;
		cmd2 = malloc(sizeof(char **) * (i + 1));
		i = 0;
		while ((*tree)->command_arr[i])
		{
			old_cmd = ft_strdup((*tree)->command_arr[i]);
			head = list_init(old_cmd);
			list_size = lst_size(&head);
			if (list_size == 0)
			{
				cmd = malloc(sizeof(char *) * (list_size + 1));
				j = 0;
				tmp = head;
				while (tmp)
				{
					new_str = ft_strdup(tmp->data);
					cmd[j] = new_str;
					j++;
					tmp = tmp->next;
				}
				cmd[j] = NULL;
				cmd2[i] = cmd;
				free_list(&head);
				free(old_cmd);
				i++;
				continue ;
			}
			cmd = malloc(sizeof(char *) * (list_size + 1));
			j = 0;
			tmp = head;
			while (tmp)
			{
				new_str = ft_strdup(tmp->data);
				cmd[j] = new_str;
				j++;
				tmp = tmp->next;
			}
			cmd[j] = NULL;
			cmd2[i] = cmd;
			free_list(&head);
			i++;
		}
		cmd2[i] = NULL;
		i = 0;
		while ((*tree)->command_arr[i])
		{
			free((*tree)->command_arr[i]);
			i++;
		}
		free((*tree)->command_arr);
		m = 0;
		i = 0;
		j = 0;
		while (cmd2[i])
		{
			j = 0;
			while (cmd2[i][j])
			{
				m++;
				j++;
			}
			i++;
		}
		(*tree)->command_arr = malloc(sizeof(char *) * (m + 1));
		i = 0;
		j = 0;
		m = 0;
		while (cmd2[i])
		{
			j = 0;
			while (cmd2[i][j])
			{
				(*tree)->command_arr[m] = ft_strdup(cmd2[i][j]);
				m++;
				j++;
			}
			i++;
		}
		(*tree)->command_arr[m] = NULL;
		i = -1;
		while (cmd2[++i])
		{
			if (cmd2[i])
			{
				j = -1;
				while (cmd2[i][++j])
				{
					if (cmd2[i][j])
					{
						free(cmd2[i][j]);
						cmd2[i][j] = NULL;
					}
				}
				free(cmd2[i]);
				cmd2[i] = NULL;
			}
		}
		(free(cmd2), cmd2 = NULL);
		i = 0;
		while ((*tree)->command_arr[i])
		{
			if (ft_strchr((*tree)->command_arr[i], '"') || ft_strchr((*tree)->command_arr[i], '\''))
			{
				old_cmd = (*tree)->command_arr[i];
				final_len = count_filtered_length(old_cmd, &(*tree)->var);
				new_str = create_filtered_string(old_cmd, final_len);
				if (!new_str)
					return ;
				free((*tree)->command_arr[i]);
				(*tree)->command_arr[i] = new_str;
			}
			i++;
		}
		i = 0;
		j = 0;
		while ((*tree)->command_arr[i])
		{
			j = 0;
			while ((*tree)->command_arr[i][j])
			{
				if ((*tree)->command_arr[i][j] == 10)
					(*tree)->command_arr[i][j] = '"';
				else if ((*tree)->command_arr[i][j] == 11)
					(*tree)->command_arr[i][j] = '\'';
				j++;
			}
			i++;
		}
	}
}
