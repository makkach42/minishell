#include "minishell.h"

int    ft_cd(char **s, t_env *h)
{
    t_env   *PWD;
    t_env   *OLD_PWD;
    t_env   *home;

    PWD = ft_check(h, "PWD");
    OLD_PWD = ft_lstnew("OLDPWD", NULL);
    if (s[1] == NULL || ft_strcmp("~", s[1]) == 0)
    {
        // dprintf(2, "entered here\n");
        home = ft_check(h, "HOME");
        if (home == NULL || home ->value == NULL)
        {
            ft_putstr_fd(2, "minishell: cd: HOME not set");
            return (1); 
        }
        if (chdir(home->value) == -1)
        {
            ft_putstr_fd(2, "minishell: cd: ");
            perror("");
            return (1);
        }
        OLD_PWD->value = PWD->value;
        PWD->value = home->value;
    }
    else
    {
        if (chdir(s[1]) == -1)
        {
            write(2, "minishell: cd: ", 16);
            write(2, s[1], ft_strlen(s[1]));
            write(2, ": No such file or directory", 28);
            return (1);
        }
        OLD_PWD->value = PWD->value;
        PWD->value = getcwd(NULL, 0);
    }
    return (0);
}

int ft_nline_check(char *str)
{
    int i;

    if (str[0] == '-' && str[1] == 'n')
    {
        i = 1;
        while (str[i] != '\0' && str[i] == 'n')
            i++;
        if (str[i] == '\0')
            return (0);
    }
    return (1);
}

int    ft_echo(char **s)
{
    int     flag;
    int     i;

    i = 1;
	flag = 0;
	while (s[i] != NULL)
	{
		if (ft_nline_check(s[i]) == 0)
		{
			while (ft_nline_check(s[i]) == 0)
				i++;
			flag = 0;
		}
		else
			flag = 1;
		while (s[i] != NULL)
		{
            ft_putstr_fd(1, s[i]);
			if (s[i + 1] != NULL)
				ft_putstr_fd(1, " ");
			i++;
		}
		if (s[i] == NULL)
			break;
		i++;
	}
	if (flag == 1)
		ft_putstr_fd(1, "\n");
    return (0);
}

int ft_env(t_env *h)
{
    while (h != NULL)
    {
        printf("%s=%s\n", h->key, h->value);
        h = h->next;
    }
    return (0);
}

int ft_check_string(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (ft_isdigit(str[i]) != 0 && str[0] != '-')
            return (1);
        i++;
    }
    return (0);
}

int ft_modulo(char *str)
{
    long    n;
    int     m;

    n = ft_atoi(str);
    if (ft_strcmp(str, "9223372036854775807") != 0 && n == LONG_MAX)
    {
        write (2, "minishell: exit: ", 18);
        write(2, str, ft_strlen(str));
        write(2, ": numeric argument required", 28);
        exit (255);
    }
    m = n % 256;
    return (m);
}


void    ft_exit(char **s)
{
    int m;

    if (s[1] == NULL)
        exit (0);
    else
    {
        if (ft_check_string(s[1]) != 0)
        {
            write(2, "minishell: exit: ", 18);
            write(2, s[1], ft_strlen(s[1]));
            write(2, ": numeric argument required\n", 29);
            exit (255);
        }
        else if (s[2] != NULL)
            write(2, "minishell: exit: too many arguments\n", 37);
        else
        {
            m = ft_modulo(s[1]);
            if (m < 0)
                exit (m + 256);
            else
                exit (m);
        }
    }
}


t_env   *ft_sort_list(t_env *h)
{
    char   *tmp_key;
    char   *tmp_value;
    t_env   *head;
    t_env   *start;

    head = h;
    start = NULL;
    while (h != NULL && h->next != NULL)
    {
        start = h->next;
        while (start != NULL)
        {
            if (ft_strcmp(h->key, start->key) > 0)
            {
                tmp_key = h->key;
                h->key = start->key;
                start->key = tmp_key;
                tmp_value = h->value;
                h->value = start->value;
                start->value = tmp_value;
            }
            else if (ft_strcmp(h->key, start->key) == 0 && ft_strcmp(h->value, start->value) > 0)
            {
                tmp_key = h->key;
                h->key = start->key;
                start->key = tmp_key;
                tmp_value = h->value;
                h->value = start->value;
                start->value = tmp_value;
            }
            start = start->next;
        }
        h = h->next;
    }
    return (head);
}

int ft_equal_check(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '=')
            return (0);
        i++;
    }
    return (1);
}

void    ft_remove_sign(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '+' && str[i + 1] == '\0')
            str[i] = '\0';
        i++;
    }
}

int    ft_export(char  **s, t_env *h)
{
    char    **v;
    t_env   *f;
    t_env   *new;
    char    *tmp;
    int     i;
    int     act;
    int     status;
    
    i = 1;
    status = 0;
    if (s[1] == NULL)
    {
        h = ft_sort_list(h);
        while (h != NULL)
        {
            printf("%s=%s\n", h->key, h->value);
            h = h->next;
        }
    }
    else if (s[i] != NULL)
    {
        while (s[i] != NULL)
        {
            act = 1;
            if (ft_equal_check(s[i]) == 0)
                act = 0;
            if (s[i][0] == '=')
            {
                write(2, "bash: export: ", 15);
                write(2, s[i], ft_strlen(s[i]));
                write(2, ": not a valid identifier\n", 26);
                status = 1;
            }
            else if (ft_strchr(s[i], '+') == NULL)
            {
                v = ft_split(s[i], '=');
                if (v == NULL)
                    return (1);
                f = ft_check(h, v[0]);
                if (f != NULL)
                {
                    if (v[1] == NULL && act == 0)
                        f->value = ft_strdup("");
                    else if (v[1] != NULL)
                    {
                        tmp = f->value;
                        f->value = v[1];
                        free (tmp);
                    }    
                }
                else
                {
                    if (ft_parse(v[0]) == 1)
                    {
                        if (i == 1)
                        {
                            write(2, "minishell: export: not an identifier: ", 39);
                            write(2, s[i], ft_strlen(s[i]));
                            write(2, "\n", 1);
                            status = 1;
                        }
                    }
                    else
                    {
                        if (act == 0)
                        {
                            if (v[1] == NULL)
                                new = ft_lstnew(v[0], ft_strdup(""));
                            else
                                new = ft_lstnew(v[0], v[1]);
                        }
                        else
                            new = ft_lstnew(v[0], NULL);
                        ft_lstadd_back(&h, new);
                    }
                }
            }
            else
            {
                if (ft_equal_check(s[i]) == 1)
                {
                    write(2, "export: not valid in this context: ", 36);
                    write(2, s[i], ft_strlen(s[i]));
                    write(2, "\n", 1);
                    status = 1;
                }
                else
                {
                    v = ft_split(s[i], '=');
                    if (v == NULL)
                        return (1);
                    ft_remove_sign(v[0]);
                    f = ft_check(h, v[0]);
                    if (f != NULL)
                    {
                        tmp = f->value;
                        f->value = ft_strjoin(f->value, v[1]);
                        free (tmp);
                    }
                    else
                    {
                        if (ft_parse(v[0]) == 1)
                        {
                            if (i == 1)
                            {
                                write(2, "minishell: export: not an identifier: ", 39);
                                write(2, s[i], ft_strlen(s[i]));
                                write(2, "\n", 1);
                                status = 1;
                            }
                        }
                        else
                        {
                            if (act == 0)
                            {
                                if (v[1] == NULL)
                                     new = ft_lstnew(v[0], ft_strdup(""));
                                else
                                    new = ft_lstnew(v[0], v[1]);
                            }
                            else
                                new = ft_lstnew(v[0], NULL);
                            ft_lstadd_back(&h, new);
                        }
                    }
                }
            }
            i++;
        }
    }
    return (status);
}

int    ft_pwd(void)
{
    char *path;

    path = getcwd(NULL, 0);
    if (path == NULL)
    {
        perror("pwd: ");
        return (1);
    }
    printf("%s\n", path);
    free (path);
    return (0);
}


t_env   *ft_unset(t_env *h, char **s)
{
    t_env   *node;
    int     flag;
    t_env   *start;//parsing
    int     i;

    flag = 0;
    i = 1;
    start = NULL;
    if (s == NULL)
        return (NULL);
    start = h;
    while (s[i] != NULL)
    {
        h = start;
        if (ft_parse(s[i]) == 1)
        {
            write(2, "unset: ", 8);
            write(2, s[i], ft_strlen(s[i]));
            write(2, ": invalid parameter name", 25);
        }
        else
        {
            node = ft_check(h, s[i]);
            if (node != NULL)
            {
                while (h != NULL && h->next != NULL)
                {
                    if (flag == 0 && h == node)
                    {
                        h = h->next;
                        return (h);
                    }
                    if (h->next == node)
                    {
                        h->next = node->next;
                        node->next = NULL;
                    }
                    flag = 1;
                    h = h->next;
                }
            }
        }
        i++;
    }
    return (start);
}

