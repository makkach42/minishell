/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:22:56 by makkach           #+#    #+#             */
/*   Updated: 2025/07/04 15:31:28 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hide_terminal_control_chars(void)
{
	struct termios	terminos_p;

	tcgetattr(0, &terminos_p);
	terminos_p.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &terminos_p);
}

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_global_status = SIGINT;
	}
}

void	process_regular_element(char ***new_cmd_arr, char *element, int *j)
{
	(*new_cmd_arr)[(*j)++] = ft_strdup(element);
}

void	free_original_cmd_arr(char ***cmd_arr, int original_size)
{
	int	i;

	i = 0;
	while (i < original_size)
		free((*cmd_arr)[i++]);
	free(*cmd_arr);
}

int	if_question_mark(t_tmp_tree **tmp, int n, t_hdoc_data *h_data)
{
	char	*before;
	char	*after;
	char	*num;
	char	*new_str;

	before = ft_substr((*tmp)->tmp->data, 0, n);
	if (!before)
		return (1);
	n++;
	after = ft_substr((*tmp)->tmp->data, n + 1,
			ft_strlen((*tmp)->tmp->data) - (n + 1));
	if (!after)
		return (free(before), 1);
	if (h_data->stat != -1)
		num = ft_itoa(h_data->stat);
	else
		num = ft_itoa((*tmp)->tree->status);
	if (!num)
		return (free(before), free(after), 1);
	new_str = ft_strjoin_three(before, num, after);
	if (!new_str)
		return (free(before), free(after), free(num), 1);
	free((*tmp)->tmp->data);
	(*tmp)->tmp->data = new_str;
	return (free(before), free(after), free(num), 0);
}
