/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/06/28 15:05:40 by makkach          ###   ########.fr       */
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
		free(*str);
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
	int			hdoc_num;
	static int	status;
	int			test;

	hdoc_num = 0;
	test = 0;
	tree->status = status;
	hdoc_num = ft_hdoc_count(tree);
	if (hdoc_num > 16)
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
	// ft_free_data(h_data);
}

void	f(){system("leaks -q minishell");}

int	main(int argc, char **argv, char **argev)
{
	t_tree			*tree;
	t_env			*tmp;
	t_env			*env;
	char			*str;
	char			**e;
	// int				*status;
	int				flag;
	int				hdoc_num;
	int				check;
	int				test;
	t_hdoc_data		*h_data;
	struct termios	termios_a;
	char			*temp;

	atexit(f);
;	temp = getcwd(NULL, 0);
	if (!isatty(0) || !isatty(1) || !temp)
		return (1);
	((void)argc, (void)argv, inits_main(&env, &tree, argev));
	e = ft_env_str(env);
	free (temp);
	tmp = NULL;
	tmp = env;
	tree = NULL;
	hdoc_num = 0;
	test = 0;
	h_data = malloc(sizeof(t_hdoc_data));
	if (!h_data)
		return (1);
	h_data->env = &env;
	h_data->sig_flag = malloc(sizeof(int));
	if (!h_data->sig_flag)
	{
		ft_free_array(e);
		free_env(&env);
		free (h_data);
		return (1);
	}
	h_data->check_stat = 0;
	h_data->stat = -1;
	h_data->end = 0;
	tcgetattr(0, &termios_a);
	while (1)
	{
		if (h_data->check_stat)
			h_data->stat = -1;
		*(h_data->sig_flag) = 1;
		global_status = 0;
		check = 0;
		flag = 0;
		hide_terminal_control_chars();
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, SIG_IGN);
		str = readline("minishell$> ");
		if (!str)
		{
			ft_str_empty(&env, e, h_data);
			break ;
		}
		else if (!*str || check_empty(str))
		{
			free(str);
			continue ;
		}
		if (global_status == SIGINT)
		{
			h_data->stat = 1;
			global_status = 0;
		}
		add_history(str);
		tree = NULL;
		ft_parsing(&str, &flag, &tree, h_data);
		if (!flag)
			ft_execution(tree, h_data, &check, e);
		if (flag)
			h_data->check_stat = 0;
		tcsetattr(0, TCSANOW, &termios_a);
		if (tree && flag != 1)
			free_tree(tree);
	}
	// free_env(&env);
	// ft_free_data(h_data);
	if (h_data->stat != -1)
		exit (h_data->stat);
	exit (h_data->end);
}
