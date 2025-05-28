/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wld_cards_helper_three.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:15:23 by makkach           #+#    #+#             */
/*   Updated: 2025/05/28 15:16:14 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pattern_match_inits(int *i, int *j, int *star_idx, int *str_idx)
{
	*i = 0;
	*j = 0;
	*star_idx = -1;
	*str_idx = -1;
}

int	skip_stars(int *i, const char *pattern)
{
	while (pattern[(*i) + 1] == '*')
		(*i)++;
	if (!pattern[(*i) + 1])
		return (1);
	return (0);
}

void	update_vars(int *star_idx, int *i, int *str_idx, int *j)
{
	*star_idx = (*i)++;
	*str_idx = *j;
}

void	back_track(int *i, int *j, int *star_idx, int *str_idx)
{
	*i = (*star_idx) + 1;
	*j = ++(*str_idx);
}

void	update_vars_two(int *i, int *j)
{
	(*i)++;
	(*j)++;
}
