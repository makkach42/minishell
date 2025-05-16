/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:35:17 by makkach           #+#    #+#             */
/*   Updated: 2025/05/16 12:06:01 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_arr_fill_helper(t_tree **tree, char **str)
{
	t_list	*head;
	char	**arr;
	t_list	*tmp;
	int		i;
	int		list_size;

	i = 0;
	head = list_init(*str);
	list_size = lst_size(&head);
	arr = malloc(sizeof(char *) * (list_size + 1));
	tmp = head;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->data);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	free_list(&head);
	(*tree)->command_arr = arr;
}

void	command_arr_fill(t_tree **tree)
{
	char	*str;
	char	*tmp;
	char	**arr;

	if ((*tree) && (*tree)->left)
		command_arr_fill(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		command_arr_fill(&(*tree)->right);
	if ((tree) && (*tree) && (*tree)->command && !(*tree)->command_arr)
	{
		str = ft_strdup((*tree)->command);
		if (!str || !*str)
		{
			arr = malloc(sizeof(char *));
			if (arr)
				arr[0] = NULL;
			(*tree)->command_arr = arr;
			free(str);
			return ;
		}
		tmp = str;
		str = ft_strtrim(str, " ");
		free(tmp);
		command_arr_fill_helper(tree, &str);
	}
}

void	env_fill_helper(t_env **node, int *i, int *j, char **argev)
{
	*node = malloc(sizeof(t_env));
	while (argev[*i] && argev[*i][*j] != '=')
		(*j)++;
	(*node)->key = ft_substr(argev[*i], 0, *j);
	(*node)->value = ft_substr(argev[*i],
			(*j + 1), ft_strlen(argev[*i]) - (*j + 1));
	*j = 0;
}

t_env	*env_fill(char **argev)
{
	int		i;
	int		j;
	t_env	*head;
	t_env	*new_node;
	t_env	*tmp;

	i = 0;
	j = 0;
	env_fill_helper(&head, &i, &j, argev);
	head->next = NULL;
	tmp = head;
	i++;
	while (argev[i])
	{
		env_fill_helper(&new_node, &i, &j, argev);
		new_node->next = NULL;
		tmp->next = new_node;
		tmp = new_node;
		i++;
	}
	return (head);
}

void	f(void)
{
	system("leaks -q minishell");
}

void	var_set(t_tree **tree)
{
	int	i;

	if ((*tree) && (*tree)->left)
		var_set(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		var_set(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		i = 0;
		while ((*tree)->command_arr[i])
		{
			if (variable_search_instr((*tree)->command_arr[i]))
				(*tree)->var = 1;
			i++;
		}
	}
}

int	hidenword(char *str, char *str2)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	if (!str || !str2)
		return (0);
	while (str2[i])
	{
		while (str[j])
		{
			if (str2[i] == str[j])
				break ;
			j++;
		}
		if (!str[j])
			break ;
		if (str[j] == str2[i])
			count++;
		i++;
	}
	if (count == (int)ft_strlen(str2))
		return (1);
	return (0);
}

void	reset_command_arr(t_tree **tree)
{
	int		i;
	int		j;
	int		count;
	int		flag;
	int		in_quotes;
	int		size_lst;
	char	quote_type;
	char	*old_cmd;
	char	*prev_command_arr;
	char	**new_cmd_arr;
	t_list	*head;
	t_list	*new_node;
	t_list	*tmp;

	if ((*tree) && (*tree)->left)
		reset_command_arr(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		reset_command_arr(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		if ((!ft_strcmp((*tree)->command_arr[0], "export"
				) || variable_search_instr((*tree
					)->command_arr[0])) || hidenword((
					*tree)->command_arr[0], "export"))
		{
			i = 0;
			while ((*tree)->command_arr[i])
				i++;
			(*tree)->command_arr_expanded = malloc(sizeof(char **) * (i + 1));
			if (!(*tree)->command_arr_expanded)
				return ;
			i = 0;
			while ((*tree)->command_arr[i])
			{
				old_cmd = ft_strdup((*tree)->command_arr[i]);
				if (!old_cmd)
				{
					while (i > 0)
					{
						i--;
						if ((*tree)->command_arr_expanded[i])
						{
							j = 0;
							while ((*tree)->command_arr_expanded[i][j])
							{
								free((*tree)->command_arr_expanded[i][j]);
								j++;
							}
							free((*tree)->command_arr_expanded[i]);
						}
					}
					free((*tree)->command_arr_expanded);
					return ;
				}
				head = NULL;
				if (ft_strchr(old_cmd, '='))
				{
					j = 0;
					while (old_cmd[j] && old_cmd[j] != '=')
						j++;
					if (old_cmd[j] == '=')
					{
						head = malloc(sizeof(t_list));
						if (!head)
						{
							free(old_cmd);
							return ;
						}
						head->data = ft_substr(old_cmd, 0, j);
						head->token = NULL;
						head->next = NULL;
						new_node = malloc(sizeof(t_list));
						if (!new_node)
						{
							free(old_cmd);
							free_list(&head);
							return ;
						}
						new_node->data = ft_strdup("=");
						new_node->token = NULL;
						new_node->next = NULL;
						head->next = new_node;
						tmp = new_node;
						new_node = malloc(sizeof(t_list));
						if (!new_node)
						{
							free(old_cmd);
							free_list(&head);
							return ;
						}
						new_node->data = ft_substr(
								old_cmd, j + 1, ft_strlen(old_cmd) - (j + 1));
						new_node->token = NULL;
						new_node->next = NULL;
						tmp->next = new_node;
						free(old_cmd);
						old_cmd = NULL;
					}
				}
				if (head)
				{
					size_lst = lst_size(&head);
					new_cmd_arr = malloc(sizeof(char *) * (size_lst + 1));
					if (!new_cmd_arr)
					{
						free_list(&head);
						if (old_cmd)
							free(old_cmd);
						return ;
					}
					j = 0;
					tmp = head;
					while (tmp)
					{
						new_cmd_arr[j] = ft_strdup(tmp->data);
						j++;
						tmp = tmp->next;
					}
					new_cmd_arr[j] = NULL;
					free_list(&head);
					(*tree)->command_arr_expanded[i] = new_cmd_arr;
				}
				else
				{
					new_cmd_arr = malloc(sizeof(char *) * 2);
					if (!new_cmd_arr)
					{
						if (old_cmd)
							free(old_cmd);
						return ;
					}
					new_cmd_arr[0] = old_cmd;
					new_cmd_arr[1] = NULL;
					(*tree)->command_arr_expanded[i] = new_cmd_arr;
					old_cmd = NULL;
				}
				if (old_cmd)
					free(old_cmd);
				i++;
			}
			(*tree)->command_arr_expanded[i] = NULL;
			i = 0;
			j = 0;
			count = 0;
			while ((*tree)->command_arr_expanded[i])
			{
				j = 0;
				while ((*tree)->command_arr_expanded[i][j])
				{
					count++;
					j++;
				}
				i++;
			}
			new_cmd_arr = malloc(sizeof(char *) * (count + 1));
			i = 0;
			j = 0;
			count = 0;
			while ((*tree)->command_arr_expanded[i])
			{
				j = 0;
				while ((*tree)->command_arr_expanded[i][j])
				{
					new_cmd_arr[count] = ft_strdup((
								*tree)->command_arr_expanded[i][j]);
					count++;
					j++;
				}
				i++;
			}
			new_cmd_arr[count] = NULL;
			i = 0;
			while ((*tree)->command_arr[i])
			{
				free((*tree)->command_arr[i]);
				i++;
			}
			free((*tree)->command_arr);
			(*tree)->command_arr = new_cmd_arr;
			i = 0;
			while ((*tree)->command_arr[i])
				i++;
			(*tree)->split = malloc(sizeof(t_split *) * (i + 1));
			i = 0;
			while ((*tree)->command_arr[i])
			{
				(*tree)->split[i] = malloc(sizeof(t_split));
				(*tree)->split[i]->arr_index = 0;
				(*tree)->split[i]->split_flag = 0;
				i++;
			}
			prev_command_arr = NULL;
			if (!ft_strcmp((*tree)->command_arr[0], "export"
				) || variable_search_instr((*tree)->command_arr[0]))
			{
				i = 1;
				in_quotes = 0;
				flag = 0;
				while ((*tree)->command_arr[i])
				{
					j = 0;
					while ((*tree)->command_arr[i][j])
					{
						if (!in_quotes && ((*tree
								)->command_arr[i][j] == '"' || (
								*tree)->command_arr[i][j] == '\''))
						{
							in_quotes = 1;
							quote_type = (*tree)->command_arr[i][j];
						}
						else if (in_quotes && (*tree)->command_arr[i][j] == '$')
							flag = 1;
						else if (in_quotes && (*tree
							)->command_arr[i][j] == quote_type)
							in_quotes = 0;
						j++;
					}
					(*tree)->split[i]->arr_index = i;
					if ((variable_search_instr((*tree)->command_arr[i]
							) && prev_command_arr && !ft_strcmp(
								prev_command_arr, "=") && (
								i - 2 < 0 || (i - 2 >= 0 && (
										!variable_search_instr((*tree
											)->command_arr[i - 2]))))) || (
							variable_search_instr((*tree)->command_arr[i]
							) && flag == 1) || (
							!ft_strchr((*tree)->command_arr[i], '$')))
						(*tree)->split[i]->split_flag = 0;
					else
						(*tree)->split[i]->split_flag = 1;
					prev_command_arr = (*tree)->command_arr[i];
					flag = 0;
					i++;
				}
				(*tree)->split[i] = NULL;
			}
			else if (hidenword((*tree)->command_arr[0], "export"))
			{
				i = 1;
				in_quotes = 0;
				flag = 0;
				while ((*tree)->command_arr[i])
				{
					j = 0;
					while ((*tree)->command_arr[i][j])
					{
						if (!in_quotes && ((*tree
								)->command_arr[i][j] == '"' || (
								*tree)->command_arr[i][j] == '\''))
						{
							in_quotes = 1;
							quote_type = (*tree)->command_arr[i][j];
						}
						else if (in_quotes && (*tree)->command_arr[i][j] == '$')
							flag = 1;
						else if (in_quotes && (*tree
							)->command_arr[i][j] == quote_type)
							in_quotes = 0;
						j++;
					}
					(*tree)->split[i]->arr_index = i;
					if (((variable_search_instr((*tree)->command_arr[i]
								) && flag == 1)))
						(*tree)->split[i]->split_flag = 0;
					else
						(*tree)->split[i]->split_flag = 1;
					i++;
				}
				(*tree)->split[i] = NULL;
				i = 1;
				while ((*tree)->command_arr[i])
				{
					if (!variable_search_instr((*tree)->command_arr[i]))
						(*tree)->split[i]->split_flag = 0;
					i++;
				}
			}
		}
	}
}

void	split_adjustments(t_tree **tree)
{
	int	i;

	if ((*tree) && (*tree)->left)
		split_adjustments(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		split_adjustments(&(*tree)->right);
	if ((*tree) && (*tree)->split)
	{
		i = 0;
		while ((*tree)->split[i])
		{
			if (countwords((*tree)->command_arr[i], 32) <= 1)
				(*tree)->split[i]->split_flag = 0;
			i++;
		}
	}
}

void command_arr_readjustments(t_tree **tree)
{
    int i, j, k, count;
    char **cmd;
    char *new_str;
    t_list *head = NULL;
    t_list *new_node;
    t_list *tmp;

    if ((*tree) && (*tree)->left)
        command_arr_readjustments(&(*tree)->left);
    if ((*tree) && (*tree)->right)
        command_arr_readjustments(&(*tree)->right);
    if ((*tree) && (*tree)->command_arr)
    {
        if ((*tree)->command_arr_expanded)
        {
            i = -1;
            while ((*tree)->command_arr_expanded[++i])
            {
                j = -1;
                while ((*tree)->command_arr_expanded[i][++j])
                {
                    free((*tree)->command_arr_expanded[i][j]);
                    (*tree)->command_arr_expanded[i][j] = NULL;
                }
                free((*tree)->command_arr_expanded[i]);
                (*tree)->command_arr_expanded[i] = NULL;
            }
            free((*tree)->command_arr_expanded);
            (*tree)->command_arr_expanded = NULL;
        }
        i = 0;
        while ((*tree)->command_arr[i])
            i++;
        (*tree)->command_arr_expanded = malloc(sizeof(char **) * (i + 1));
        if (!(*tree)->command_arr_expanded)
            return;
        if (!(*tree)->split)
		{
            (*tree)->split = malloc(sizeof(t_split *) * (i + 1));
            if (!(*tree)->split)
			{
                free((*tree)->command_arr_expanded);
                (*tree)->command_arr_expanded = NULL;
                return ;
            }
            j = 0;
            while (j < i) {
                (*tree)->split[j] = malloc(sizeof(t_split));
                if (!(*tree)->split[j])
				{
                    while (j > 0)
					{
                        j--;
                        free((*tree)->split[j]);
                    }
                    free((*tree)->split);
                    (*tree)->split = NULL;
                    free((*tree)->command_arr_expanded);
                    (*tree)->command_arr_expanded = NULL;
                    return ;
                }
                (*tree)->split[j]->arr_index = j;
                (*tree)->split[j]->split_flag = 0;
                j++;
            }
            (*tree)->split[i] = NULL;
        }
        i = 0;
        while ((*tree)->command_arr[i])
        {
            if ((*tree)->split && (*tree)->split[i] && 
                countwords((*tree)->command_arr[i], 32) > 1 && 
                (*tree)->split[i]->split_flag == 1)
            {
                (*tree)->command_arr_expanded[i] = ft_split((*tree)->command_arr[i], ' ');
            }
            else
            {
                cmd = malloc(sizeof(char *) * 2);
                if (!cmd)
				{
                    i++;
                    continue ;
                }
                cmd[0] = ft_strdup((*tree)->command_arr[i]);
                cmd[1] = NULL;
                (*tree)->command_arr_expanded[i] = cmd;
            }
            i++;
        }
        (*tree)->command_arr_expanded[i] = NULL;
        i = 0;
        count = 0;
        while ((*tree)->command_arr_expanded[i])
        {
            j = 0;
            while ((*tree)->command_arr_expanded[i][j])
            {
                count++;
                j++;
            }
            i++;
        }
        if (count == 0)
		{
            cmd = malloc(sizeof(char *));
            if (!cmd)
                return ;
            cmd[0] = NULL;
        }
		else
		{
            cmd = malloc(sizeof(char *) * (count + 1));
            if (!cmd)
                return ;
            i = 0;
            k = 0;
            while ((*tree)->command_arr_expanded[i])
            {
                j = 0;
                while ((*tree)->command_arr_expanded[i][j])
                {
                    cmd[k] = ft_strdup((*tree)->command_arr_expanded[i][j]);
                    k++;
                    j++;
                }
                i++;
            }
            cmd[k] = NULL;
            if (k > 0)
			{
                head = malloc(sizeof(t_list));
                if (!head)
				{
                    i = 0;
                    while (cmd[i])
					{
                        free(cmd[i]);
                        i++;
                    }
                    free(cmd);
                    return ;
                }
                head->data = ft_strdup(cmd[0]);
                head->token = NULL;
                head->prev = NULL;
                head->next = NULL;
                tmp = head;
                i = 1;
                while (cmd[i])
				{
                    new_node = malloc(sizeof(t_list));
                    if (!new_node)
					{
                        free_list(&head);
                        j = 0;
                        while (cmd[j])
						{
                            free(cmd[j]);
                            j++;
                        }
                        free(cmd);
                        return ;
                    }
                    new_node->data = ft_strdup(cmd[i]);
                    new_node->token = NULL;
                    new_node->prev = tmp;
                    new_node->next = NULL;
                    tmp->next = new_node;
                    tmp = new_node;
                    i++;
                }
                tmp = head;
                while (tmp)
				{
                    if (tmp->data && tmp->prev && tmp->next && 
                        tmp->prev->data && tmp->next->data &&
                        !ft_strcmp(tmp->data, "="))
                    {
                        new_str = ft_strjoin_three(tmp->prev->data, tmp->data, tmp->next->data);
                        free(tmp->data);
                        tmp->data = ft_strdup(new_str);
                        free(new_str);
                        free(tmp->prev->data);
                        tmp->prev->data = NULL;
                        free(tmp->next->data);
                        tmp->next->data = NULL;
                    }
                    tmp = tmp->next;
                }
            }
            i = 0;
            while (cmd[i])
			{
                free(cmd[i]);
                i++;
            }
            free(cmd);
            j = 0;
            if (head)
			{
                j = lst_size(&head);
            }
            cmd = malloc(sizeof(char *) * (j + 1));
            if (!cmd)
			{
                if (head)
                    free_list(&head);
                return ;
            }
            i = 0;
            if (head)
			{
                tmp = head;
                while (tmp)
				{
                    if (tmp->data)
					{
                        cmd[i] = ft_strdup(tmp->data);
                        i++;
                    }
                    tmp = tmp->next;
                }
                free_list(&head);
            }
            cmd[i] = NULL;
        }
        i = 0;
        while ((*tree)->command_arr[i])
		{
            free((*tree)->command_arr[i]);
            (*tree)->command_arr[i] = NULL;
            i++;
        }
        free((*tree)->command_arr);
        (*tree)->command_arr = cmd;
        printf("\n\n");
        i = 0;
        while ((*tree)->command_arr[i])
		{
            printf("%s\n", (*tree)->command_arr[i]);
            i++;
        }
        if ((*tree)->split)
		{
            i = 0;
            while ((*tree)->split[i])
			{
                free((*tree)->split[i]);
                i++;
            }
            free((*tree)->split);
        }
        i = 0;
        while ((*tree)->command_arr[i])
            i++;
        (*tree)->split = malloc(sizeof(t_split *) * (i + 1));
        if (!(*tree)->split)
            return ;
        i = 0;
        while ((*tree)->command_arr[i])
		{
            (*tree)->split[i] = malloc(sizeof(t_split));
            if (!(*tree)->split[i])
			{
                j = 0;
                while (j < i)
				{
                    free((*tree)->split[j]);
                    j++;
                }
                free((*tree)->split);
                (*tree)->split = NULL;
                return ;
            }
            (*tree)->split[i]->arr_index = i;
            (*tree)->split[i]->split_flag = 0;
            i++;
        }
        (*tree)->split[i] = NULL;
    }
}

int	main(int argc, char **argv, char **argev)
{
	char		*str;
	int			flag;
	t_env		*env;
	t_tree		*tree;

	atexit(f);
	((void)argc, (void)argv, inits_main(&env, &tree, argev));
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	while (1)
	{
		flag = 0;
		str = readline("minishell$> ");
		if (!str)
			break ;
		else if (!*str || check_empty(str))
		{
			free(str);
			continue ;
		}
		add_history(str);
		quote_parse(&str, &flag);
		lexer_to_tree(str, &tree, &flag);
		tree_to_rediropen(tree, &flag);
		var_set(&tree);
		if (!flag)
			reset_command_arr(&tree);
		redirections_list_maker(&tree);
		if (has_wild_cards_comarr(&tree) == 1)
			handle_wildcards_in_cmdarr(&tree);
		if (has_wild_cards_fdlst(&tree) == 1)
			handle_wildcards_in_fdlst(&tree);
		quote_remove_lst(&tree);
		quote_remove(&tree);
		if (variable_search(&tree) == 1) //TO EXPAND WITH IN EXECUTION THIS SEARCHES FOR VARIABLES AND THE NEXT ONE EXPANDS THEM
			variable_expantion(&tree, &env);
		if (variable_search_inlnkedlst(&tree) == 1)
			variable_expantion_inlnkedlst(&tree, &env);
		if (!flag)
			split_adjustments(&tree);
		if (!flag)
			command_arr_readjustments(&tree);
		quote_remove_two(&tree);
		quote_remove_lst_two(&tree);
		ambiguous_set(&tree);
		if (ambiguous_syntax_error(&tree, &env) == 1)
			(write(2, "ambiguous redirect\n", 19), flag = 1);
		if (ambiguous_syntax_error(&tree, &env) == 2)
			(write(2, "No such file or directory\n", 26), flag = 1);
		print_tree_visual(tree, 1, 1);
		// if (ambiguous_syntax_error(&tree, &env) != 2)
		// 	tree_empty_error(&tree, &flag);
		printf("*******************%d\n", flag);
		lasfree(&tree);
	}
	free_env(&env);
}
//(cat << EOF | grep "hello ") && (echo "skimi7a" || echo "fails") > infile.txt
//((ls > file) && echo | la > file2) > file3

//((ls)>file2) > file
// "((ls)>file2) > file"
// >file4(>file5 ls>file>file2>file3 -la>file6)>file7>file8
//> file4 (>file5 ls>file>file2>file3 -la>file6)>file7>file8make

//"*"