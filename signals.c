/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:22:56 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 21:15:28 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	hide_terminal_control_chars(void)
{
	struct termios	terminos_p;

	tcgetattr(STDOUT_FILENO, &terminos_p);
	terminos_p.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &terminos_p);
}

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		hide_terminal_control_chars();
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_redisplay();
	}
}
