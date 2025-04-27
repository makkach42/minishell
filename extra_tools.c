/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:28:43 by makkach           #+#    #+#             */
/*   Updated: 2025/04/27 14:54:24 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tree_visual(t_tree *tree, int level, int is_left)
{
	int	indent_index;
	int	arr_index;
	int	marker_index;

	if (!tree)
		return ;
	indent_index = 0;
	while (indent_index < level)
	{
		if (indent_index == level - 1)
		{
			if (is_left)
				printf("----");
			else
				printf("    ");
		}
		else
			printf("    ");
		indent_index++;
	}
	if (tree->type)
		printf("%s", tree->type);
	else
		printf("NULL");
	if (tree->command)
		printf(" (Command: %s)", tree->command);
	else
		printf(" (Command: NULL)");
	if (tree->command_arr)
	{
		arr_index = 0;
		while (tree->command_arr[arr_index])
		{
			printf(" (Command_arr: %s)", tree->command_arr[arr_index]);
			arr_index++;
		}
	}
	else
		printf(" (Command_arr: NULL)");
	if (tree->redirections)
		printf(" [Redirections: %s]", tree->redirections);
	if (tree->left || tree->right)
	{
		marker_index = 0;
		while (marker_index < level)
		{
			printf("    ");
			marker_index++;
		}
		printf("---left;\n");
	}
	if (tree->left)
		print_tree_visual(tree->left, level + 1, 1);
	if (tree->right)
	{
		marker_index = 0;
		while (marker_index < level)
		{
			printf("    ");
			marker_index++;
		}
		printf("---right;\n");
	}
	if (tree->right)
		print_tree_visual(tree->right, level + 1, 0);
}
