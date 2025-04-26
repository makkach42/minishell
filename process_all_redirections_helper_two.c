/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_redirections_helper_two.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:16:49 by makkach           #+#    #+#             */
/*   Updated: 2025/04/26 14:44:13 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_cmd_with_args(char **cmd_part, char *args_start,
			char *redir_start)
{
	int		redir_len;
	char	*tmp;
	char	*redir_part;

	redir_len = args_start - redir_start;
	redir_part = ft_substr(redir_start, 0, redir_len);
	tmp = *cmd_part;
	if (tmp[0] != '\0')
		*cmd_part = ft_strjoin(tmp, " ");
	else
		*cmd_part = ft_strdup("");
	free(tmp);
	tmp = *cmd_part;
	*cmd_part = ft_strjoin(tmp, args_start);
	free(tmp);
	free(redir_part);
}

int	count_filtered_length(char *old_str)
{
	int	i;
	int	len;
	int	final_len;
	int	in_single_quotes;
	int	in_double_quotes;

	len = ft_strlen(old_str);
	final_len = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	i = 0;
	while (i < len)
	{
		if (old_str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (old_str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
			final_len++;
		i++;
	}
	return (final_len);
}
