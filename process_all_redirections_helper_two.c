/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper_two.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:45:09 by makkach           #+#    #+#             */
/*   Updated: 2025/04/25 14:51:31 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_args_position(char *str, int j)
{
	return (str[j] && str[j] != '>' && str[j] != '<');
}
void	init_args_search(int *i, int *in_quotes, char *quote_type, int *redir_active)
{
	*i = -1;
	*in_quotes = 0;
	*quote_type = 0;
	*redir_active = 0;
}

int	handle_redirection_char(char c, int *redir_active)
{
	if (c == '>' || c == '<')
	{
		*redir_active = 1;
		return (1);
	}
	return (0);
}

int	process_break_point(char *redir_start, int i, int *j, int redir_active)
{
	if (is_args_break_point(redir_start, i, redir_active))
	{
		*j = skip_spaces(redir_start, i);
		if (is_valid_args_position(redir_start, *j))
			return (1);
		return (2);
	}
	return (0);
}

void	update_redir_active(char c, int *redir_active)
{
	if (*redir_active && c != ' ')
		*redir_active = 2;
}
