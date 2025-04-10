/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:46:34 by makkach           #+#    #+#             */
/*   Updated: 2025/04/10 20:22:42 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_list
{
	char			*data;
	char			*token;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;


typedef struct s_list_fd
{
	int					fd;
	char				*name;
	char				*command;
	char				*redir;
	struct s_list_fd	*next;
}	t_list_fd;

typedef struct s_tree
{
	char			*type;
	struct s_tree	*left;
	struct s_tree	*right;
	char			*command;
	char			**command_arr;
	char			*redirections;
}	t_tree;

char	*ft_strdup(char *s1);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strtrim(char *s1, char *set);
char	**ft_split(char *s, char c);
size_t	ft_strlen(char *str);
char	*ft_substr(char *s, unsigned int start, size_t len);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strchr(char *str, int c);
char	*ft_strncpy(char *dest, char *src, int n);
char	*ft_strcpy(char *dest, char *src);
char	*ft_strcat(char *dest, const char *src);
int		ft_strncmp(char *str1, char *str2, size_t n);

#endif