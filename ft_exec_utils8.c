/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils8.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:18:39 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/03 16:10:50 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_hdoc_handle(t_tree *tree, t_hdoc_data *h_data, int status)
{
	if (!tree)
		return ;
	if (tree->left)
		ft_hdoc_handle(tree->left, h_data, status);
	if (tree->right)
		ft_hdoc_handle(tree->right, h_data, status);
	ft_hdoc_check(tree, h_data, status);
}

char	*ft_node_joint(t_env *h)
{
	char	*str;
	char	*temp;

	str = ft_strjoin(h->key, "=");
	if (!str)
		return (NULL);
	temp = str;
	str = ft_strjoin(str, h->value);
	if (!str)
		return (NULL);
	free (temp);
	return (str);
}

char	**ft_env_str(t_env *h)
{
	char	**p;
	int		i;
	int		lst_lent;

	i = 0;
	if (h == NULL)
		return (NULL);
	lst_lent = ft_lstsize(h);
	p = malloc(sizeof(char *) * (lst_lent + 1));
	if (p == NULL)
	{
		free_env(&h);
		exit (1);
	}
	while (i < lst_lent && h)
	{
		p[i] = ft_node_joint(h);
		h = h->next;
		i++;
	}
	p[i] = NULL;
	return (p);
}

void	ft_st(t_tree *tree, int stat_flag)
{
	if (stat_flag == 0)
	{
		ft_close_fd(tree);
		tree->status = 1;
	}
}

void	ft_signal_exec(void)
{
	if (g_global_status == 2)
		ft_putstr_fd(1, "\n");
	else if (g_global_status == 3)
		ft_putstr_fd(1, "Quit: 3\n");
	g_global_status = 0;
}
