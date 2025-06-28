/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/06/28 17:48:20 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	protect_wild_card(t_tree **tree)
{
	int		i;
	char	*tmp;

	if ((*tree) && (*tree)->left)
		protect_wild_card(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		protect_wild_card(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		i = 1;
		while ((*tree)->command_arr[i])
		{
			if (if_has_wildcards((*tree)->command_arr[i]))
			{
				tmp = (*tree)->command_arr[i];
				(*tree)->command_arr[i] = ft_strjoin_three("\"",
						(*tree)->command_arr[i], "\"");
				free(tmp);
			}
			i++;
		}
	}
}

void	ft_parsing(char **str, int *flag, t_tree **tree, t_hdoc_data *h_data)
{
	quote_parse(str, flag);
	if (*flag)
		(free(*str), h_data->stat = 258);
	if (!*flag)
	{
		lexer_to_tree(*str, tree, flag, h_data);
		tree_to_rediropen(*tree, flag);
		export_cases(tree);
		redirections_list_maker(tree);
	}
	if (!*flag && has_wild_cards_comarr(tree) == 1)
		handle_wildcards_in_cmdarr(tree);
	if (!*flag && has_wild_cards_fdlst(tree) == 1)
		handle_wildcards_in_fdlst(tree);
	if (!*flag && has_wild_cards_comarr(tree))
		protect_wild_card(tree);
}

void	ft_execution(t_tree *tree, t_hdoc_data *h_data, int *check, char **e)
{
	static int	status;
	int			test;

	h_data->hdoc_num = 0;
	test = 0;
	tree->status = status;
	h_data->hdoc_num = ft_hdoc_count(tree);
	if (h_data->hdoc_num > 16)
	{
		ft_putstr_fd(2, "minishell: maximum here-document count exceeded\n");
		exit (2);
	}
	ft_hdoc_handle(tree, h_data, tree->status);
	ft_st(tree, *(h_data->sig_flag));
	ft_exec_test(tree, &test, h_data);
	if (*(h_data->sig_flag))
	{
		ft_execute(tree, e, check, h_data);
		ft_signal_exec();
	}
	else
		tree->status = 1;
	if (tree->status > 0)
		h_data->check_stat = 1;
	else
		h_data->check_stat = 0;
	status = tree->status;
	h_data->end = status;
}

void	ft_str_empty(t_env **env, char **e, t_hdoc_data *h_data)
{
	ft_putstr_fd(1, "exit\n");
	ft_free_array(e);
	free_env(env);
	ft_free_data(h_data);
}

int	first_inits(t_var_main *shell, char **argev, char **argv, int argc)
{
	shell->temp = getcwd(NULL, 0);
	if (!isatty(0) || !isatty(1) || !shell->temp)
		return (1);
	((void)argc, (void)argv, inits_main(&shell->env, &shell->tree, argev));
	shell->e = ft_env_str(shell->env);
	free (shell->temp);
	shell->tree = NULL;
	shell->hdoc_num = 0;
	shell->h_data = malloc(sizeof(t_hdoc_data));
	if (!shell->h_data)
		return (1);
	shell->h_data->env = &shell->env;
	shell->h_data->sig_flag = malloc(sizeof(int));
	if (!shell->h_data->sig_flag)
	{
		ft_free_array(shell->e);
		free_env(&shell->env);
		free (shell->h_data);
		return (1);
	}
	shell->h_data->check_stat = 0;
	shell->h_data->stat = -1;
	shell->h_data->end = 0;
	tcgetattr(0, &shell->termios_a);
	return (0);
}

int	second_inits(t_var_main *shell)
{
	if (shell->h_data->check_stat)
		shell->h_data->stat = -1;
	*(shell->h_data->sig_flag) = 1;
	global_status = 0;
	shell->check = 0;
	shell->flag = 0;
	shell->e = ft_env_str(shell->env);
	hide_terminal_control_chars();
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	shell->str = readline("minishell$> ");
	if (!shell->str)
	{
		ft_str_empty(&shell->env, shell->e, shell->h_data);
		return (1);
	}
	else if (!*shell->str || check_empty(shell->str))
		return (free(shell->str), 2);
	if (global_status == SIGINT)
	{
		shell->h_data->stat = 1;
		shell->h_data->end = 1;
		global_status = 0;
	}
	return (0);
}

void	execution(t_var_main *shell)
{
	shell->tree = NULL;
	ft_parsing(&shell->str, &shell->flag, &shell->tree, shell->h_data);
	if (!shell->flag)
		ft_execution(shell->tree, shell->h_data, &shell->check, shell->e);
	if (shell->flag)
		shell->h_data->check_stat = 0;
	tcsetattr(0, TCSANOW, &shell->termios_a);
}

int	main(int argc, char **argv, char **argev)
{
	t_var_main	shell;
	int			i;

	if (first_inits(&shell, argev, argv, argc))
		return (1);
	while (1)
	{
		i = second_inits(&shell);
		if (i == 1)
			break ;
		if (i == 2)
			continue ;
		add_history(shell.str);
		execution(&shell);
		if (shell.tree && shell.flag != 1)
			free_tree(shell.tree);
	}
	if (shell.h_data->stat != -1)
		exit (shell.h_data->stat);
	exit (shell.h_data->end);
}
