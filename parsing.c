/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 09:34:44 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_arr_fill_helper(t_tree **tree, char **str)
{
	t_list	*head;
	char	**arr;
	t_list	*tmp;
	int		i;
	int		list_size;

	i = 0;
	head = list_init(*str);
	list_size = lst_size(&head);
	arr = malloc(sizeof(char *) * (list_size + 1));
	tmp = head;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->data);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	free_list(&head);
	(*tree)->command_arr = arr;
}

void	command_arr_fill(t_tree **tree)
{
	char	*str;
	char	*tmp;
	char	**arr;

	if ((*tree)->left)
		command_arr_fill(&(*tree)->left);
	if ((*tree)->right)
		command_arr_fill(&(*tree)->right);
	if ((tree) && (*tree) && (*tree)->command && !(*tree)->command_arr)
	{
		str = ft_strdup((*tree)->command);
		if (!str || !*str)
		{
			arr = malloc(sizeof(char *));
			if (arr)
				arr[0] = NULL;
			(*tree)->command_arr = arr;
			free(str);
			return ;
		}
		tmp = str;
		str = ft_strtrim(str, " ");
		free(tmp);
		command_arr_fill_helper(tree, &str);
	}
}

void	env_fill_helper(t_env **node, int *i, int *j, char **argev)
{
	*node = malloc(sizeof(t_env));
	while (argev[*i] && argev[*i][*j] != '=')
		(*j)++;
	(*node)->key = ft_substr(argev[*i], 0, *j);
	(*node)->value = ft_substr(argev[*i],
			(*j + 1), ft_strlen(argev[*i]) - (*j + 1));
	*j = 0;
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
	env_fill_helper(&head, &i, &j, argev);
	head->next = NULL;
	tmp = head;
	i++;
	while (argev[i])
	{
		env_fill_helper(&new_node, &i, &j, argev);
		new_node->next = NULL;
		tmp->next = new_node;
		tmp = new_node;
		i++;
	}
	return (head);
}

int	ft_exec(t_tree *tree, t_env *h)
{
	char	*temp;
	t_env *path;
	char *env;
	// t_pip *value;

	path = ft_check(h, "PATH");
	if (path == NULL)
		exit (1);
	env = path->value; 
	// if (tree->redirections== NULL)
	// {
		temp = tree->command_arr[0];
		tree->command_arr[0] =  ft_cmd_check(env, tree->command_arr[0]);
		execve(tree->command_arr[0], tree->command_arr, NULL);
		printf("execve failed\n");
	// }
}

int	ft_execute(t_tree *tree, t_env *h, t_list_fd *head_fd);

int	ft_pip(t_tree *tree, t_env *h, t_list_fd *head_fd)
{
	int	fd[2];
	int	id;
	int id2;
	int	status;
	
	status = 0;
	if (pipe(fd) == -1)
		perror("minishell: pipe: ");
	if (tree->left)
	{
		id = fork();
		if (id == -1)
			perror("minishell: fork: ");
		if (id == 0)
		{
			close (fd[0]);
			dup2(fd[1], 1);
			close (fd[1]);
			exit (ft_execute(tree->left, h, head_fd));
		}
	}
	if (tree->right)
	{
		id2 = fork();
		if (id2 == -1)
			perror("minishell: fork: ");
		if (id2 == 0)
		{
			close (fd[1]);
			dup2(fd[0], 0);
			close (fd[0]);
			exit(ft_execute(tree->right, h, head_fd));
		}
	}
	close (fd[0]);
	close (fd[1]);
	waitpid(id, NULL, 0);
	waitpid(id2, &status, 0);
	return (status);
}

void	ft_cmd_exec(t_tree *tree, t_env *h)
{
	if (ft_strcmp(tree->command_arr[0], "cd") == 0)
		ft_cd(tree->command_arr, h);
	if (ft_strcmp(tree->command_arr[0], "echo") == 0)
		ft_echo(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "env") == 0)
		ft_env(h);
	if (ft_strcmp(tree->command_arr[0], "exit") == 0)
		ft_exit(tree->command_arr);
	if (ft_strcmp(tree->command_arr[0], "export") == 0)
		ft_export(tree->command_arr, h);
	if (ft_strcmp(tree->command_arr[0], "pwd") == 0)
		ft_pwd();
	if (ft_strcmp(tree->command_arr[0], "unset") == 0)
		ft_unset(h, tree->command_arr);
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

int	ft_exec_redir(t_tree *tree, t_env *h, t_list_fd *head_fd)
{

	if (ft_redir_check(head_fd->redir) == 1)
	{
			while (head_fd->next != NULL)
			{
				head_fd->fd = ft_file_check(head_fd->name);
				if (head_fd ->fd == -1)
				{
					perror("minishell: ");
					return (-1);
				}
				head_fd = head_fd->next;
			}
			head_fd->fd = ft_file_check(head_fd->name);
			dup2(head_fd->fd, 0);
			ft_exec(tree, h);
	}
	else if (ft_redir_check(head_fd->redir) == 2)
	{
		while (head_fd -> next != NULL)
		{
			head_fd->fd = ft_file_create(head_fd->name);
			head_fd = head_fd->next;
		}
		dprintf(2, "this is output fd %d\n", head_fd->fd);
		head_fd->fd = ft_file_create(head_fd->name);
		dup2(head_fd->fd, 1);
		ft_exec(tree, h);
	}
	dprintf(2,"this is redir type %s\n", head_fd->redir);
	
}

int	ft_execute(t_tree *tree, t_env *h, t_list_fd *head_fd)
{
	int id;
	int	status;
	int	check;
	int	redir;

	status = 0;
	check = 0;
	if (ft_strcmp("WORD", tree->type) == 0)
	{
		id = fork();
		if (id == 0)
		{
			ft_exec(tree, h);
		}
		waitpid(id, &status, 0);
	}
	if (ft_strcmp("REDIRECTION", tree->type) == 0)
	{
		id = fork();
		if (id == 0)
		{
			ft_exec_redir(tree, h, head_fd);
		}
		waitpid(id, &status, 0);
	}
	if (ft_strcmp("COMMAND", tree->type) == 0)
	{
		ft_cmd_exec(tree, h);
	}
	if (ft_strcmp("PIPE", tree->type) == 0)
	{
		ft_pip(tree, h, head_fd);	
	}
	return (status);
}

// void	f(void)
// {
// 	system("leaks -q minishell");
// }

int	main(int argc, char **argv, char **argev)// expand variable if herdoc and in double quotes
{
	char		*str;
	t_env		*env;
	t_tree		*tree;
	t_list_fd	*head_fd;
	t_list_fd	*tmp;

	// atexit(f);
	(void)argc;
	(void)argv;
	inits_main(&head_fd, &env, &tree);
	while (1)
	{
		str = readline("minishell$> ");
		if (!str)
			break ;
		else if (!*str)
		{
			free(str);
			continue ;
		}
		add_history(str);
		env_fill_quote_parse(&env, &str, argev);
		lexer_to_tree(str, &tree, argev);
		tree_to_rediropen(tree);
		// printf("this is type: %s\n", tree->redirections);
		// print_tree_visual(tree, 1, 1);
		redirections_opener(&tree, &head_fd);
		// printf("this is type redirection: %s\n", tree->redirections);
		// print_tree_visual(tree, 1, 1);
		// tmp = head_fd;
		// while (tmp)
		// {
		// 	printf("command = %s\n", tmp->command);
		// 	printf("name = %s\n", tmp->name);
		// 	printf("redir = %s\n", tmp->redir);
		// 	printf("fd = %d\n", tmp->fd);
		// 	printf("\n");
		// 	tmp = tmp->next;
		// }
		ft_execute(tree, env, head_fd);
		lasfree(&env, &tree, &head_fd);
	}
}
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt
//((ls > file) && echo | la > file2) > file3

//((ls)>file2) > file
// "((ls)>file2) > file"