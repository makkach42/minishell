/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:28:43 by makkach           #+#    #+#             */
/*   Updated: 2025/04/29 10:55:09 by makkach          ###   ########.fr       */
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
	if (tree->quotes)
		printf(" (Quotes: %d)", tree->quotes);
	else
		printf(" (Quotes: 0)");
	if (tree->command_arr)
	{
		for (int i = 0; tree->command_arr[i]; i++)
		{
			printf(" (Command_arr: %s)", tree->command_arr[i]);
		}
	}
	else
		printf(" (Command_arr: NULL)");
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
