/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:26:59 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/06 10:27:19 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_env	*ft_check(t_env *h, char *str)
{
	if (!h || !str)
		return (NULL);
	while (h != NULL)
	{
		if (ft_strcmp(h->key, str) == 0)
			return (h);
		h = h->next;
	}
	return (NULL);
}

int	ft_parse(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	if (ft_isalpha(s[0]) != 0 && s[0] != '_')
	{
		return (1);
	}
	while (s[i] != '\0')
	{
		if (ft_isalpha(s[i]) != 0 && s[i] != '_' && ft_isdigit(s[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*help;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		new -> next = *lst;
		*lst = new;
		return ;
	}
	help = *lst;
	while (help -> next != NULL)
	{
		help = help -> next;
	}
	help -> next = new;
}

void	if_dollar_two(t_tree *tree, int i, t_hdoc_data *h_data)
{
	int		in_quotes;
	char	quote_type;
	char	c;
	int		j;

	if_dollar_two_inits(&j, &in_quotes);
	while (tree->command_arr[i][++j])
	{
		c = tree->command_arr[i][j];
		if (!in_quotes && (c == '"' || c == '\''))
			set_quote_vars(&in_quotes, &quote_type, c);
		else if (in_quotes && c == quote_type)
			in_quotes = 0;
		if (c == '$' && (!in_quotes || (in_quotes && quote_type == '"')))
		{
			if (if_dollar(&j, &tree->command_arr[i],
					h_data->env, &tree->status))
				break ;
			tree->var = 1;
			in_quotes = 0;
		}
	}
	replace_whites_spaces(tree->command_arr[i]);
	reset_var_cmd_split(&tree);
	reset_var_remove_quotes(&tree);
}

int	ft_var_check_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '"' || str[i] != '\'')
			return (0);
		i++;
	}
	return (1);
}
