/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/03/16 12:55:07 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_empty(char *str)
{
    int i;

    i = 0;
    if (!str)
        return (0);
    if (!*str)
        return (1);
    while (str[i] == 32)
        i++;
    if (str[i] == '\0')
        return (1);
    return (0);
}

char **parsing(char **argv, int argc)
{
    int i;
    char c;

    i = 0;
    c = (char)34;
    if (argc == 2)
    {
        if (check_empty(argv[1]) == 1) 
        {
            write(1, "error\n", 6);
            exit(1);
        }
        argv[1] = ft_strtrim(argv[1], &c);
    }
    return (argv);
}

int main(int argc, char *argv[])
{
    argv = parsing(argv, argc);
    int i = 0;
    while (argv[++i])
        printf("%s\n", argv[i]);
    i = 0;
    while (argv[++i])
        free(argv[i]);
    return 0;
}
