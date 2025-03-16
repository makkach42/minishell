/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:44:26 by makkach           #+#    #+#             */
/*   Updated: 2025/03/16 12:47:27 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(char *s1)
{
	int		i;
	char	*j;

	i = 0;
	while (s1[i] != '\0')
		i++;
	i += 1;
	j = (char *)malloc(i * sizeof(char));
	if (!j)
		return (0);
	i = 0;
	while (s1[i] != '\0')
	{
		j[i] = s1[i];
		i++;
	}
	j[i] = '\0';
	return (j);
}
