/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:10:18 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/17 10:58:25 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_id	*ft_lst_new_id(int x)
{
	t_id *newnode;
	
	newnode = malloc(sizeof(t_id));
	if (!newnode)
		return (NULL);
	newnode->id = x;
	newnode->next = NULL;
	return (newnode);
}

void	ft_lstadd_back2(t_id **lst, t_id *new)
{
	t_id	*help;

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

int	ft_id_creation(t_id **lst, int id1)
{
	t_id	*new1;
	
	new1 = ft_lst_new_id(id1);
	if (!new1)
		return (1);
	ft_lstadd_back2(lst, new1);
	return (0);
}

void	ft_kill(t_id *head)
{
	if (!head)
		return ;
	while (head)
	{
		kill(head->id, SIGKILL);
		head = head->next;
	}
}

void	ft_free_kill(t_id **head)
{
	t_id *tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		free (*head);
		*head = tmp;
	}
	*head = NULL;
}

int	ft_pip(t_tree *tree, t_env **h, char **e, int *check)
{
	int	id1;
	int	id2;
	int	status;
	int	status1;
	static t_id *head;

	status = 0;
	status1 = 0;
	if (pipe(tree->fd) == -1)
		perror("minishell: pipe: ");
	ft_pip_signal();
	id1 = fork();
	if (id1 > 0)
		ft_id_creation(&head, id1);
	if (id1 == -1)
	{
		// ft_kill(head);
		dprintf(2, "entered in the left\n");
		return (perror("minishell: fork: "), 1);
	}
	if (id1 == 0)
		ft_first_child(tree, check, e, h);
	id2 = fork();
	if (id2 > 0)
		ft_id_creation(&head, id2);
	if (id2 == -1)
	{
		dprintf(2, "entered in the right\n");
		// ft_kill(head);
		return (perror("minishell: fork: "), 1);
	}
	if (id2 == 0)
		ft_second_child(tree, check, e, h);
	close (tree->fd[0]);
	close (tree->fd[1]);
	ft_close_fd(tree);
	if (waitpid(id1, &status1, 0) == -1 || waitpid(id2, &(status), 0) == -1)
		return (1);
	if (*check)
	{
		if (WIFSIGNALED(status))
		{
			status = WTERMSIG(status);
			signal(status, SIG_DFL);
			dprintf(2, "entered with this status: %d\n", status);
			kill(0, status);
		}
	}
	status = ft_wait_for_child(status, status1);
	dprintf(2, "entered in pipe with status: %d\n", status);
	if (*check)
		exit (status);
	// if (!*check)
	// {
	// 	ft_free_kill(&head);
	// }
	return (status);
}

int	cmd_check(t_tree *tree)
{
	if (!tree)
		return (1);
	if (!tree->command_arr || !tree->command_arr[0])
		return (1);
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		return (0);
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		return (0);
	return (1);
}

int	ft_cmd_exec(t_tree *tree, t_env **h)
{
	int	status;

	status = 0;
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		status = ft_cd(tree->command_arr, *h);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		status = ft_echo(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		status = ft_env(*h);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		ft_exit(tree->command_arr, h, tree->status);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		status = ft_export(tree->command_arr, *h, tree);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		status = ft_pwd(*h);
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		ft_unset(h, tree->command_arr);
	return (status);
}

int	ft_redir_check(char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (1);
	else if (ft_strcmp(str, ">") == 0)
		return (2);
	else if (ft_strcmp(str, "<<") == 0)
		return (3);
	else if (ft_strcmp(str, ">>") == 0)
		return (4);
	return (-1);
}

void	ft_hdoc_free(char **str, char **limiter, int fd)
{
	free (*str);
	free (*limiter);
	close (fd);
}

void	ft_pip_signal(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
