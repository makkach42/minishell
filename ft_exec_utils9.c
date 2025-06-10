#include <minishell.h>

void	*env_fill_pwd(t_env *head)
{
	t_env	*tmp;

	tmp = ft_check(head, "OLDPWD");
	if (tmp)
	{
		tmp->active = 1;
		(free(tmp->value), tmp->value = NULL);
	}
	ft_lstadd_back(&head, ft_lstnew("1PWD", NULL));
	tmp = ft_check(head, "1PWD");
	if (tmp)
	{
		tmp->active = 1;
		tmp->h = 1;
	}
}

t_env	*env_fill(char **argev)
{
	int		i;
	int		j;
	t_env	*head;
	t_env	*new_node;
	t_env	*tmp;

	i = 0;
	j = 0;
	if (!argev || !*argev)
		env_fill_empty(&head, &i, &j);
	else
	{
		env_fill_helper(&head, &i, &j, argev);
		head->next = NULL;
		head->h = 0;
		head->active = 0;
		tmp = head;
		i++;
		while (argev[i])
		{
			env_fill_helper(&new_node, &i, &j, argev);
			new_node->next = NULL;
			tmp->next = new_node;
			tmp = new_node;
			tmp->active = 0;
			tmp->h = 0;
			i++;
		}
		env_fill_pwd(head);
	}
	return (head);
}

void	ft_close_handle(t_list_fd *head)
{
	if (!head)
		return ;
	while (head)
	{
		close (head->fd1);
		head = head->next;
	}
}

void	ft_first_child(t_tree *tree, int *check, char **e, t_env **h)
{
	signal(SIGINT, SIG_DFL);
	*check = 1;
	dup2(tree->fd[1], 1);
	close (tree->fd[1]);
	close (tree->fd[0]);
	ft_execute(tree->left, h, e, check);
	exit (tree->status);
}

void	ft_second_child(t_tree *tree, int *check, char **e, t_env **h)
{
	signal(SIGINT, SIG_DFL);
	*check = 1;
	dup2(tree->fd[0], 0);
	close (tree->fd[0]);
	close (tree->fd[1]);
	ft_execute(tree->right, h, e, check);
	exit(tree->status);
}

int	ft_wait_for_child(int status, int status1)
{
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		if (status == 2)
			global_status = SIGINT;
		else if (status == 3)
			global_status = SIGQUIT;
		status += 128;
	}
	else if (WIFSIGNALED(status1))
	{
		status1 = WTERMSIG(status1);
		if (status1 == 2)
			global_status = SIGINT;
		else if (status1 == 3)
			global_status = SIGQUIT;
	}
	else
		status = WEXITSTATUS(status);
	return (status);
}

