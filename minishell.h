#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_list
{
    char *data;
    char *token;
    struct s_list *next;
}   t_list;

char	*ft_strdup(char *s1);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strtrim(char *s1, char *set);
char	**ft_split(char *s, char c);
size_t	ft_strlen(char *str);
char	*ft_substr(char  *s, unsigned int start, size_t len);
int    ft_strcmp(char *s1, char *s2);

#endif