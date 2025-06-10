/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:17:14 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/10 16:18:04 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_name_check(char *name)
{
	int		i;
	char	*add;
	char	*tmp;

	i = 0;
	add = NULL;
	tmp = NULL;
	if (name == NULL)
		return (NULL);
	while (access(name, F_OK) == 0)
	{
		tmp = name;
		add = ft_itoa(i);
		if (!add)
			return (NULL);
		name = ft_strjoin(name, add);
		if (!name)
			return (NULL);
		free (tmp);
		i++;
	}
	return (name);
}

void	ft_hdoc_expand(char **line, t_env **env, int status)
{
	int		in_quotes;
	int		i;
	int		n;
	int		flag;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	flag = 0;
	quote_type = 0;
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
			n = i;
			if (process_array_variable(line, 0, &i, env) == -1)
				break ;
			if (i == -1)
				if_question_mark_two(line, status, n);
			i = -1;
		}
		i++;
	}
}

void	ft_hdoc(char *limiter, int fd, t_env **env, int status)
{
	char	*str;
	char	*tmp;

	str = readline("> ");
	while (str != NULL && ft_strcmp(limiter, str) != 0)
	{
		ft_hdoc_expand(&str, env, status);
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		tmp = str;
		str = readline("> ");
		free(tmp);
	}
	ft_hdoc_free(&str, &limiter, fd);
}

void	ft_exec_redir(t_tree *tree, t_env **h, char **env)
{
	while (tree->fd_list != NULL && tree->ambiguous == 0)
	{
		if (ft_redir_check(tree->fd_list->redir) == 3)
		{
			dup2(tree->fd_list->fd1, 0);
			close (tree->fd_list->fd1);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 2)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 1);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return ;
			}
			dup2(tree->fd_list->fd, 1);
			close (tree->fd_list->fd);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 1)
		{
			tree->fd_list->fd = ft_file_check(tree->fd_list->name);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return ;
			}
			dup2(tree->fd_list->fd, 0);
			close (tree->fd_list->fd);
		}
		else if (ft_redir_check(tree->fd_list->redir) == 4)
		{
			tree->fd_list->fd = ft_file_create(tree->fd_list->name, 2);
			if (tree->fd_list ->fd == -1)
			{
				perror("minishell: ");
				return ;
			}
			dup2(tree->fd_list->fd, 1);
			close (tree->fd_list->fd);
		}
		tree->fd_list = tree->fd_list->next;
	}
	if (tree->ambiguous == 1)
		return ;
	// if (tree->command_arr[0] && check_empty(tree->command_arr[0]) == 0)
	// 	ft_exec(tree, *h, env);
}
