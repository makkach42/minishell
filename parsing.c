/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/07/05 18:38:29 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}

int	first_inits(t_var_main *shell, char **argev, char **argv, int argc)
{
	shell->temp = getcwd(NULL, 0);
	if (!isatty(0) || !isatty(1) || !shell->temp)
		return (perror(""), 1);
	((void)argc, (void)argv, inits_main(&shell->env, &shell->tree, argev));
	free (shell->temp);
	shell->tree = NULL;
	shell->hdoc_num = 0;
	shell->e = NULL;
	shell->h_data = malloc(sizeof(t_hdoc_data));
	if (!shell->h_data)
		return (1);
	shell->h_data->env = &shell->env;
	shell->h_data->sig_flag = malloc(sizeof(int));
	if (!shell->h_data->sig_flag)
		return (ft_free_array(shell->e),
			free_env(&shell->env), free (shell->h_data), 1);
	shell->h_data->check_stat = 0;
	shell->h_data->stat = -1;
	shell->h_data->end = 0;
	shell->main_stat = -1;
	shell->main_end = 0;
	tcgetattr(0, &shell->termios_a);
	return (0);
}

void	ft_status(t_var_main *shell)
{
	if (shell->tree->status != 130 && shell->tree->status != 131)
		shell->h_data->stat = 1;
}

int	second_inits(t_var_main *shell)
{
	if (shell->h_data->check_stat)
		shell->h_data->stat = -1;
	*(shell->h_data->sig_flag) = 1;
	shell->check = 0;
	shell->flag = 0;
	ft_free_array(shell->e);
	shell->e = ft_env_str(shell->env);
	hide_terminal_control_chars();
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	shell->str = readline("minishell$> ");
	if (g_global_status == SIGINT)
	{
		if (shell->tree)
			ft_status(shell);
		else
			shell->h_data->stat = 1;
	}
	if (!shell->str)
		return (ft_str_empty(shell, &shell->env, shell->e, shell->h_data), 1);
	else if (!*shell->str || check_empty(shell->str))
		return (free(shell->str), 2);
	return (0);
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
			(free_tree(shell.tree), shell.tree = NULL);
	}
	if (shell.main_stat != -1)
		exit (shell.main_stat);
	exit (shell.main_end);
}
