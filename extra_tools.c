/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:28:43 by makkach           #+#    #+#             */
/*   Updated: 2025/05/18 09:33:18 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tree_visual(t_tree *tree, int level, int is_left)
{
	if (!tree)
		return ;
	for (int i = 0; i < level; i++)
	{
		if (i == level - 1)
			printf("%s", is_left ? "----" : "    ");
		else
			printf("    ");
	}
	printf("%s", tree->type ? tree->type : "NULL");
	if (tree->command)
		printf(" (Command: %s)", tree->command);
	else
		printf(" (Command: NULL)");
	if (tree->ambiguous)
		printf(" (Ambiguous: %d)", tree->ambiguous);
	else
		printf(" (Ambiguous: 0)");
	// if (tree->quotes)
	// 	printf(" (Quotes: %d)", tree->quotes);
	// else
	// 	printf(" (Quotes: 0)");
	if (tree->var)
		printf(" (Var: %d)", tree->var);
	else
		printf(" (Var: 0)");
	if (tree->fd_list)
	{
		printf(" (Fd_list: %p)", tree->fd_list);
		for (t_list_fd *tmp = tree->fd_list; tmp; tmp = tmp->next)
		{
			printf(" (fd_list_redirs: %s)", tmp->name);
			if (tmp->name_split)
				printf(" (fd_list_redirs split: %s)", tmp->name_split[0]);
		}
	}
	else
		printf(" (Fd_list: 0)");
	if (tree->command_arr)
	{
		for (int i = 0; tree->command_arr[i]; i++)
		{
			// Handle special case for dollar sign
			if (tree->command_arr[i] && tree->command_arr[i][0] == '$')
				printf(" (Command_arr: \\$%s)", &(tree->command_arr[i][1]));
			else
				printf(" (Command_arr: %s)", tree->command_arr[i]);
		}
	}
	else
		printf(" (Command_arr: NULL)");
	if (tree->split)
	{
		for (int i = 0; tree->command_arr[i]; i++)
		{
			printf(" (Split idx: %d)", tree->split[i]->arr_index); // Always print the index
			printf(" (Split flg: %d)", tree->split[i]->split_flag); // Always print the flag
		}
	}
	else
		printf(" (Split: NULL)");
	if (tree->command_arr_expanded)
	{
		printf(" (Command_arr_expanded: %p)", tree->command_arr_expanded);
		// for (int i = 0; tree->command_arr_expanded[i]; i++)
		// {
		// 	if (tree->command_arr_expanded[i])
		// 	{
		// 		for (int j = 0; tree->command_arr_expanded[i][j]; j++)
		// 			printf(" (Command_arr_expanded: %s i = %d j = %d)", tree->command_arr_expanded[i][j], i, j);
		// 	}
		// }
	}
	else
		printf(" (Command_arr_expanded: NULL)");
	if (tree->redirections)
		printf(" [Redirections: %s]", tree->redirections);
	printf("\n");
	if (tree->left || tree->right)
	{
		for (int i = 0; i < level; i++)
			printf("    ");
		printf("---left;\n");
	}
	if (tree->left)
		print_tree_visual(tree->left, level + 1, 1);
	if (tree->right)
	{
		for (int i = 0; i < level; i++)
			printf("    ");
		printf("---right;\n");
	}
	if (tree->right)
		print_tree_visual(tree->right, level + 1, 0);
}
