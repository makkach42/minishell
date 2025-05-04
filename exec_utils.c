#include "minishell.h"

int	delimiter(char c, char del)
{
	return (c == del);
}

void	ft_free(char **p)
{
	int	i;

	i = 0;
	while (p[i] != NULL)
	{
		free (p[i]);
		i++;
	}
	free (p);
}

void	ft_cmd_helper(t_cmd *com, int i, char *env, char *s)
{
	if (i == 0)
	{
		com->p = ft_split(env, ':');
		if (com->p == NULL)
			exit (1);
		com->t = ft_split(s, 32);
		if (com->t == NULL || com->t[0] == NULL)
			exit (1);
		com->str = ft_strmcpy(com->t[0]);
	}
	if (i == 1)
	{
		ft_free(com->t);
		ft_free(com->p);
		free (com->str);
	}
}

char	*ft_cmd_check(char *env, char *s)
{
	t_cmd	com;

	com.i = 0;
	if (s[0] == '\0' || ft_is_spaces(s) == 0)
		return (NULL);
	if (ft_strchr(s, '/') != NULL || env == NULL || env[0] == '\0')
	{
		if (access(s, X_OK) == 0)
			return (ft_strdup(s));
		return (NULL);
	}
	ft_cmd_helper(&com, 0, env, s);
	while (com.p[com.i] != NULL)
	{
		com.cmd = ft_strjoin(com.p[com.i], com.str);
		if (access(com.cmd, X_OK) == 0)
		{
			ft_cmd_helper(&com, 1, env, s);
			return (com.cmd);
		}
		free (com.cmd);
		com.i++;
	}
	ft_cmd_helper(&com, 1, env, s);
	return (NULL);
}

static int	ft_count_strings(char	const	*s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && delimiter(s[i], c) == 1)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != '\0' && delimiter(s[i], c) == 0)
			i++;
	}
	return (count);
}

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

static int	ft_fill_string(char **str, char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && delimiter(s[i], c) == 1)
			i++;
		if (s[i] != '\0')
		{
			str[j] = ft_word(&s[i], c);
			if (str[j] == NULL)
				return (0);
			j++;
		}
		while (s[i] != '\0' && delimiter(s[i], c) == 0)
			i++;
	}
	str[j] = 0;
	return (1);
}

char	**ft_split(char	const	*s, char c)
{
	char	**strings;
	int		i;

	i = 0;
	if (s == NULL)
		return (NULL);
	strings = (char **)malloc(sizeof(char *) * (ft_count_strings(s, c) + 1));
	if (strings == NULL)
		return (NULL);
	if (ft_fill_string(strings, s, c) == 0)
	{
		while (strings[i] != NULL)
		{
			free(strings[i]);
			i++;
		}
		free(strings);
		return (NULL);
	}
	return (strings);
}

t_env *ft_check(t_env *h, char *str)
{
    while (h != NULL)
    {
        if (ft_strcmp(h->key, str) == 0)
            return (h);
        h = h->next;
    }
    return (NULL);
}

int ft_parse(char *s)
{
    int i;

    i = 0;
    if (ft_isalpha(s[0]) != 0 && s[0] != '_')
        return (1);
    while (s[i] != '\0')
    {
        if (ft_isalpha(s[i]) != 0 && s[i] != '_' && ft_isdigit(s[i]) != 0)  
            return (1);
        i++;
    }
    return (0);
}


static int	ft_space(const char **str, int sign)
{
	while ((**str >= 9 && **str <= 13) || **str == 32)
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign *= -1;
		(*str)++;
	}
	return (sign);
}

long	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	long		res;
	long		tmp;

	sign = 1;
	i = 0;
	res = 0;
	tmp = 0;
	sign = ft_space(&str, sign);
	if (ft_strcmp((char *)str, "9223372036854775808") == 0)
		return (LONG_MIN);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - 48);
		if (res / 10 != tmp)
			return (LONG_MAX);
		tmp = res;
		i++;
	}
	return (res * sign);
}

int	ft_isalpha(int c)
{
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
		return (0);
	else
		return (1);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (0);
	else
		return (1);
}

void	ft_lstadd_front(t_env **lst, t_env *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new -> next = *lst;
	*lst = new;
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

t_env	*ft_lstnew(void *key, void *value)
{
	t_env	*newnode;

	newnode = (t_env *)malloc(sizeof(t_env));
	if (newnode == NULL)
		return (NULL);
	newnode -> key = key;
    newnode -> value = value;
	newnode -> next = NULL;
	return (newnode);
}

char	*ft_strmcpy(char *src)
{
	char	*dest;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dest = malloc(ft_strlen(src) + 2);
	if (dest == NULL)
		return (NULL);
	dest[j] = '/';
	j++;
	while (src[i])
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

int	ft_is_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != 9 && s[i] != 32)
			return (1);
		i++;
	}
	return (0);
}

int	ft_file_check(char *str)
{
	int	fd;

	fd = open(str, O_RDONLY);
	return (fd);
}

int	ft_file_create(char *str, int n)
{
	int	fd;

	if (n == 1)
		fd = open(str, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (n == 2)
		fd = open(str, O_RDWR | O_APPEND | O_CREAT, 0644);
	return (fd);
}

void	ft_putstr_fd(int fd, char *s)
{
	int	i;

	i = 0;
	if (s == NULL || fd < 0)
		return ;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	ft_lstsize(t_env *lst)
{
	int	count;

	count = 0;
	while (lst != NULL)
	{
		count++;
		lst = lst -> next;
	}
	return (count);
}
