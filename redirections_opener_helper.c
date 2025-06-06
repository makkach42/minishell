/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_opener_helper.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:38:31 by makkach           #+#    #+#             */
/*   Updated: 2025/05/26 11:21:44 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	if_env_value(t_list *tmp, char **env_value)
// {
// 	char	*tmp_char;

// 	tmp_char = tmp->data;
// 	tmp->data = *env_value;
// 	free(tmp_char);
// }

void	init_list_fd_node(t_list_fd *node)
{
	node->name = NULL;
	node->command = NULL;
	node->redir = NULL;
	node->next = NULL;
	node->in_quotes = 0;
	node->name_split = NULL;
	node->fd = -1;
}
