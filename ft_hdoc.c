/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hdoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:30:26 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/06 11:23:31 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_hdoc_first(t_list_fd *tmp)
{
	int	id;
	int	h_doc;

	h_doc = 0;
	id = fork();
	if (id == 0)
		exit (0);
	if (id != 0)
		h_doc = id;
	waitpid(id, NULL, 0);
	tmp->name_split = ft_split("e", 32);
	tmp->name_split[0] = ft_name_check(tmp->name_split[0], h_doc);
	tmp->fd = open(tmp->name_split[0], O_RDWR | O_CREAT, 0777);
	if (tmp->fd == -1)
		return (1);
	tmp->fd1 = open(tmp->name_split[0], O_RDONLY, 0777);
	if (tmp->fd1 == -1)
		return (1);
	unlink(tmp->name_split[0]);
	return (0);
}

void	ft_hdoc_check_helper(t_list_fd *tmp, t_tree *tree, int *sig_flag)
{
	if (WIFSIGNALED(tree->status))
	{
		tree->status = WTERMSIG(tree->status);
		if (tree->status == 2)
			ft_putstr_fd(1, "\n");
		*sig_flag = 0;
	}
	close (tmp->fd);
}

int	ft_hdoc_child(t_list_fd *tmp, t_env **env, int status, t_tree *tree)
{
	int	id;

	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		ft_hdoc(ft_strdup(tmp->name), tmp, env, status);
		close (tmp->fd1);
		exit (0);
	}
	else if (id < 0)
		return (perror("fork"), 1);
	signal(SIGINT, SIG_IGN);
	waitpid(id, &(tree->status), 0);
	return (id);
}

int	ft_hdoc_process(t_list_fd *tmp, t_tree *tree,
	t_hdoc_data *h_data, int status)
{
	int	id;

	id = 0;
	if (h_data->prev_fd != -1)
		close (h_data->prev_fd);
	if (!ft_hdoc_first(tmp))
	{
		h_data->prev_fd = tmp->fd1;
		if (tmp->fd == -1 || tmp->fd1 == -1)
			return (perror("minishell: "), 1);
		id = ft_hdoc_child(tmp, h_data->env, status, tree);
		if (id == 1)
			return (1);
		ft_hdoc_check_helper(tmp, tree, h_data->sig_flag);
	}
	return (0);
}

void	ft_hdoc_check(t_tree *tree, t_hdoc_data *h_data, int status)
{
	t_list_fd	*tmp;
	int			id;

	h_data->prev_fd = -1;
	id = -1;
	tmp = tree->fd_list;
	while (tmp != NULL)
	{
		if (ft_redir_check(tmp->redir) == 3 && *(h_data->sig_flag) == 1)
		{
			if (ft_strchr(tmp->name, '$'))
				rem_dollar(&tmp->name);
			if (ft_strchr(tmp->name, '\'') || ft_strchr(tmp->name, '"'))
				tmp->in_quotes = 1;
			quote_remove_lst(&tree);
			if (ft_hdoc_process(tmp, tree, h_data, status))
				return ;
		}
		tmp = tmp->next;
	}
}
