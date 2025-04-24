/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:20:09 by makkach           #+#    #+#             */
/*   Updated: 2025/04/24 17:02:21 by makkach          ###   ########.fr       */
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
	if (tmp && tmp->prev && (ft_strcmp(tmp->prev->data, "<<")))//in double quotes
	{
		variable_name = ft_substr_leak(
				tmp->data, 1, ft_strlen(
					tmp->data) - 1, 2421);
		while (ev[i])
		{
			if (!ft_strncmp(ev[i], variable_name, ft_strlen(
						variable_name)) && ev[i][
				ft_strlen(variable_name)] == '=')
			{
				tmp_char = tmp->data;
				tmp->data = ft_substr_leak(ev[i], ft_strlen(
							variable_name), ft_strlen(
							ev[i]) - ft_strlen(
							variable_name), 2428);
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
						tmp_word = ft_substr_leak(tmp->data, l, j - l, 2506);
						t_free(tmp_char, 2227, "parsing.c");
						k = 0;
						while (argev[k] && ft_strncmp(
								argev[k], tmp_word, ft_strlen(tmp_word)))
							k++;
						if (!argev[k])
						{
							t_free(tmp_word, 2233, "parsing.c");
							tmp_char = first_part;
							first_part = ft_substr_leak(
									tmp->data, 0, i, 2515);
							t_free(tmp_char, 2236, "parsing.c");
							tmp_char = second_part;
							second_part = ft_substr_leak(
									tmp->data, j, ft_strlen(
										tmp->data) - j, 2518);
							t_free(tmp_char, 2239, "parsing.c");
							tmp_char = new_str;
							new_str = ft_strjoin_leak(
									first_part, second_part, 2501);
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
							tmp_word = ft_substr_leak(
									argev[k], l, ft_strlen(argev[k]) - l, 2533);
							t_free(tmp_char, 2254, "parsing.c");
							tmp_char = tmp_word;
							tmp_word = ft_strtrim_leak(
									tmp_word, "=", 2252, "list_init");
							t_free(tmp_char, 2257, "parsing.c");
							tmp_char = first_part;
							first_part = ft_substr_leak(
									tmp->data, 0, i, 2539);
							t_free(tmp_char, 2260, "parsing.c");
							tmp_char = second_part;
							second_part = ft_substr_leak(
									tmp->data, j, ft_strlen(
										tmp->data) - j, 2542);
							t_free(tmp_char, 2263, "parsing.c");
							tmp_char = new_str;
							new_str = ft_strjoin_leak(
									first_part, tmp_word, 2525);
							t_free(tmp_char, 2266, "parsing.c");
							tmp_char = new_str;
							new_str = ft_strjoin_leak(
									new_str, second_part, 2528);
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
