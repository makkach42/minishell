/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:20:09 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 16:22:57 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_search(t_list **head)
{
	t_list	*tmp;

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

int	check_for_variable(char *str)
{
	int	i;

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

void	variable_expantion(t_list **head, char **ev)
{
	t_list	*tmp;
	int		i;
	char	*variable_name;
	char	*tmp_char;

	tmp = *head;
	i = 0;
	variable_name = NULL;
	while (tmp && ft_strcmp(tmp->token, "VARIABLE"))
		tmp = tmp->next;
	if (tmp && tmp->prev && (ft_strcmp(tmp->prev->data, "<<"))) //in double quotes
	{
		variable_name = ft_substr(
				tmp->data, 1, ft_strlen(
					tmp->data) - 1);
		while (ev[i])
		{
			if (!ft_strncmp(ev[i], variable_name, ft_strlen(
						variable_name)) && ev[i][
				ft_strlen(variable_name)] == '=')
			{
				tmp_char = tmp->data;
				tmp->data = ft_substr(ev[i], ft_strlen(
							variable_name), ft_strlen(
							ev[i]) - ft_strlen(
							variable_name));
				free(tmp_char);
				tmp_char = tmp->data;
				tmp->data = ft_strtrim(tmp->data, "=");
				free(tmp_char);
			}
			i++;
		}
		free(variable_name);
	}
}

int	variable_in_word(t_list **head, char **argev)
{
	t_list	*tmp;
	int		i;
	int		j;
	int		k;
	int		l;
	char	*new_str;
	char	*tmp_word;
	char	*first_part;
	char	*tmp_char;
	char	*second_part;
	char	quote_type;
	int		in_quote;

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
					if (in_quote == 0 && (
							tmp->data[i] == '"' || tmp->data[i] == '\''))
					{
						quote_type = tmp->data[i];
						in_quote = 1;
					}
					else if (in_quote && tmp->data[i] == quote_type)
					{
						in_quote = 0;
						quote_type = '\0';
					}
					if ((tmp->data[i] == '$' && (
								in_quote && quote_type == '"')) || (
							tmp->data[i] == '$' && !in_quote))
					{
						j = i;
						l = i;
						j++;
						if ((tmp->data[j] >= '0' && tmp->data[j] <= '9'
							) || !tmp->data[j])
							return (-1);
						while ((tmp->data[j] >= 'a' && tmp->data[j] <= 'z') || (
								tmp->data[j] >= 'A' && tmp->data[j] <= 'Z') || (
								tmp->data[j] >= '0' && tmp->data[j] <= '9'))
							j++;
						l++;
						tmp_char = tmp_word;
						tmp_word = ft_substr(tmp->data, l, j - l);
						free(tmp_char);
						k = 0;
						while (argev[k] && ft_strncmp(
								argev[k], tmp_word, ft_strlen(tmp_word)))
							k++;
						if (!argev[k])
						{
							free(tmp_word);
							tmp_char = first_part;
							first_part = ft_substr(
									tmp->data, 0, i);
							free(tmp_char);
							tmp_char = second_part;
							second_part = ft_substr(
									tmp->data, j, ft_strlen(
										tmp->data) - j);
							free(tmp_char);
							tmp_char = new_str;
							new_str = ft_strjoin(
									first_part, second_part);
							free(tmp_char);
							free(tmp->data);
							tmp->data = new_str;
						}
						else
						{
							free(tmp_word);
							l = 0;
							while (argev[k][l] && argev[k][l] != '=')
								l++;
							tmp_char = tmp_word;
							tmp_word = ft_substr(
									argev[k], l, ft_strlen(argev[k]) - l);
							free(tmp_char);
							tmp_char = tmp_word;
							tmp_word = ft_strtrim(
									tmp_word, "=");
							free(tmp_char);
							tmp_char = first_part;
							first_part = ft_substr(
									tmp->data, 0, i);
							free(tmp_char);
							tmp_char = second_part;
							second_part = ft_substr(
									tmp->data, j, ft_strlen(
										tmp->data) - j);
							free(tmp_char);
							tmp_char = new_str;
							new_str = ft_strjoin(
									first_part, tmp_word);
							free(tmp_char);
							tmp_char = new_str;
							new_str = ft_strjoin(
									new_str, second_part);
							free(tmp_char);
							free(tmp->data);
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
