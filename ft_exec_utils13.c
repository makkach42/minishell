/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils13.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:15:04 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/26 18:31:51 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_hdoc_count_second(t_tree *tree)
{
	t_list_fd	*tmp;
	int			lent;

	lent = 0;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3)
			lent++;
		tmp = tmp->next;
	}
	return (lent);
}

int	ft_hdoc_count(t_tree *tree)
{
	int	lent;
	int	left_lent;
	int	right_lent;

	lent = 0;
	left_lent = 0;
	right_lent = 0;
	if (!tree)
		return (-1);
	if (tree->left)
		left_lent += ft_hdoc_count(tree->left);
	if (tree->right)
		right_lent += ft_hdoc_count(tree->right);
	lent += ft_hdoc_count_second(tree);
	return (lent + left_lent + right_lent);
}

void	ft_free_data(t_hdoc_data *h_data)
{
	free (h_data->sig_flag);
	h_data->sig_flag = NULL;
	free (h_data);
	h_data = NULL;
}

int	if_dollar(int *i, char **line, t_env **env, int *status)
{
	int		n;

	n = *i;
	if (process_array_variable(line, 0, i, env) == -1)
		return (1);
	if (*i == -1)
		if_question_mark_two(line, *status, n);
	*i = -1;
	return (0);
}

void	ft_hdoc_expand(char **line, t_env **env, int status)
{
	int		in_quotes;
	int		i;
	int		n;
	int		flag;
	char	quote_type;

	ft_hdoc_expand_inits(&i, &in_quotes, &flag, &quote_type);
	while ((*line) && (*line)[i])
	{
		if (!in_quotes && ((*line)[i] == '"' || (*line)[i] == '\''))
		{
			in_quotes = 1;
			quote_type = (*line)[i];
		}
		else if ((*line)[i] == quote_type)
			in_quotes = 0;
		if ((*line)[i] == '$' && (
				!in_quotes || (in_quotes && quote_type == '"')))
		{
			if (if_dollar(&i, line, env, &status))
				break ;
		}
		i++;
	}
}
