/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:26:07 by aakroud           #+#    #+#             */
/*   Updated: 2025/06/22 17:16:41 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char *ft_str_env(t_env *h)
{
    char *buff;
    char *tmp;
    
    buff = ft_strjoin(h->key, "=");
    if (!buff)
        return (NULL);
    tmp = buff;
    buff = ft_strjoin(buff, h->value);
    if (!buff)
    {
        free (tmp);
        return (NULL);
    }
    free (tmp);
    tmp = buff;
    buff = ft_strjoin(buff, "\n");
    if (!buff)
    {
        free (tmp);
        return (NULL);
    }
    free (tmp);
    return (buff);
}

int ft_env(t_env *h)
{
    char *buff;
    char *str;
    char *tmp;
    t_env *start;

    start = h;
    buff = NULL;
    // if (h == NULL || ft_check_act(h))
    // {
    //     ft_putstr_fd(2, "minishell: env: No such file or directory\n");
    //     return (127);
    // }
    while (h != NULL)
    {
        if (h->active == 0)
        {
            if (h == start)
            {
                str = ft_str_env(h);
                if (!str)
                    return (1);
                buff = str;
            }
            else
            {
                tmp = buff;
                str = ft_str_env(h);
                buff = ft_strjoin(buff, str);
                if (!buff)
                {
                    free (str);
                    return (1);
                }
                free (str);
                free (tmp);
            }
            // printf("%s=%s\n", h->key, h->value);
        }
        h = h->next;
    }
    if (buff)
    {
        ft_putstr_fd(1, buff);
        free (buff);
    }
    return (0);
}

int ft_check_string(char *str)
{
    int i;

    i = 0;
    if (str[i] != '\0' && (str[i] == '+' || str[i] == '-'))
        i++;
	if (str[i] == NULL)
		return (1);
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


void    ft_exit(char **s, t_env **h, int status)
{
    int m;

    if (isatty(0) && isatty(1))
        ft_putstr_fd(1, "exit\n");
    if (s[1] == NULL)
        exit (status);
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
        {
            write(2, "minishell: exit: too many arguments\n", 37);
        }
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
    // dprintf(2, "this is it : %d\n", count);
    return (count);
}

char **ft_equal_str(char *str)
{
    int i;
    char    **p;
    int     lent_first;

    i = 0;
    // flag = 0;
    lent_first = 0;
    p = malloc(sizeof(char *) * 2);
    if (p == NULL)
        return (NULL);
    while (str[i])
    {
        if (str[i] == '=')
        {
            p[0] = ft_substr(str, 0, i);
            if (p[0] == NULL)
            {
                free (p);
                return (NULL);
            }
            lent_first = i;
            i++;
            p[1] = ft_substr(str, i, ft_strlen(&str[i]));
            if (p[1] == NULL)
            {
                free (p[0]);
                free (p);
                return (NULL);
            }
            break ;
        }
        i++;
    }
    p[0] = ft_strncpy(p[0], str, lent_first);
    p[1] = ft_strcpy(p[1], &str[i]);
    return (p);
}

char *ft_str_export(t_env *h)
{
    char *buff;
    char *tmp;
    
    buff = ft_strjoin("declare -x ", NULL);
    if (!buff)
        return (NULL);
    tmp = buff;
    buff = ft_strjoin(buff, h->key);
    if (!buff)
        return (NULL);
    free (tmp);
    tmp = buff;
    buff = ft_strjoin(buff, "=");
    if (!buff)
        return (NULL);
    free (tmp);
    tmp = buff;
    buff = ft_strjoin(buff, h->value);
    if (!buff)
    {
        free (tmp);
        return (NULL);
    }
    free (tmp);
    tmp = buff;
    buff = ft_strjoin(buff, "\n");
    if (!buff)
    {
        free (tmp);
        return (NULL);
    }
    free (tmp);
    return (buff);
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
    char    *buff;
    t_env *start;
    char    *str;

    start = h;
    buff = NULL;
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
            {
                if (h == start)
                {
                    str = ft_str_export(h);
                    if (!str)
                        return (1);
                    buff = str;
                }
                else
                {
                    tmp = buff;
                    str = ft_str_export(h);
                    buff = ft_strjoin(buff, str);
                    if (!buff)
                    {
                        free (str);
                        return (1);
                    }
                    free (str);
                    free (tmp);
                }
            }
            h = (h)->next;
        }
        if (buff)
        {
            ft_putstr_fd(1, buff);
            free (buff);
        }
        return (0);
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
                        write(2, "minishell: export: ", 20);
                        write(2, s[i], ft_strlen(s[i]));
                        write(2, ": not a valid identifier\n", 26);
                        status = 1;
                    }
                    else if (ft_strchr(s[i], '+') == NULL)
                    {
                        // dprintf(2, "this is : %s\n", s[i]);
                        if (ft_equal_count(s[i]) > 1)
                            v = ft_equal_str(s[i]);//must be checked for leaks
                        else
                            v = ft_split(s[i], '=');
                        dprintf(2, "this is v0 %s and this is v1 %s\n", v[0], v[1]);
                        if (v == NULL)
                            return (1);
                        f = ft_check(h, v[0]);
                        if (f != NULL && f->h == 0)
                        {
                            if (v[1] == NULL && act == 0)
                            {
                                dprintf(2, "entered in NULL\n");
                                tmp = f->value;
                                f->value = ft_strdup("");
                                f->active = 0;
                                f->h = 0;
                                free (v[0]);
                                free (v);
                                free (tmp);
                            }
                            else if (v[1] != NULL)
                            {
                                dprintf(2, "entered in not NULL\n");
                                tmp = f->value;
                                f->value = ft_strdup(v[1]);
                                f->active = 0;
                                f->h = 0;
                                free (v[1]);
                                free (v[0]);
                                free (v);
                                free (tmp);
                            }
                            else if (!v[1] && act == 1)
                            {
                                free (v[0]);
                                free (v);
                            }
                        }
                        else
                        {
                            if (ft_parse(v[0]) == 1)
                            {
                                    write(2, "minishell: export: not an identifier: ", 39);
                                    write(2, s[i], ft_strlen(s[i]));
                                    write(2, "\n", 1);
                                    status = 1;
                            }
                            else
                            {
                                if (act == 0)
                                {
                                    if (v[1] == NULL)
                                    {
                                        dprintf(2, "it entered in NULL again\n");
                                        new = ft_lstnew(v[0], "");
                                        if (!new)
                                            return (1);
                                        free (v[0]);
                                        free (v);
                                        new->active = 0;
                                        new->h = 0;
                                    }
                                    else
                                    {
                                        new = ft_lstnew(v[0], v[1]);
                                        free (v[0]);
                                        free (v[1]);
                                        free (v);
                                        new->active = 0;
                                        new->h = 0;
                                    }
                                }
                                else
                                {
                                    new = ft_lstnew(v[0], NULL);
                                    free (v[0]);
                                    free (v);
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
                            if (ft_equal_count(s[i]) > 1)
                                v = ft_equal_str(s[i]);//must be checked for leaks
                            else
                                v = ft_split(s[i], '=');
                            if (v == NULL)
                                return (1);
                            ft_remove_sign(v[0]);
                            f = ft_check(h, v[0]);
                            if (f != NULL && f->h == 0)
                            {
                                tmp = f->value;
                                f->value = ft_strjoin(f->value, v[1]);
                                f->active = 0;
                                f->h = 0;
                                free (v[0]);
                                free (tmp);
                                free (v[1]);
                                free (v);
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
                                            free (v);
                                            new->active = 0;
                                            new->h = 0;
                                        }
                                        else
                                        {
                                            new = ft_lstnew(v[0], v[1]);
                                            free (v[0]);
                                            free (v[1]);
                                            free (v);
                                            new->active = 0;
                                            new->h = 0;
                                        }
                                    }
                                    else
                                    {
                                        new = ft_lstnew(v[0], NULL);
                                        free (v[0]);
                                        free (v);
                                        new->active = 1;
                                        new->h = 0;
                                    }
                                    ft_lstadd_back(&h, new);
                                }
                            }
                        }
                    }
                }
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
    {
        dprintf(2, "entered in path: in pwd\n");
        temp = n->value;
        n->value = path;
        free (temp);// to free 1PWD when it is allocated in cd
    }
    else if (!path)
    {
        // dprintf(2, "entered in path NULL\n");
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
        if (ft_parse(s[i]) == 1)
        {
            write(2, "unset: ", 8);
            write(2, s[i], ft_strlen(s[i]));
            write(2, ": invalid parameter name\n", 26);
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

