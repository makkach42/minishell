/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:46:34 by makkach           #+#    #+#             */
/*   Updated: 2025/06/01 12:04:26 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <termios.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_list
{
	char			*data;
	char			*token;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

typedef struct s_list_fd
{
	int					fd;
	int					in_quotes;
	char				*name;
	char				**name_split;
	char				*command;
	char				*redir;
	struct s_list_fd	*next;
}	t_list_fd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_tree
{
	char			*type;
	struct s_tree	*left;
	struct s_tree	*right;
	char			*command;
	char			**command_arr;
	char			***command_arr_expanded;
	char			*redirections;
	int				ambiguous;
	int				quotes;
	int				var;
	t_list_fd		*fd_list;
}	t_tree;

typedef struct s_dynbuf
{
	char	*data;
	size_t	size;
	size_t	pos;
}	t_dynbuf;

typedef struct s_extract
{
	t_dynbuf	command_buf;
	t_dynbuf	redir_buf;
	int			i;
	int			in_quotes;
	char		quote_type;
	int			paren_count;
}	t_extract;

typedef struct s_idx
{
	int	i;
	int	j;
	int	original_size;
}	t_idx;

void	print_tree_visual(t_tree *tree, int level, int is_left);
int		ft_strcmp(char *s1, char *s2);
size_t	ft_strlen(char *str);
int		variable_search_instr(char *str);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strtrim(char *s1, char *set);
char	*ft_strdup(char *s1);
int		lst_size(t_list **head);
t_list	*list_init(char *str);
char	*ft_strchr(char *str, int c);
int		word_recognizer(char *str);
char	*str_extractor(char *str);
void	free_list(t_list **head);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strjoin_three(char *s1, char *s2, char *s3);
void	redirections_list_maker(t_tree **tree);
void	tree_to_rediropen(t_tree *tree, int *flag);
void	lexer_to_tree(char *str, t_tree **tree, int *flag);
void	quote_parse(char **str, int *flag);
int		check_empty(char *str);
void	handle_signal(int sig);
void	inits_main(t_env **env,
			t_tree **tree, char **argev);
char	*create_filtered_string(char *old_str, int final_len);
int		count_filtered_length(char *old_str);
int		process_array_variable(char **command_arr,
			int arr_idx, int *var_pos, t_env **env);
int		is_operator(char c);
char	**ft_split(char *s, char c);
int		countwords(char *s, char c);
void	free_env(t_env **env);
void	lasfree(t_tree **tree);
int		ambiguous_syntax_error(t_tree **tree, t_env **env);
void	quote_remove_lst(t_tree **tree);
void	ambiguous_set(t_tree **tree);
void	variable_expantion_inlnkedlst(t_tree **tree, t_env **env);
int		variable_search_inlnkedlst(t_tree **tree);
void	handle_wildcards_in_cmdarr(t_tree **tree);
// void	handle_wildcards_in_fdlst(t_tree **tree);
// int		has_wild_cards_fdlst(t_tree **tree);
int		has_wild_cards_comarr(t_tree **tree);
int		operation_recognizer(char *str);
int		redirection_recognizer(char *str);
int		pipe_recognizer(char *str);
int		parenthasis_recognizer(char *str);
int		variable_recognizer(char *str);
int		command_recognizer(char *str);
void	if_string_while_loop(int *i, char *str, char **word);
void	if_par_loop(int *i, char *str, char **word);
void	handle_quotes(char **word, char **str);
void	handle_operator(char **word, char **str);
void	variable_handle(char **word, char **str);
void	handle_parenthases(char **word, char **str);
void	handle_else(char **word, char **str);
void	init_tree_node(t_tree **node, char *type);
char	*side_maker(t_list **head, int number, int j);
void	process_operation_node(t_list **head, t_tree **tree, t_list *tmp,
			int *node_info);
void	process_pipe_node(t_list **head, t_tree **tree, t_list *tmp,
			int *node_info);
int		count_total_nodes(t_list *head, t_list **last);
void	update_tree_with_cmd(t_tree *tree, t_tree *cmd_tree);
int		check_pipe_or_and(const char *str, int i, char quote);
void	free_cmd_list(t_list *cmd_list);
void	lexer(t_list **head);
void	process_command_with_pipes_inits(t_list **cmd_list, char **cmd_copy);
t_tree	*handle_operation_token(t_list **head, t_list *pipe_pos);
t_tree	*handle_pipe_token(t_list **head, char *left_cmd,
			t_list *pipe_pos);
void	free_list_to_position(t_list **head, t_list *position);
t_tree	*create_tree_node(void *command, char *type);
void	build_command_str(char **command_str, t_list *current);
void	redirections_in_par_handle(t_tree **tree, char **cmd_part,
			char **original_redirs, char **content);
void	sub_tree_creation(t_tree **sub_tree, char **content, t_list **sub_list);
void	sub_tree_attachment(t_tree **sub_tree, t_tree **tree,
			char **original_redirs);
void	free_all(char **content, char **original_redirs);
int		initialize_buffers(t_extract *v, char *cmd_str);
void	init_extraction_vars(int *i, int *in_quotes, char *quote_type,
			int *paren_count);
int		process_command_string_two(t_extract *v, char *cmd_str);
void	dyn_buf_free(t_dynbuf *buf);
void	cleanup_and_assign(t_dynbuf *command_buf, t_dynbuf *redir_buf,
			char **cmd_part, char **redir_part);
void	init_list_fd_node(t_list_fd *node);
void	print_syntax_error(char *str2);
void	syntax_error_parentheses_while_loop(t_list *tmp,
			int *i, int *flag, int *open_par);
int		check_quotes(char *str, int *flag);
int		no_words_beside(char *str);
void	even_more_ifs(char *prev_token,
			char *prev_data, t_list *tmp, int *flag);
char	*replace_whites_spaces(char *str);
t_env	*env_fill(char **argev);
void	syntax_error_two(t_tree **tree, int *flag);
void	command_arr_fill(t_tree **tree);
void	process_all_redirections(t_tree **tree);
void	process_pipe_trees(t_tree *tree);
void	process_nested_parentheses(t_tree **tree);
void	tree_maker(t_list **head, t_tree **tree);
int		syntax_error_parentheses(t_list **head);
void	syntax_error(t_list **head, int *flag);
int		new_syntax_error(t_list **head);
void	open_quotes(int	*in_quotes, char *quote_type, char **str, int *i);
void	parenthesis_recognizer_helper(int *open_par,
			int *closed_par, int *i, char *str);
char	*first_word_remover(char *str, char *word);
char	*word_extractor(char *str);
char	*parenthesis_remover(char *str, char *word);
char	*extract_parenthesis(char *str);
char	*first_word_remover(char *str, char *word);
char	*extract_variable(char *str);
char	*remove_operator(char *str, char *word);
char	*extract_operator(char *str);
char	*str_remover(char *str, char *word);
char	*extract_content_from_parentheses(char *command);
void	redirections_handle(char **original_redirs, t_tree **tree);
void	extract_redirections(char *cmd_str, char **cmd_part, char **redir_part);
void	free_tree(t_tree *tree);
int		still_has_parentheses(t_tree **tree);
void	cleanup_right_nodes(t_list *nodes);
void	free_node_data(t_list *node);
void	cleanup_left_nodes(t_list *nodes);
void	if_prev(t_list *tmp, t_list **prev_part, t_tree **tree, t_list **head);
void	handle_operation_command(t_tree **tree, t_list *tmp,
			int total_nodes, int i);
t_tree	*build_pipe_tree(t_list **head);
t_list	*copy_list_segment(t_list *head, t_list *end_pos);
int		dyn_buf_add_char(t_dynbuf *buf, char c);
int		process_redirection_helper(char *cmd_str, int *i, t_dynbuf *redir_buf);
void	dyn_buf_finalize(t_dynbuf *buf);
void	handle_quotes_and_parens(char c, int *in_quotes, char *quote_type,
			int *paren_count);
int		process_redirection(char *cmd_str, int *i, t_dynbuf *redir_buf);
int		dyn_buf_init(t_dynbuf *buf, size_t initial_capacity);
int		is_command_char(char c, int in_quotes);
int		if_has_wildcards(char *str);
void	wild_cards_handle_cmdarr(char ***cmd_arr, char *dir_path);
// void	process_wildcard_node_fd(t_list_fd *node, char *dir_path);
int		fd_list_size(t_list_fd **head);
void	sort_matches(char **arr, int count);
int		copy_and_resize_matches(char ***matches, int match_count, int new_cap);
int		match_pattern(const char *pattern, const char *string);
char	*str_duplicate(const char *s);
char	*join_matches(char **matches, int match_count);
char	*ft_strcpy(char *dest, char *src);
void	quote_remove_two(t_tree **tree);
void	process_command_array(t_tree **tree);
// void	process_command_string(t_tree **tree, int k);
t_list	*new_list_init(char *str);
void	reset_vars(t_tree **tree, t_env **env);
int		isnt_valid(char *str);
void	export_cases(t_tree **tree);
// void	new_quote_handle(char **str, char **word);
void	new_parenthasis_handle(char **str, char **word);
void	new_variable_handle(char **str, char **word);
// void	plus_handle(char **str, char **word);
void	new_operator_handle(char **str, char **word);
void	new_word_handle(char **str, char **word);
// void	new_quote_word_extractor(char **word, char **str, int i);
void	new_parenthasis_word_extractor(char **word, int i, char **str);
void	new_variable_word_extractor(char **str, char **word, int i);
// void	plus_handle_word_extractor(char **str, char **word);
void	new_operator_word_extractor(char **word, char **str);
void	equal_handle(char **str, char **word);
void	reset_var_expand(t_list	*tmp, t_env **env);
void	reset_var_expand_var(t_tree **tree, t_env **env);
void	process_no_list_size(t_list	*head, int list_size, char ***cmd2, int *i);
char	***cmd2_fill(t_tree **tree);
void	free_twod_char(char **arr);
int		count_triple_ptr(char ***cmd2);
void	fill_twod_char_with_triple(char **arr, char ***cmd2);
void	str_extractor_helper(char *str, int *i, int *flag, char **word);
int		if_var_end_is_dollar(char **command_arr,
			int *var_pos, int arr_idx, int var_end);
void	var_end_set(char **command_arr, int arr_idx, int *var_end);
void	extract_var_value(t_env **env, char *var_name, char **var_value);
int		init_matches(char ***matches, DIR **dir,
			char *dir_path, int *capacity);
void	pattern_match_inits(int *i, int *j, int *star_idx, int *str_idx);
int		skip_stars(int *i, const char *pattern);
void	update_vars(int *star_idx, int *i, int *str_idx, int *j);
void	back_track(int *i, int *j, int *star_idx, int *str_idx);
void	update_vars_two(int *i, int *j);
int		count_pattern_matches(const char *pattern, char *dir_path);
int		new_size_calcs(int original_size, char ***cmd_arr,
			char *dir_path, int *match_count);
void	if_expanded_matches(char ***new_cmd_arr,
			char **expanded_matches, int match_count, int *j);
int		count_cmd_arr_size(char **cmd_arr);
char	**init_new_cmd_arr(int new_size);
void	process_wildcard_element(char ***new_cmd_arr, char *element,
			char *dir_path, int *j);
void	free_original_cmd_arr(char ***cmd_arr, int original_size);
void	process_regular_element(char ***new_cmd_arr, char *element, int *j);
char	**get_matches(const char *pattern, char *dir_path, int *match_count);
void	process_wildcard_node_fd(t_list_fd *node, char *dir_path);
void	handle_wildcards_in_fdlst(t_tree **tree);
int		has_wild_cards_fdlst(t_tree **tree);

#endif