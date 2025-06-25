/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:46:34 by makkach           #+#    #+#             */
/*   Updated: 2025/06/24 17:25:54 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <dirent.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <errno.h>
# include <signal.h>

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
	int					fd1;
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
	int				active;
	int				h;
}	t_env;

typedef struct s_tree
{
	char			*type;
	struct s_tree	*left;
	struct s_tree	*right;
	char			*command;
	char			**command_arr;
	char			*redirections;
	int				ambiguous;
	// int				quotes;
	int				var;
	t_list_fd		*fd_list;
	int				status;
	int				sig_flag;
	int				fd[2];
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

typedef struct s_hdoc_data
{
	t_env	**env;
	int		*sig_flag;
	int		prev_fd;
	int		stat;
	int		check_stat;
} t_hdoc_data;

typedef struct s_cmd
{
	char	**p;
	char	**t;
	char	*str;
	char	*cmd;
	int		i;
}	t_cmd;

typedef struct s_idx
{
	int	i;
	int	j;
	int	original_size;
}	t_idx;

typedef struct s_id
{
	int id;
	struct s_id *next;
} t_id;

typedef	struct s_tmp_tree
{
	t_list	*tmp;
	t_tree	*tree;
}	t_tmp_tree;

int	global_status;

t_env		*env_fill(char **argev);
t_list		*list_init(char *str);
int			syntax_error_parentheses(t_list **head);
int			lst_size(t_list **head);
char		*replace_whites_spaces(char *str);
void		tree_maker(t_list **head, t_tree **tree);
void		lexer(t_list **head);
char		*word_extractor(char *str);
char		*first_word_remover(char *str, char *word);
int			is_operator(char c);
char		*str_extractor(char *str);
char		*str_remover(char *str, char *word);
char		*extract_parenthesis(char *str);
char		*parenthesis_remover(char *str, char *word);
char		*extract_variable(char *str);
char		*remove_operator(char *str, char *word);
char		*extract_operator(char *str);
void		process_pipe_trees(t_tree *tree);
void		process_nested_parentheses(t_tree **tree);
void		process_all_redirections(t_tree **tree);
void		extract_redirections(char *cmd_str,
				char **cmd_part, char **redir_part);
char		*extract_content_from_parentheses(char *command);
void		syntax_error_two(t_tree **tree, int *flag);
void		syntax_error(t_list **head, int *flag);
void		even_more_ifs(char *prev_token, char *prev_data,
				t_list *tmp, int *flag);
void		lexer_to_tree(char *str, t_tree **tree, int *flag, t_hdoc_data *data);
void		tree_to_rediropen(t_tree *tree, int *flag);
void		inits_main(t_env **env,
				t_tree **tree, char **argev);
void		command_arr_fill(t_tree **tree);
void		quote_remove(t_tree **tree);
void		quote_remove_two(t_tree **tree);
void		free_tree(t_tree *tree);
void		free_list_fd(t_list_fd **head);
int			redirection_recognizer(char *str);
int			parenthasis_recognizer(char *str);
void		free_list(t_list **head);
void		lasfree(t_tree **tree);
void		free_env(t_env **env);
int			variable_search(t_tree **tree);
// void		variable_expantion(t_list **head, t_env **env);
void		variable_expantion(t_tree **tree, t_env **env);
int			variable_recognizer(char *str);
int			operation_recognizer(char *str);
int			pipe_recognizer(char *str);
int			word_recognizer(char *str);
int			command_recognizer(char *str);
int			variable_in_word(t_list **head, t_env **env);
char		*ft_strdup(char *s1);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strtrim(char *s1, char *set);
size_t		ft_strlen(char *str);
char		*ft_substr(char *s, unsigned int start, size_t len);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strchr(char *str, int c);
char		*ft_strncpy(char *dest, char *src, int n);
char		*ft_strcpy(char *dest, char *src);
char		*ft_strcat(char *dest, const char *src);
int			ft_strncmp(char *str1, char *str2, size_t n);
void		t_free(void *ptr, int line, char *file);
char		*ft_strstr(const char *haystack, const char *needle);
void		print_tree_visual(t_tree *tree, int level, int is_left);
void		handle_quotes(char **word, char **str);
void		handle_operator(char **word, char **str);
void		variable_handle(char **word, char **str);
void		handle_parenthases(char **word, char **str);
void		handle_else(char **word, char **str);
void		open_quotes(int	*in_quotes, char *quote_type, char **str, int *i);
void		parenthesis_recognizer_helper(int *open_par,
				int *closed_par, int *i, char *str);
void		sub_tree_attachment(t_tree **sub_tree, t_tree **tree,
				char **original_redirs);
void		sub_tree_creation(t_tree **sub_tree,
				char **content, t_list **sub_list);
void		free_all(char **content, char **original_redirs);
void		redirections_in_par_handle(t_tree **tree, char **cmd_part,
				char **original_redirs, char **content);
int			still_has_parentheses(t_tree **tree);
void		redirections_handle(char **original_redirs, t_tree **tree);
// void		print_syntax_error(char *str2);
void		syntax_error_parentheses_while_loop_parentheses(t_list *tmp,
				int *i, int *open_par, int *closed_par);
void		syntax_error_parentheses_while_loop(t_list *tmp,
				int *i, int *flag, int *open_par);
void		if_par_loop(int *i, char *str, char **word);
void		if_string_while_loop(int *i, char *str, char **word);
char		*side_maker(t_list **head, int number, int j);
void		init_tree_node(t_tree **node, char *type);
void		free_node_data(t_list *node);
void		cleanup_right_nodes(t_list *nodes);
void		cleanup_left_nodes(t_list *nodes);
// t_tree		*create_command_node(t_list **node_list, int offset);
int			count_total_nodes(t_list *head, t_list **last);
void		if_prev(t_list *tmp, t_list **prev_part,
				t_tree **tree, t_list **head);
// t_tree		*setup_operation_command(t_list **next_list, int offset);
void		handle_operation_command(t_tree **tree, t_list *tmp,
				int total_nodes, int i);
void		process_operation_node(t_list **head, t_tree **tree, t_list *tmp,
				int *node_info);
void		process_pipe_node(t_list **head, t_tree **tree, t_list *tmp,
				int *node_info);
int			skip_spaces(char *str, int start);
int			check_quotes(char *str, int *flag);
t_tree		*build_pipe_tree(t_list **head);
t_tree		*create_tree_node(void *command, char *type);
void		build_command_str(char **command_str, t_list *current);
void		free_list_to_position(t_list **head, t_list *position);
t_list		*copy_list_segment(t_list *head, t_list *end_pos);
t_tree		*handle_pipe_token(t_list **head, char *left_cmd,
				t_list *pipe_pos);
t_tree		*handle_operation_token(t_list **head, t_list *pipe_pos);
void		free_cmd_list(t_list *cmd_list);
void		process_command_with_pipes_inits(t_list **cmd_list,
				char **cmd_copy);
int			check_pipe_or_amp(const char *str, int i, char quote);
void		update_tree_with_cmd(t_tree *tree, t_tree *cmd_tree);
void		update_quote_state(char c, int *in_quotes, char *quote_type);
void		join_cmd_with_args(char **cmd_part, char *args_start,
				char *redir_start);
// int			count_filtered_length(char *old_str, int *var_flag, int l, int k);
int			process_variable(t_list *tmp, int i, t_env **env);
int			process_word_variable(t_list *tmp, t_env **env);
char		*check_for_valid_args(char *redir_start, int j);
int			handle_space_after_redir(char *redir_start, int i, char **args);
int			update_redir_state(char *redir_start, int i, int *redir_active);
int			check_spaces_and_redirs(char *str, int i, int *redir_active);
int			is_command_char(char c, int in_quotes);
int			skip_spaces_in_str(char *str, int start);
void		init_extraction_vars(int *i, int *in_quotes, char *quote_type,
				int *paren_count);
void		handle_quotes_and_parens(char c, int *in_quotes, char *quote_type,
				int *paren_count);
int			process_redirection(char *cmd_str, int *i, t_dynbuf *redir_buf);
// int			process_regular_char(char c, t_dynbuf *command_buf,
				// int in_quotes_or_paren);
void		cleanup_and_assign(t_dynbuf *command_buf, t_dynbuf *redir_buf,
				char **cmd_part, char **redir_part);
int			initialize_buffers(t_extract *v, char *cmd_str);
// int			process_command_char(t_extract *v, char *cmd_str);
int			process_command_string_two(t_extract *v, char *cmd_str);
int			process_redirection_helper(char *cmd_str, int *i,
				t_dynbuf *redir_buf);
int			determine_redirection_flag(char *redirection, int i);
void		skip_redirection_and_spaces(char *redirection, int *i);
// void		extract_filename(char **old_redirs, char **target_name);
void		set_command_for_node(t_tree *tree, t_list_fd *new_node);
void		set_redirection_type(t_list_fd *new_node, int flag);
t_list_fd	*process_single_redirection(t_tree *tree,
				char *redirection, int *pos);
int			skip_to_next_redirection(char *redirections, int i);
int			initialize_redirection_processing(t_tree *tree,
				char **redirections_copy);
int			process_first_redirection(t_tree *tree, t_list_fd **head,
				char *redirections_copy, int *i);
void		append_new_redirection(t_list_fd **tmp, t_tree *tree,
				char *redirections_copy, int *i);
int			check_empty(char *str);
void		quote_parse(char **str, int *flag);
int			countwords(char *s, char c);
void		ambiguous_set(t_tree **tree);
int			variable_search_instr(char *str);
int			new_syntax_error(t_list **head);
int			ambiguous_syntax_error(t_tree **tree, t_env **env);
void		quote_set(t_tree **tree);
void		tree_empty_error(t_tree **tree, int *flag);
void		redirections_list_maker(t_tree **tree);
char	*ft_cmd_check(char *env, char *s);
int		ft_parse(char *s);
long	ft_atoi(const char *str);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
void    ft_empty_list(t_env *h, char **env);
void	ft_lstadd_front(t_env **lst, t_env *new);
t_env	*ft_check(t_env *h, char *str);
t_env	*ft_lstnew(void *key, void *value);
void	ft_lstadd_back(t_env **lst, t_env *new);
void	ft_cmd_helper(t_cmd *com, int i, char *env, char *s);
char	*ft_strmcpy(char *src);
int		ft_is_spaces(char *s);
int		ft_cd(char **s, t_env *h);
int		ft_echo(char **s);
int		ft_env(t_env *h);
int		ft_check_string(char *str);
int		ft_exit(char **s, t_env **h, int status);
int		ft_export(char  **s, t_env *h, t_tree *tree);
int		ft_pwd(t_env *h);
int		ft_unset(t_env **h, char **s);
int	ft_file_check(char *str);
int	ft_file_create(char *str, int n);
void	ft_putstr_fd(int fd, char *s);
int	ft_lstsize(t_env *lst);
void		init_list_fd_node(t_list_fd *node);
void		if_env_value(t_list *tmp, char **env_value);
void		dyn_buf_free(t_dynbuf *buf);
int			dyn_buf_add_char(t_dynbuf *buf, char c);
void		dyn_buf_finalize(t_dynbuf *buf);
int			dyn_buf_init(t_dynbuf *buf, size_t initial_capacity);
void		if_expandable(t_list *tmp, t_env **env);
int			variable_search_inlnkedlst(t_tree **tree);
char		*ft_strjoin_three(char *s1, char *s2, char *s3);
void		variable_expantion_inlnkedlst(t_tree **tree, t_env **env);
int    check_empty(char *str);
char		**ft_split(char	*s, char c);
void		handle_signal(int sig);
int			has_wild_cards_comarr(t_tree **tree);
int			has_wild_cards_fdlst(t_tree **tree);
void		handle_wildcards_in_cmdarr(t_tree **tree);
void		handle_wildcards_in_fdlst(t_tree **tree);
void		quote_remove_lst(t_tree **tree);
void	variable_expantion_para(t_tree **tree, t_env **env);
char	*ft_itoa(int n);
void		quote_remove_lst_two(t_tree **tree);
int			expandableornot(char *str);
int     ft_equal_count(char *str);
void		wild_cards_handle_cmdarr(char ***cmd_arr, char *dir_path);
char		**get_matches(char *pattern,
				char *dir_path, int *match_count);
int			match_pattern(char *pattern, char *string);
char		*str_duplicate(const char *s);
int			if_has_wildcards(char *str);
void		sort_matches(char **arr, int count);
void		process_wildcard_node_fd(t_list_fd *node, char *dir_path);
int			copy_and_resize_matches(char ***matches,
				int match_count, int new_cap);
char		*join_matches(char **matches, int match_count);
int			fd_list_size(t_list_fd **head);
int			no_words_beside(char *str);
// void		process_command_string(t_tree **tree, int k);
void		process_command_array(t_tree **tree);
// char		*create_filtered_string(char *old_str, int final_len, int l, int k);
void		ft_is_dir(char *s);
void		variable_expantion_preserve_quotes(t_tree **tree, t_env **env);
void		quote_remove_preserve_var(t_tree **tree);
void		quote_remove_improved(t_tree **tree);
int			process_array_variable(char **command_arr,
				int arr_idx, int *var_pos, t_env **env);
// int			count_filtered_length(char *old_str, int *var_flag, int l, int k);
// char		*create_filtered_string(char *old_str, int final_len, int l, int k);
void    reset_vars(t_tree **tree, t_env **env, t_hdoc_data *h_data);
typedef struct s_expand
{
	int		var_end;
	char	*var_name;
	char	*var_value;
	char	*before;
	char	*after;
}	t_expand;

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
void	lexer_to_tree(char *str, t_tree **tree, int *flag, t_hdoc_data *h_data);
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
void	print_syntax_error(void);
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
// void	sort_matches(char **arr, int count);
int		copy_and_resize_matches(char ***matches, int match_count, int new_cap);
int		match_pattern(char *pattern, char *string);
char	*str_duplicate(const char *s);
// char	*join_matches(char **matches, int match_count);
char	*ft_strcpy(char *dest, char *src);
void	quote_remove_two(t_tree **tree);
void	process_command_array(t_tree **tree);
// void	process_command_string(t_tree **tree, int k);
t_list	*new_list_init(char *str);
void	reset_vars(t_tree **tree, t_env **env, t_hdoc_data *h_data);
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
void	reset_var_expand(t_tmp_tree	*tmp, t_env **env, t_hdoc_data *h_data);
void	reset_var_expand_var(t_tree **tree, t_env **env, t_hdoc_data *h_data);
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
void	update_vars(int *star_idx, int *i, int *str_idx, int *j, int in_quotes);
void	back_track(int *i, int *j, int *star_idx, int *str_idx);
void	update_vars_two(int *i, int *j);
int		count_pattern_matches(char *pattern, char *dir_path);
int		new_size_calcs(int original_size, char ***cmd_arr,
			char *dir_path, int *match_count);
// void	if_expanded_matches(char ***new_cmd_arr,
// 			char **expanded_matches, int match_count, int *j);
int		count_cmd_arr_size(char **cmd_arr);
char	**init_new_cmd_arr(int new_size);
void	process_wildcard_element(char ***new_cmd_arr, char *element,
			char *dir_path, int *j);
void	free_original_cmd_arr(char ***cmd_arr, int original_size);
void	process_regular_element(char ***new_cmd_arr, char *element, int *j);
char	**get_matches(char *pattern, char *dir_path, int *match_count);
void	process_wildcard_node_fd(t_list_fd *node, char *dir_path);
void	handle_wildcards_in_fdlst(t_tree **tree);
int		has_wild_cards_fdlst(t_tree **tree);
void	ft_sig(int sig);
void	hide_terminal_control_chars(void);
void	display_terminal_control_chars(void);
void	ft_new_handler(int sig);
void	ft_new_handler_pip(int sig);
void	ft_free_array(char **p);
void	if_question_mark(t_tmp_tree **tmp, int n, t_hdoc_data *h_data);
int		underscore_case_check(char *var_name, char **command_arr, int arr_idx);
void	if_question_mark_two(char **line, int status, int n);
void	command_arr_fill_helper(t_tree **tree, char **str);
void	command_arr_fill(t_tree **tree);
void	env_fill_helper(t_env **node, int *i, int *j, char **argev);
void	env_fill_empty(t_env **node, int *i, int *j);
t_env	*env_fill(char **argev);
void	free_array(char **s);
int	ft_space_check(char *str);
void	ft_exec(t_tree *tree, t_env *h, char **e);
void	ft_close_handle(t_list_fd *head);
void	ft_close_fd(t_tree *tree);
int	ft_pip(t_tree *tree, t_env **h, char **e, int *check);
int	cmd_check(t_tree *tree);
int	ft_cmd_exec(t_tree *tree, t_env **h);
int	ft_redir_check(char *str);
void	ft_hdoc_free(char **str, char **limiter, int fd);
char	*ft_name_check(char *name);
void	ft_hdoc_signal(int sig);
void	ft_hdoc_expand(char **line, t_env **env, int status);
void	ft_hdoc(char *limiter, int fd, t_env **env, int status);
void	ft_exec_redir(t_tree *tree, t_env **h, char **env);
int ft_cmd_redir(t_tree *tree, t_env **h);
int	ft_variable(t_tree *tree, t_env **h, char **e, int *check);
int	check_amb(t_tree *tree);
void	ft_is_dir(char *s);
void	ft_new_handler(int sig);
void	ft_new_handler_pip(int sig);
void	display_terminal_control_chars(void);
void	ft_word_handle(t_tree *tree, t_env **h, char **e, int *check);
void	ft_execute(t_tree *tree, char **e, int *check, t_hdoc_data *h_data);
int	ft_hdoc_first(t_list_fd *tmp);
void	 ft_hdoc_check(t_tree *tree, t_hdoc_data *h_data, int status);
int	ft_hdoc_count_second(t_tree *tree);
int	ft_hdoc_count(t_tree *tree);
void	ft_hdoc_handle(t_tree *tree, t_hdoc_data *h_data, int status);
char	*ft_node_joint(t_env *h);
char **ft_env_str(t_env *h);
void	ft_st(t_tree *tree, int stat_flag);
void	ft_signal_exec(void);
t_env	*env_fill(char **argev);
void	*env_fill_pwd(t_env *head);
void	ft_close_handle(t_list_fd *head);
int	ft_wait_for_child(int status, int status1);
void	ft_pip_signal(void);
void	ft_exec_redir(t_tree *tree, t_env **h, char **env);
void	ft_exec_redir_helper(t_tree *tree, int i);
void	ft_exec_redir_support(t_tree *tree, int i);
int	check_amb(t_tree *tree);
int	ft_para_redir(t_tree *tree, t_hdoc_data *h_data);
int	ft_parenthasis(t_tree *tree, t_hdoc_data *h_data, char **e, int *check);
void	ft_signal_ign(void);
void	ft_word_handle_signal(t_tree *tree, int *check);
void	quote_check_filename(char **old_redirs, int *i);
int	ft_op_and(t_tree *tree, t_hdoc_data *h_data, char **e, int *check);
int	ft_op_or(t_tree *tree, t_hdoc_data *h_data, char **e, int *check);
void	ft_first_child(t_tree *tree, int *check, char **e, t_hdoc_data *h_data);
void	ft_second_child(t_tree *tree, int *check, char **e, t_hdoc_data *h_data);
int	ft_cmd_redir_support(t_tree *tree);
int	ft_n_return(int org_stdin, int org_stdout, int *check);
int	ft_para_signal(int status, int org_stdout, int org_stdin);
void	ft_para_child(t_tree *tree, int *check, t_hdoc_data *h_data, char **e);
void	ft_exec_test2(t_tree *tree, int *test, t_hdoc_data *h_data);
void	ft_exec_test(t_tree *tree, int *test, t_hdoc_data *h_data);
void	ft_exec_test(t_tree *tree, int *test, t_hdoc_data *h_data);
void	ft_free_data(t_hdoc_data *h_data);
int		check_inside_parenths(char *str);
int		parenth_case(char *str);
int		ft_check_act(t_env *h);
void    ft_f_node(t_env *node);
int	ft_echo(char **s);
// int	ft_cd(char **s, t_env *h);
char	*ft_str_back(char *s);
int	ft_cd_helper(t_env *h);
int	ft_chdir_helper(char **p, char *past);
int	ft_cd_fail(t_env *n, char **s, char *past);
int	ft_chdir_fail(char **s, char *past);
char **ft_equal_str(char *str);
t_env   *ft_sort_list(t_env *h);
int ft_equal_check(char *str);
int     ft_equal_count(char *str);
void    ft_remove_sign(char *str);
// char	*ft_cmd_check(char *env, char *s);
// int	ft_cmd_exec(t_tree *tree, t_env **h);
// void	ft_hdoc_free(char **str, char **limiter, int fd);
// int	ft_pip(t_tree *tree, t_env **h, char **e, int *check);
// void	protected_extraction(char **word, char **str);

#endif