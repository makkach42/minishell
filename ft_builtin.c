#include "minishell.h"

int    ft_cd(char **s, t_env *h)
{
    t_env   *PWD;
    t_env   *OLD_PWD;
    t_env   *home;
    t_env   *n;
    // static char    *p;
    char        *temp;

    temp = NULL;
    PWD = ft_check(h, "PWD");
    OLD_PWD = ft_check(h, "OLDPWD");
    n = ft_check(h, "1PWD");
    // dprintf(2, "this is PWD: %s\n", PWD->value);
    if (PWD && PWD->value)
        n->value = PWD->value;
    // OLD_PWD = ft_lstnew("OLDPWD", NULL);
    if (s[1] == NULL)
    {
        // dprintf(2, "entered here\n");
        home = ft_check(h, "HOME");
        if (home == NULL || home ->value == NULL)
        {
            ft_putstr_fd(2, "minishell: cd: HOME not set\n");
            return (1); 
        }
        if (chdir(home->value) == -1)
        {
            ft_putstr_fd(2, "minishell: cd: ");
            perror("");
            return (1);
        }
        if (!PWD || !PWD->value)
            return (0);
        else
        {
            if (!OLD_PWD)
                OLD_PWD->value = PWD->value;
            PWD->value = home->value;
        }
    }
    else
    {
        if (chdir(s[1]) == -1)
        {
            write(2, "minishell: cd: ", 16);
            write(2, s[1], ft_strlen(s[1]));
            write(2, ": No such file or directory\n", 29);
            return (1);
        }
        if (!PWD || !PWD->value)
            return (0);
        else
        {
            if (!OLD_PWD || !OLD_PWD->value)
                OLD_PWD->value = PWD->value;
            PWD->value = getcwd(NULL, 0);
            if (PWD->value == NULL)
            {
                temp = s[1];
                s[1] = ft_strjoin("/", s[1]);
                if (s[1] == NULL)
                    return (1);
                free (temp);
                temp = PWD->value;
                PWD->value = ft_strjoin(n->value, s[1]);
                if (!PWD->value)
                    return (1);
                n->value = PWD->value;
                free (temp);
            }

        }
    }
    // dprintf(2, "this is the value of n: %s\n", n->value);
    return (0);
}

int ft_nline_check(char *str)
{
    int i;

    if (str == NULL)
        return (1);
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
			break ;
		i++;
	}
	if (flag == 1)
		ft_putstr_fd(1, "\n");
    return (0);
}

int ft_env(t_env *h)
{
    if (h == NULL)
    {
        ft_putstr_fd(2, "minishell: env: No such file or directory\n");
        return (127);
    }
    while (h != NULL)
    {
        if (h->active == 0)
            printf("%s=%s\n", h->key, h->value);
        h = h->next;
    }
    return (0);
}

int ft_check_string(char *str)
{
    int i;

    i = 0;
    if (str[i] != '\0' && (str[i] == '+' || str[i] == '-'))
        i++;
    while (str[i] != '\0')
    {
        if (ft_isdigit(str[i]) != 0)
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


void    ft_exit(char **s, t_env **h)
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
            {
                free_env(h);
                exit (m + 256);
            }
            else
            {
                free_env(h);
                exit (m);
            }
        }
    }
}


t_env   *ft_sort_list(t_env *h)
{
    char   *tmp_key;
    char   *tmp_value;
    int     active;
    int     hidden;
    t_env   *head;
    t_env   *start;

    head = h;
    active = 0;
    hidden = 0;
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
                active = h->active;
                h->active = start->active;
                start->active = active;
                hidden = h->h;
                h->h = start->h;
                start->h = hidden;
            }
            else if (ft_strcmp(h->key, start->key) == 0 && ft_strcmp(h->value, start->value) > 0)
            {
                tmp_key = h->key;
                h->key = start->key;
                start->key = tmp_key;
                tmp_value = h->value;
                h->value = start->value;
                start->value = tmp_value;
                active = h->active;
                h->active = start->active;
                start->active = active;
                hidden = h->h;
                h->h = start->h;
                start->h = hidden;
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

int     ft_equal_count(char *str)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == '=')
            count++;
        i++;
    }
    return (count);
}

char **ft_equal_str(char *str)
{
    int i;
    char    **p;
    int     flag;

    i = 0;
    flag = 0;
    p = malloc(sizeof(char *) * 2);
    if (p == NULL)
        return (NULL);
    while (str[i])
    {
        if (str[i] == '=' && flag == 0)
        {
            // dprintf(2, "this is i: %d\n", i);
            p[0] = ft_substr(str, 0, i);
            // dprintf(2, "this is the first: %s\n", p[0]);
            if (p[0] == NULL)
            {
                free (p);
                return (NULL);
            }
            i++;
            p[1] = ft_substr(str, i, ft_strlen(&str[i]));
            // dprintf(2, "this is the second : %s\n", p[1]);
            if (p[1] == NULL)
            {
                free (p[0]);
                free (p);
                return (NULL);
            }
            flag = 1;
        }
        i++;
    }
    return (p);
}

int    ft_export(char  **s, t_env *h, t_tree *tree)
{
    char    **v;
    t_env   *f;
    t_env   *new;
    char    *tmp;
    int     i;
    int     act;
    int     status;
    // t_env *temp;
    
    i = 1;
    status = 0;
    while (s[i] != NULL && (check_empty(s[i]) == 1 && tree->var == 1))
        i++;
    if (s[i] == NULL)
    {
        h = ft_sort_list(h);
        while (h != NULL)
        {
            if ((h)->h == 0)
                printf("%s=%s\n", (h)->key, (h)->value);
            h = (h)->next;
        }
    }
     while (s[i] != NULL)
    {
        if (s[i] != NULL)
        {
                act = 1;
                if ((check_empty(s[i]) == 1 && tree->var == 0))
                {
                    write(2, "minishell: export: ", 20);
                    write(2, s[i], ft_strlen(s[i]));
                    write(2, ": not a valid identifier\n", 26);
                    status = 1;
                }
                else if (check_empty(s[i]) != 1)
                {
                    if (ft_equal_check(s[i]) == 0)
                        act = 0;
                    if (s[i][0] == '=')
                    {
                        write(2, "minishell: export: ", 15);
                        write(2, s[i], ft_strlen(s[i]));
                        write(2, ": not a valid identifier\n", 26);
                        status = 1;
                    }
                    else if (ft_strchr(s[i], '+') == NULL)
                    {
                        // dprintf(2, "this is : %s\n", s[i]);
                        if (ft_equal_count(s[i]) > 1)
                            v = ft_equal_str(s[i]);
                        else
                            v = ft_split(s[i], '=');
                        if (v == NULL)
                            return (1);
                        f = ft_check(h, v[0]);
                        if (f != NULL && f->h == 0)
                        {
                            if (v[1] == NULL && act == 0)
                            {
                                f->value = ft_strdup("");
                                // free (v[0]);
                            }
                            else if (v[1] != NULL)
                            {
                                // dprintf(2, "%s\n", v[1]);
                                tmp = f->value;
                                f->value = v[1];
                                free (tmp);
                            }    
                        }
                        else
                        {
                            if (ft_parse(v[0]) == 1)
                            {
                                // if (i == 1)
                                // {
                                    // dprintf(2, "this is the first and second: %s , %s", v[0], v[1]);
                                    write(2, "minishell: export: not an identifier: ", 39);
                                    write(2, s[i], ft_strlen(s[i]));
                                    write(2, "\n", 1);
                                    status = 1;
                                // }
                            }
                            else
                            {
                                if (act == 0)
                                {
                                    if (v[1] == NULL)
                                    {
                                        new = ft_lstnew(v[0], ft_strdup(""));
                                        free (v[0]);
                                        new->active = 0;
                                        new->h = 0;
                                    }
                                    else
                                    {
                                        new = ft_lstnew(v[0], v[1]);
                                        free (v[0]);
                                        free (v[1]);
                                        new->active = 0;
                                        new->h = 0;
                                    }
                                }
                                else
                                {
                                    new = ft_lstnew(v[0], NULL);
                                    free (v[0]);
                                    new->active = 1;
                                    new->h = 0;
                                }
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
                            if (f != NULL && f->h == 0)
                            {
                                tmp = f->value;
                                f->value = ft_strjoin(f->value, v[1]);
                                free (tmp);
                                free (v[1]);
                            }
                            else
                            {
                                if (ft_parse(v[0]) == 1)
                                {
                                    // if (i == 1)
                                    // {
                                        write(2, "minishell: export: not an identifier: ", 39);
                                        write(2, s[i], ft_strlen(s[i]));
                                        write(2, "\n", 1);
                                        status = 1;
                                    // }
                                }
                                else
                                {
                                    if (act == 0)
                                    {
                                        if (v[1] == NULL)
                                        {
                                            new = ft_lstnew(v[0], ft_strdup(""));
                                            free (v[0]);
                                            new->active = 0;
                                            new->h = 0;
                                        }
                                        else
                                        {
                                            new = ft_lstnew(v[0], v[1]);
                                            free (v[0]);
                                            free (v[1]);
                                            new->active = 0;
                                            new->h = 0;
                                        }
                                    }
                                    else
                                    {
                                        new = ft_lstnew(v[0], NULL);
                                        free (v[0]);
                                        new->active = 1;
                                        new->h = 0;
                                    }
                                    ft_lstadd_back(&h, new);
                                }
                            }
                        }
                    }
                }
            // }
        }
        i++;
    }
    return (status);
}

int    ft_pwd(t_env *h)
{
    char *path;
    char *temp;
    t_env *n;
    
    n = ft_check(h, "1PWD");
    path = getcwd(NULL, 0);
    if (path)
        n->value = path;
    if (path == NULL)
    {
        path = n->value;
        // perror("pwd: ");
        // return (0);
    }
    printf("%s\n", path);
    // free (path);
    return (0);
}

void    ft_f_node(t_env *node)
{
    free (node->key);
    free (node->value);
    free (node);
    node = NULL;
}


void    ft_unset(t_env **h, char **s)
{
    t_env   *node;
    int     flag;
    t_env   *start;//parsing
    int     i;

    flag = 0;
    i = 1;
    start = NULL;
    if (s == NULL)
        return ;
    while (s[i] != NULL)
    {
        // dprintf(2, "this is the string: %s\n", s[i]);
        if (ft_parse(s[i]) == 1)
        {
            write(2, "unset: ", 8);
            write(2, s[i], ft_strlen(s[i]));
            write(2, ": invalid parameter name", 25);
        }
        else
        {
            node = NULL;
            node = ft_check(*h, s[i]);
            if (node != NULL)
            {
                if (*h == node)
                {
                    *h = (*h)->next;
                    node->next = NULL;
                    ft_f_node(node);
                    // return ;
                }
                else 
                {
                    start = *h;
                    while (start != NULL)
                    {
                        if (start->next == node)
                        {
                            start->next = node->next;
                            node->next = NULL;
                            ft_f_node(node);
                            break ;
                        }
                        start = start->next;
                    }
                }
            }
            // }
        }
        i++;
    }
}

