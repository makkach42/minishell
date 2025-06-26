/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:26:59 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/26 11:27:06 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	ft_lent_strlen(char	const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && delimiter(s[i], c) == 0)
		i++;
	return (i);
}

static char	*ft_word(char	const	*s, char c)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	len_word = ft_lent_strlen(s, c);
	word = (char *)malloc(len_word + 1);
	if (word == NULL)
		return (NULL);
	while (i < len_word)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

t_env	*ft_check(t_env *h, char *str)
{
	if (!h || !h->next || !str)
		return (NULL);
	while (h != NULL)
	{
		if (ft_strcmp(h->key, str) == 0)
			return (h);
		h = h->next;
	}
	return (NULL);
}

int	ft_parse(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	if (ft_isalpha(s[0]) != 0 && s[0] != '_')
	{
		return (1);
	}
	while (s[i] != '\0')
	{
		if (ft_isalpha(s[i]) != 0 && s[i] != '_' && ft_isdigit(s[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*help;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		new -> next = *lst;
		*lst = new;
		return ;
	}
	help = *lst;
	while (help -> next != NULL)
	{
		help = help -> next;
	}
	help -> next = new;
}
