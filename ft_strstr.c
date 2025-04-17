/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:02:35 by makkach           #+#    #+#             */
/*   Updated: 2025/04/17 19:19:46 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strstr(const char *haystack, const char *needle)
{
    size_t i;
    size_t j;

    if (!haystack || !needle)
        return (NULL);
    if (!*needle)
        return ((char *)haystack);
    i = 0;
    while (haystack[i])
    {
        j = 0;
        while (needle[j] && haystack[i + j] == needle[j])
            j++;
        if (!needle[j])
            return ((char *)&haystack[i]);
        i++;
    }
    return (NULL);
}