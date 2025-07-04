/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:03:06 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/04 12:30:53 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_new_handler_pip(int sig)
{
	(void)sig;
	hide_terminal_control_chars();
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	display_terminal_control_chars(void)
{
	struct termios	terminos_p;

	tcgetattr(0, &terminos_p);
	terminos_p.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &terminos_p);
}

void	ft_word_handle_signal(t_tree *tree, int *check)
{
	if (WIFEXITED(tree->status))
		tree->status = WEXITSTATUS(tree->status);
	else if (WIFSIGNALED(tree->status))
	{
		tree->status = WTERMSIG(tree->status);
		if (tree->status == 2)
			g_global_status = SIGINT;
		else if (tree->status == 3)
			g_global_status = SIGQUIT;
		tree->status += 128;
	}
	if (*check)
		exit (tree->status);
}

void	ft_word_handle(t_tree *tree, t_env **h, char **e, int *check)
{
	int	id;

	id = 0;
	display_terminal_control_chars();
	if (!*check)
	{
		id = fork();
		if (id == 0)
			ft_exec(tree, *h, e);
		else if (id < 0)
		{
			perror("fork");
			tree->status = 1;
			return ;
		}
		ft_signal_ign();
	}
	else
		ft_exec(tree, *h, e);
	if (waitpid(id, &(tree->status), 0) == -1)
		tree->status = 1;
	else
		ft_word_handle_signal(tree, check);
}
