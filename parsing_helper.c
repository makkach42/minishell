/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:52:32 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/01 14:10:19 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	protect_wild_card(t_tree **tree)
{
	int		i;
	char	*tmp;

	if ((*tree) && (*tree)->left)
		protect_wild_card(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		protect_wild_card(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
	{
		i = 1;
		while ((*tree)->command_arr[i])
		{
			if (if_has_wildcards((*tree)->command_arr[i]))
			{
				tmp = (*tree)->command_arr[i];
				(*tree)->command_arr[i] = ft_strjoin_three("\"",
						(*tree)->command_arr[i], "\"");
				free(tmp);
			}
			i++;
		}
	}
}

void	ft_execution(t_tree *tree, t_hdoc_data *h_data, int *check, char **e)
{
	static int	status;
	int			test;

	h_data->hdoc_num = 0;
	test = 0;
	tree->status = status;
	h_data->hdoc_num = ft_hdoc_count(tree);
	if (h_data->hdoc_num > 16)
		return (ft_hdoc_exed());
	ft_hdoc_handle(tree, h_data, tree->status);
	ft_st(tree, *(h_data->sig_flag));
	ft_exec_test(tree, &test, h_data);
	if (*(h_data->sig_flag))
	{
		ft_execute(tree, e, check, h_data);
		ft_signal_exec();
	}
	else
		tree->status = 1;
	if (tree->status >= 0)
		h_data->check_stat = 1;
	else
		h_data->check_stat = 0;
	status = tree->status;
	h_data->end = status;
}

void	execution(t_var_main *shell)
{
	shell->tree = NULL;
	g_global_status = 0;
	ft_parsing(&shell->str, &shell->flag, &shell->tree, shell->h_data);
	if (!shell->flag)
		ft_execution(shell->tree, shell->h_data, &shell->check, shell->e);
	if (shell->flag)
		shell->h_data->check_stat = 0;
	tcsetattr(0, TCSANOW, &shell->termios_a);
}

void	ft_str_empty(t_env **env, char **e, t_hdoc_data *h_data)
{
	ft_putstr_fd(1, "exit\n");
	ft_free_array(e);
	free_env(env);
	ft_free_data(h_data);
}

void	ft_hdoc_exed(void)
{
	ft_putstr_fd(2, "minishell: maximum here-document count exceeded\n");
	exit (2);
}
