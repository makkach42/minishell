/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:10:18 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/05 10:21:51 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_pip(t_tree *tree)
{
	close (tree->fd[0]);
	close (tree->fd[1]);
}

int	ft_c(t_tree *tree, t_pip *x)
{
	ft_close_pip(tree);
	ft_close_fd(tree);
	if (waitpid(x->id1, &(x->status1), 0) == -1
		|| waitpid(x->id2, &(x->status), 0) == -1)
		return (1);
	return (0);
}

int	ft_pip(t_tree *tree, t_hdoc_data *h_data, char **e, int *check)
{
	t_pip	*x;
	int		status;

	x = NULL;
	status = 0;
	if (!ft_test_x(&x))
		return (1);
	tree->left->status = tree->status;
	if (pipe(tree->fd) == -1)
		perror("minishell: pipe: ");
	ft_signal_ign();
	x->id1 = fork();
	if (x->id1 == -1)
		return (perror("minishell: fork: "), 1);
	if (x->id1 == 0)
		ft_first_child(tree, check, e, h_data);
	x->id2 = fork();
	if (x->id2 == -1)
		return (perror("minishell: fork: "), 1);
	if (x->id2 == 0)
		ft_second_child(tree, check, e, h_data);
	if (ft_c(tree, x))
		return (1);
	return (ft_wait_for_child(x->status, x->status1,
			check, &status), free (x), status);
}

// void	remove_quotes_from_var_two(char	**arr)
// {
// 	int		i;
// 	char	*old_cmd;
// 	int		final_len;
// 	char	*new_str;

// 	i = 0;
// 	if (arr[i])
// 	{
// 		if (ft_strchr(arr[i], '"'
// 			) || ft_strchr(
// 				arr[i], '\''))
// 		{
// 			old_cmd = arr[i];
// 			final_len = count_filtered_length(old_cmd);
// 			new_str = create_filtered_string(old_cmd, final_len);
// 			if (!new_str)
// 				return ;
// 			free(arr[i]);
// 			arr[i] = new_str;
// 		}
// 	}
// }

// void	get_var_name_in_str(char **str, int *j)
// {
// 	while ((*str)[*j] == '$')
// 		(*j)++;
// 	while ((*str)[*j])
// 	{
// 		if (((*str)[*j] >= 'a' && (*str)[*j] <= 'z') || (
// 			(*str)[*j] >= 'A' && (*str)[*j] <= 'Z'
// 			) || (((*str)[*j] >= '0' && (*str)[*j] <= '9')
// 			) || (*str)[*j] == '?')
// 		{
// 			if ((*str)[*j] >= '0' && (*str)[*j] <= '9' && (*str)[*j] == '$')
// 			{
// 				(*j)++;
// 				break ;
// 			}
// 			if ((*str)[*j] == '?' && (*str)[*j - 1] != '$')
// 				break ;
// 			else
// 				(*j)++;
// 		}
// 		else
// 			break ;
// 	}
// }

// int	expand_str_if_dollar(char **str, int *i, t_rm_dollar *s)
// {
// 	int			n;
// 	int			j;

// 	n = *i;
// 	j = *i;
// 	get_var_name_in_str(str, &j);
// 	s->before = ft_substr((*str), 0, n);
// 	if (!s->before)
// 		return (-1);
// 	s->after = ft_substr((*str), j, ft_strlen((*str)) - j);
// 	if (!s->after)
// 		return (free(s->before), -1);
// 	s->new_str = ft_strjoin(s->before, s->after);
// 	if (!s->new_str)
// 		return (free(s->before), free(s->after), -1);
// 	free((*str));
// 	(*str) = s->new_str;
// 	free(s->before);
// 	free(s->after);
// 	return (0);
// }

int	expand_str(char **str)
{
	int			i;
	t_rm_dollar	s;
	t_qfilter	t;

	i = 0;
	t.in_quotes = 0;
	while ((*str)[i])
	{
		if (!t.in_quotes && ((*str)[i] == '\'' || (*str)[i] == '"'))
			set_quote_vars(&t.in_quotes, &t.quote_type, (*str)[i]);
		else if (t.in_quotes && (*str)[i] == t.quote_type)
			t.in_quotes = 0;
		if ((*str)[i] == '$' && (!t.in_quotes || (
				t.in_quotes && t.quote_type == '"')))
		{
			if (expand_str_if_dollar(str, &i, &s) == -1)
				break ;
		}
		if (i < (int)ft_strlen((*str)))
			i++;
	}
	return (0);
}

int	cmd_check(t_tree *tree)
{
	char	*str;

	if (!tree)
		return (1);
	if (!tree->command_arr || !tree->command_arr[0])
		return (1);
	str = ft_strdup(tree->command_arr[0]);
	expand_str(&str);
	remove_quotes_from_var_two(&str);
	if (ft_strcmp(str, "cd") == 0)
		return (free(str), 0);
	if (ft_strcmp(str, "echo") == 0)
		return (free(str), 0);
	if (ft_strcmp(str, "env") == 0)
		return (free(str), 0);
	if (ft_strcmp(str, "exit") == 0)
		return (free(str), 0);
	if (ft_strcmp(str, "export") == 0)
		return (free(str), 0);
	if (ft_strcmp(str, "pwd") == 0)
		return (free(str), 0);
	if (ft_strcmp(str, "unset") == 0)
		return (free(str), 0);
	return (free(str), 1);
}

// int	ft_cmd_exec(t_tree *tree, t_hdoc_data *h_data)
// {
// 	int	status;

// 	status = 0;
// 	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
// 		status = ft_cd(tree->command_arr, *(h_data->env), tree->var);
// 	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
// 		status = ft_echo(tree->command_arr);
// 	if (ft_strcmp(tree->command_arr[0], "env") == 0)
// 		status = ft_env(*(h_data->env));
// 	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
// 		status = ft_exit(tree->command_arr, h_data, tree->status);
// 	if (ft_strcmp(tree->command_arr[0], "export") == 0)
// 		status = ft_export(tree->command_arr, *(h_data->env), tree);
// 	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
// 		status = ft_pwd(*(h_data->env));
// 	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
// 		status = ft_unset(h_data->env, tree->command_arr);
// 	return (status);
// }

// int	ft_redir_check(char *str)
// {
// 	if (ft_strcmp(str, "<") == 0)
// 		return (1);
// 	else if (ft_strcmp(str, ">") == 0)
// 		return (2);
// 	else if (ft_strcmp(str, "<<") == 0)
// 		return (3);
// 	else if (ft_strcmp(str, ">>") == 0)
// 		return (4);
// 	return (-1);
// }
