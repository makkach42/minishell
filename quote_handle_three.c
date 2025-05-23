/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handle_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:29:57 by makkach           #+#    #+#             */
/*   Updated: 2025/05/18 10:49:35 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_command_array_two(t_tree **tree)
{
	int	k;

	k = 0;
	while ((*tree)->command_arr[k])
	{
		if (variable_search_instr((*tree)->command_arr[k]))
			process_command_string(tree, k);
		k++;
	}
}

void	quote_remove(t_tree **tree)
{
	if ((*tree) && (*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
		process_command_array(tree);
}

void	quote_remove_two(t_tree **tree)
{
	if ((*tree) && (*tree)->left)
		quote_remove_two(&(*tree)->left);
	if ((*tree) && (*tree)->right)
		quote_remove_two(&(*tree)->right);
	if ((*tree) && (*tree)->command_arr)
		process_command_array_two(tree);
}

// char	*create_filtered_string_preserve_var(char *old_str, int final_len, int is_var)
// {
// 	int		i;
// 	int		j;
// 	char	*new_str;
// 	int		in_single_quotes;
// 	int		in_double_quotes;

// 	new_str = (char *)malloc(sizeof(char) * (final_len + 1));
// 	if (!new_str)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	in_single_quotes = 0;
// 	in_double_quotes = 0;
// 	while (i < (int)ft_strlen(old_str))
// 	{
// 		if (is_var)
// 		{
// 			new_str[j++] = old_str[i];
// 		}
// 		else
// 		{
// 			if (old_str[i] == '\'' && !in_double_quotes)
// 				in_single_quotes = !in_single_quotes;
// 			else if (old_str[i] == '\"' && !in_single_quotes)
// 				in_double_quotes = !in_double_quotes;
// 			else
// 				new_str[j++] = old_str[i];
// 		}
// 		i++;
// 	}
// 	new_str[j] = '\0';
// 	return (new_str);
// }

// void	process_command_string_preserve_var(t_tree **tree, int k)
// {
// 	char	*old_str;
// 	char	*new_str;
// 	int		final_len;
// 	int		is_var;

// 	old_str = (*tree)->command_arr[k];
// 	final_len = ft_strlen(old_str);
// 	is_var = (*tree)->var;
// 	new_str = create_filtered_string_preserve_var(old_str, final_len, is_var);
// 	if (!new_str)
// 		return ;
// 	free((*tree)->command_arr[k]);
// 	(*tree)->command_arr[k] = new_str;
// }

// void	process_command_array_preserve_var(t_tree **tree)
// {
// 	int	k;

// 	k = 0;
// 	while ((*tree)->command_arr[k])
// 	{
// 		process_command_string_preserve_var(tree, k);
// 		k++;
// 	}
// }

// void	quote_remove_preserve_var(t_tree **tree)
// {
// 	if ((*tree) && (*tree)->left)
// 		quote_remove_preserve_var(&(*tree)->left);
// 	if ((*tree) && (*tree)->right)
// 		quote_remove_preserve_var(&(*tree)->right);
// 	if ((*tree) && (*tree)->command_arr)
// 		process_command_array_preserve_var(tree);
// }


// int	is_var_expansion_in_quotes(char *str)
// {
// 	int len = ft_strlen(str);
	
// 	// Check if the string starts and ends with double quotes
// 	if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
// 		// Check if the content between quotes has single quotes
// 		// which means it likely came from a variable expansion
// 		for (int i = 1; i < len - 1; i++) {
// 			if (str[i] == '\'') {
// 				return 1;
// 			}
// 		}
// 	}
// 	return 0;
// }

// /*
//  * Modified version that properly handles quotes in both normal strings and expanded variables
//  */
// char	*create_filtered_string_improved(char *old_str, int final_len)
// {
// 	int		i;
// 	int		j;
// 	char	*new_str;
// 	int		in_single_quotes;
// 	int		in_double_quotes;
// 	int		is_var_in_quotes;

// 	is_var_in_quotes = is_var_expansion_in_quotes(old_str);
	
// 	new_str = (char *)malloc(sizeof(char) * (final_len + 1));
// 	if (!new_str)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	in_single_quotes = 0;
// 	in_double_quotes = 0;
	
// 	while (i < (int)ft_strlen(old_str))
// 	{
// 		if (is_var_in_quotes) {
// 			// For expanded variables in double quotes, we want to:
// 			// 1. Remove the outer double quotes
// 			// 2. Preserve everything else including any single quotes inside
// 			if ((i == 0 || i == (int)ft_strlen(old_str) - 1) && old_str[i] == '"') {
// 				// Skip the outer double quotes
// 				i++;
// 				continue;
// 			}
// 			// Keep all other characters
// 			new_str[j++] = old_str[i];
// 		} else {
// 			// For normal strings, remove quotes as before
// 			if (old_str[i] == '\'' && !in_double_quotes)
// 				in_single_quotes = !in_single_quotes;
// 			else if (old_str[i] == '\"' && !in_single_quotes)
// 				in_double_quotes = !in_double_quotes;
// 			else
// 				new_str[j++] = old_str[i];
// 		}
// 		i++;
// 	}
// 	new_str[j] = '\0';
// 	return (new_str);
// }

// /*
//  * Modified version of process_command_string with improved quote handling
//  */
// void	process_command_string_improved(t_tree **tree, int k)
// {
// 	char	*old_str;
// 	char	*new_str;
// 	int		final_len;

// 	old_str = (*tree)->command_arr[k];
// 	// Calculate the maximum possible length of the result string
// 	// For variable expansions in quotes, we need to subtract 2 for the outer quotes
// 	if (is_var_expansion_in_quotes(old_str))
// 		final_len = ft_strlen(old_str) - 2;
// 	else
// 		final_len = count_filtered_length(old_str);
	
// 	new_str = create_filtered_string_improved(old_str, final_len);
// 	if (!new_str)
// 		return;
		
// 	free((*tree)->command_arr[k]);
// 	(*tree)->command_arr[k] = new_str;
// }

// /*
//  * Modified version of process_command_array that uses the improved function
//  */
// void	process_command_array_improved(t_tree **tree)
// {
// 	int	k;

// 	k = 0;
// 	while ((*tree)->command_arr[k])
// 	{
// 		process_command_string_improved(tree, k);
// 		k++;
// 	}
// }

// /*
//  * Modified version of quote_remove that uses the improved functions
//  */
// void	quote_remove_improved(t_tree **tree)
// {
// 	if ((*tree) && (*tree)->left)
// 		quote_remove_improved(&(*tree)->left);
// 	if ((*tree) && (*tree)->right)
// 		quote_remove_improved(&(*tree)->right);
// 	if ((*tree) && (*tree)->command_arr)
// 		process_command_array_improved(tree);
// }