/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/03/15 19:38:21 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	**helper(char **argv)
// {
// 	int		i;
// 	char	**d;
// 	char	*tmp;
// 	char	*str;

// 	i = 2;
// 	str = ft_strdup(argv[1]);
// 	while (argv[i] != NULL)
// 	{
// 		tmp = str;
// 		str = ft_strjoin(str, " ");
// 		free(tmp);
// 		tmp = str;
// 		str = ft_strjoin(str, argv[i]);
// 		free(tmp);
// 		i++;
// 	}
// 	d = ft_split(str, 32);
// 	free(str);
// 	return (d);
// }

// char	**splitter_joinner(char **argv, int argc)
// {
// 	char	**d;

// 	argc = 0;
// 	d = helper(argv);
// 	return (d);
// }