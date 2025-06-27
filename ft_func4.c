/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:38:41 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/27 11:32:16 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	delimiter(char c, char del)
{
	return (c == del);
}

void	ft_free_array(char **p)
{
	int	i;

	i = 0;
	if (!p)
		return ;
	while (p[i] != NULL)
	{
		free (p[i]);
		i++;
	}
	free (p);
}
