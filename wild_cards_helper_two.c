/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_cards_helper_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:16:23 by makkach           #+#    #+#             */
/*   Updated: 2025/06/29 19:17:13 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_quote(char *pattern, int *i, char *quote_type, int *quote_idx)
{
	*quote_type = pattern[(*i)];
	*quote_idx = (*i);
	(*i)++;
}

void	quote_filter(char *pattern, int *i, t_qfilter *qfil)
{
	if (!qfil->in_quotes && (pattern[(*i)] == '"' || pattern[(*i)] == '\''))
		(if_quote(pattern, i, &qfil->quote_type,
				&qfil->quote_idx), qfil->in_quotes = 1);
	else if (qfil->in_quotes && pattern[(*i
			)] == qfil->quote_type && (*i) != qfil->quote_idx)
	{
		qfil->in_quotes = 0;
		(*i)++;
	}
}

void	pattern_is_string(int *i, int *j, char *pattern, t_qfilter *qfil)
{
	update_vars_two(i, j);
	if (qfil->in_quotes && pattern[(*i
			)] == qfil->quote_type && *i != qfil->quote_idx)
	{
		qfil->in_quotes = 0;
		(*i)++;
	}
}

void	back_track_two(int *i, int *j, t_matchpattern_idxs	*mp, int *in_quotes)
{
	back_track(i, j, &mp->star_idx, &mp->str_idx);
	*in_quotes = 0;
}

int	match_pattern(char *pattern, char *string)
{
	int					i;
	int					j;
	t_matchpattern_idxs	mp;
	t_qfilter			qfil;

	pattern_match_inits(&i, &j, &mp, &qfil);
	while (string[j])
	{
		quote_filter(pattern, &i, &qfil);
		if (pattern[i] == '*' && !qfil.in_quotes)
		{
			if (skip_stars(&i, pattern) == 1)
				return (1);
			update_vars(&mp.star_idx, &i, &mp.str_idx, &j);
		}
		else if (pattern[i] == string[j])
			pattern_is_string(&i, &j, pattern, &qfil);
		else if (mp.star_idx >= 0)
			back_track_two(&i, &j, &mp, &qfil.in_quotes);
		else
			return (0);
	}
	while (pattern[i] == '*')
		i++;
	return (!pattern[i]);
}
