/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:46:34 by makkach           #+#    #+#             */
/*   Updated: 2025/07/09 14:57:49 by makkach          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <termios.h>
# include <dirent.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

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
	int		end;
	int		hdoc_num;
}	t_hdoc_data;

typedef struct s_pip
{
	int	id1;
	int	id2;
	int	status;
	int	status1;
}	t_pip;

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
	int			id;
	struct s_id	*next;
}	t_id;

typedef struct s_tmp_tree
{
	t_list		*tmp;
	t_list_fd	*tmp_fd;
	t_tree		*tree;
}	t_tmp_tree;

typedef struct s_qfilter
{
	int		in_quotes;
	int		quote_idx;
	char	quote_type;
}	t_qfilter;

typedef struct s_matchpattern_idxs
{
	int			star_idx;
	int			str_idx;
}	t_matchpattern_idxs;

typedef struct s_par
{
	int	open_par;
	int	closed_par;
}	t_par;

typedef struct s_expand
{
	int		var_end;
	char	*var_name;
	char	*var_value;
	char	*before;
	char	*after;
}	t_expand;

typedef struct s_var_main
{
	t_tree			*tree;
	t_env			*env;
	char			*str;
	char			**e;
	int				flag;
	int				hdoc_num;
	int				check;
	t_hdoc_data		*h_data;
	struct termios	termios_a;
	char			*temp;
	int				main_stat;
	int				main_end;
}	t_var_main;

typedef struct s_rm_dollar
{
	int		j;
	char	*before;
	char	*after;
	char	*dollars;
	char	*new_str;
}	t_rm_dollar;

int	g_global_status;

void	quote_parse(char **str, int *flag);
void	lexer_to_tree(char *str, t_tree **tree, int *flag, t_hdoc_data *h_data);
void	tree_to_rediropen(t_tree *tree, int *flag);
void	export_cases(t_tree **tree);
void	redirections_list_maker(t_tree **tree);
void	inits_main(t_env **env,
			t_tree **tree, char **argev);
void	ft_free_array(char **p);
void	free_env(t_env **env);
void	free_tree(t_tree *tree);
void	execution(t_var_main *shell);
int		check_empty(char *str);
void	ft_str_empty(t_var_main *shell, t_env **env,
			char **e, t_hdoc_data *h_data);
void	handle_signal(int sig);
void	hide_terminal_control_chars(void);
char	**ft_env_str(t_env *h);
size_t	ft_strlen(char *str);
char	*ft_strdup(char *s1);
int		operation_recognizer(char *str);
int		redirection_recognizer(char *str);
int		pipe_recognizer(char *str);
int		word_recognizer(char *str);
int		parenthasis_recognizer(char *str);
int		command_recognizer(char *str);
int		variable_recognizer(char *str);
char	*ft_substr(char *s, unsigned int start, size_t len);
int		is_operator(char c);
void	if_par_loop(int *i, char *str, char **word);
void	if_string_while_loop(int *i, char *str, char **word);
void	str_extractor_helper(char *str, int *i, int *flag, char **word);
void	handle_wildcards_in_fdlst(t_tree **tree);
int		has_wild_cards_fdlst(t_tree **tree);
void	handle_wildcards_in_cmdarr(t_tree **tree);
void	extract_variable_inits(int *i, int *in_quotes, char *quote_type);
int		has_wild_cards_comarr(t_tree **tree);
void	handle_quotes_two(char **word, char **str);
void	handle_operator(char **word, char **str);
void	variable_handle(char **word, char **str);
void	handle_parenthases(char **word, char **str);
void	handle_else(char **word, char **str);
int		count_total_nodes(t_list *head, t_list **last);
void	process_pipe_node(t_list **head, t_tree **tree, t_list *tmp,
			int *node_info);
int		ft_strcmp(char *s1, char *s2);
void	process_operation_node(t_list **head, t_tree **tree, t_list *tmp,
			int *node_info);
char	*side_maker(t_list **head, int number, int j);
void	init_tree_node(t_tree **node, char *type);
void	update_tree_with_cmd(t_tree *tree, t_tree *cmd_tree);
int		check_pipe_or_and(const char *str, int i, char quote);
void	free_cmd_list(t_list *cmd_list);
void	lexer(t_list **head);
t_list	*list_init(char *str);
void	process_command_with_pipes_inits(t_list **cmd_list, char **cmd_copy);
t_tree	*handle_operation_token(t_list **head, t_list *pipe_pos);
void	build_command_str(char **command_str, t_list *current);
t_tree	*create_tree_node(void *command, char *type);
void	free_list_to_position(t_list **head, t_list *position);
t_tree	*handle_pipe_token(t_list **head, char *left_cmd,
			t_list *pipe_pos);
char	*ft_strjoin_three(char *s1, char *s2, char *s3);
char	*ft_strjoin(char *s1, char *s2);
int		if_var_end_is_dollar(char **command_arr,
			int *var_pos, int arr_idx, int var_end);
void	var_end_set(char **command_arr, int arr_idx, int *var_end);
int		underscore_case_check(char *var_name, char **command_arr, int arr_idx);
void	extract_var_value(t_env **env, char *var_name, char **var_value);
int		if_redir_in_para(char *str, int *i, int *in_quotes, char *quote_type);
void	redirections_in_par_handle(t_tree **tree, char **cmd_part,
			char **original_redirs, char **content);
void	sub_tree_creation(t_tree **sub_tree, char **content, t_list **sub_list);
void	free_all(char **content, char **original_redirs);
void	sub_tree_attachment(t_tree **sub_tree, t_tree **tree,
			char **original_redirs);
int		initialize_buffers(t_extract *v, char *cmd_str);
void	init_extraction_vars(int *i, int *in_quotes, char *quote_type,
			int *paren_count);
int		process_command_string_two(t_extract *v, char *cmd_str);
void	dyn_buf_free(t_dynbuf *buf);
void	cleanup_and_assign(t_dynbuf *command_buf, t_dynbuf *redir_buf,
			char **cmd_part, char **redir_part);
char	*ft_strchr(char *str, int c);
char	*ft_strtrim(char *s1, char *set);
void	quote_check_filename(char **old_redirs, int *i);
void	init_list_fd_node(t_list_fd *node);
void	print_syntax_error(void);
void	even_more_ifs(char *prev_token,
			char *prev_data, t_list *tmp, int *flag);
int		count_filtered_length(char *old_str);
char	*create_filtered_string(char *old_str, int final_len);
void	ft_putstr_fd(int fd, char *s);
int		check_quotes(char *str, int *flag);
char	*replace_whites_spaces(char *str);
t_env	*env_fill(char **argev);
void	syntax_error_two(t_tree **tree, int *flag);
void	command_arr_fill(t_tree **tree);
void	process_all_redirections(t_tree **tree);
void	process_pipe_trees(t_tree *tree);
void	process_nested_parentheses(t_tree **tree);
int		if_not_head(t_list *head, char *str);
int		new_syntax_error(t_list **head);
void	syntax_error(t_list **head, int *flag);
void	tree_maker(t_list **head, t_tree **tree);
void	free_list(t_list **head);
void	open_quotes(int *in_quotes, char *quote_type, char **str, int *i);
void	parenthesis_recognizer_helper(int *open_par,
			int *closed_par, int *i, char *str);
char	*extract_variable(char *str);
char	*first_word_remover(char *str, char *word);
char	*str_extractor(char *str);
char	*str_remover(char *str, char *word);
char	*extract_operator(char *str);
char	*remove_operator(char *str, char *word);
char	*extract_parenthesis(char *str);
char	*parenthesis_remover(char *str, char *word);
char	*word_extractor(char *str);
int		still_has_parentheses(t_tree **tree);
void	extract_redirections(char *cmd_str, char **cmd_part, char **redir_part);
void	redirections_handle(char **original_redirs, t_tree **tree);
char	*extract_content_from_parentheses(char *command);
int		in_para_check(char *str);
void	free_node_data(t_list *node);
void	cleanup_left_nodes(t_list *nodes);
void	cleanup_right_nodes(t_list *nodes);
void	handle_operation_command(t_tree **tree, t_list *tmp,
			int total_nodes, int i);
void	if_prev(t_list *tmp, t_list **prev_part, t_tree **tree, t_list **head);
t_tree	*build_pipe_tree(t_list **head);
t_list	*copy_list_segment(t_list *head, t_list *end_pos);
int		dyn_buf_add_char(t_dynbuf *buf, char c);
int		process_redirection_helper(char *cmd_str, int *i, t_dynbuf *redir_buf);
int		is_command_char(char c, int in_quotes);
void	dyn_buf_finalize(t_dynbuf *buf);
int		process_redirection(char *cmd_str, int *i, t_dynbuf *redir_buf);
int		dyn_buf_init(t_dynbuf *buf, size_t initial_capacity);
void	handle_quotes_and_parens(char c, int *in_quotes, char *quote_type,
			int *paren_count);
int		process_array_variable(char **command_arr,
			int arr_idx, int *var_pos, t_env **env);
int		if_question_mark_three(t_tmp_tree **tmp, int n, t_hdoc_data *h_data);
int		variable_search_instr(char *str);
char	*ft_itoa(int n);
int		process_variable_expantion(char **command_arr,
			int arr_idx, int *var_pos, t_env **env);
int		countwords(char *s, char c);
int		if_has_wildcards(char *str);
void	wild_cards_handle_cmdarr(char ***cmd_arr, char *dir_path);
int		copy_and_resize_matches(char ***matches, int match_count, int new_cap);
char	*str_duplicate(const char *s);
int		init_matches(char ***matches, DIR **dir,
			char *dir_path, int *capacity);
int		count_cmd_arr_size(char **cmd_arr);
int		new_size_calcs(int original_size, char ***cmd_arr,
			char *dir_path, int *match_count);
int		match_pattern(char *pattern, char *string);
char	**init_new_cmd_arr(int new_size);
void	process_wildcard_element(char ***new_cmd_arr, char *element,
			char *dir_path, int *j);
void	process_regular_element(char ***new_cmd_arr, char *element, int *j);
void	free_original_cmd_arr(char ***cmd_arr, int original_size);
void	update_vars_two(int *i, int *j);
void	back_track(int *i, int *j, int *star_idx, int *str_idx);
void	pattern_match_inits(int *i, int *j,
			t_matchpattern_idxs	*mp, t_qfilter *qfil);
int		skip_stars(int *i, const char *pattern);
void	update_vars(int *star_idx, int *i, int *str_idx, int *j);
void	handle_quotes(char **word, char **str);
void	new_parenthasis_handle(char **str, char **word);
void	new_variable_handle(char **str, char **word);
void	new_operator_handle(char **str, char **word);
void	new_word_handle(char **str, char **word);
void	equal_handle(char **str, char **word);
char	***cmd2_fill(t_tree **tree);
void	free_twod_char(char **arr);
int		count_triple_ptr(char ***cmd2);
void	fill_twod_char_with_triple(char **arr, char ***cmd2);
void	reset_var_expand_var(t_tree **tree, t_hdoc_data *h_data);
int		isnt_valid(char *str);
t_list	*new_list_init(char *str);
void	new_parenthasis_word_extractor(char **word, int i, char **str);
void	new_variable_word_extractor(char **str, char **word, int i);
void	new_operator_word_extractor(char **word, char **str);
int		if_question_mark(t_tmp_tree **tmp, int n, t_hdoc_data *h_data);
int		lst_size(t_list **head);
void	process_no_list_size(t_list *head, int list_size, char ***cmd2, int *i);
int		fd_list_size(t_list_fd **head);
void	process_wildcard_node_fd(t_list_fd *node, char *dir_path);
int		count_pattern_matches(char *pattern, char *dir_path);
char	**get_matches(char *pattern, char *dir_path, int *match_count);
t_env	*ft_check(t_env *h, char *str);
char	**ft_split(char *s, char c);
char	*ft_strmcpy(char *src);
int		ft_is_spaces(char *s);
void	ft_lstadd_back(t_env **lst, t_env *new);
t_env	*ft_lstnew(void *key, void *value);
char	*ft_cmd_check(char *env, char *s);
void	ft_close_handle(t_list_fd *head);
void	ft_close_fd(t_tree *tree);
t_pip	*ft_test_x(t_pip **x);
void	ft_signal_ign(void);
void	ft_first_child(t_tree *tree, int *check, char **e, t_hdoc_data *h_data);
void	ft_second_child(t_tree *tree,
			int *check, char **e, t_hdoc_data *h_data);
int		ft_wait_for_child(int status, int status1, int *check, int *s);
void	set_quote_vars(int *in_quotes, char *quote_type, char c);
int		expand_str_if_dollar(char **str, int *i, t_rm_dollar *s);
void	remove_quotes_from_var_two(char	**arr);
void	ft_hdoc_expand(char **line, t_env **env, int status);
void	ft_hdoc_free(char **str, char **limiter, int fd);
int		ft_file_check(char *str);
int		ft_file_create(char *str, int n);
int		ft_redir_check(char *str);
int		ft_cmd_exec(t_tree *tree, t_hdoc_data *h_data);
void	ft_exec(t_tree *tree, t_env *h, char **e);
void	ft_execute_com(t_tree *tree, t_hdoc_data *h_data);
void	reset_vars(t_tree **tree, t_env **env, t_hdoc_data *h_data);
int		cmd_check(t_tree *tree);
void	display_terminal_control_chars(void);
void	ft_execute_redir(t_tree *tree, t_hdoc_data *h_data, char **e);
void	ft_word_handle_signal(t_tree *tree, int *check);
void	ft_word_expand(t_tree *tree, t_hdoc_data *h_data);
void	ft_word_handle(t_tree *tree, t_env **h, char **e, int *check);
void	variable_expantion_inlnkedlst(t_tree **tree, t_hdoc_data *h_data);
void	ft_hdoc_check(t_tree *tree, t_hdoc_data *h_data, int status);
int		ft_lstsize(t_env *lst);
void	env_fill_helper(t_env **node, int *i, int *j, char **argev);
void	env_fill_empty(t_env **node);
char	*ft_name_check(char *name, int h_doc);
void	ft_hdoc(char *limiter, t_list_fd *tmp, t_env **env, int status);
void	rem_dollar(char **str);
void	quote_remove_lst(t_tree **tree);
void	ft_exec_redir_helper(t_tree *tree, int i);
int		ft_cmd_redir_support(t_list_fd *tmp);
int		ft_n_return(int org_stdin, int org_stdout, int *check);
void	ft_para_child(t_tree *tree, int *check, t_hdoc_data *h_data, char **e);
int		ft_para_signal(int status, int org_stdout, int org_stdin);
void	ft_execute(t_tree *tree, char **e, int *check, t_hdoc_data *h_data);
void	if_question_mark_two(char **line, int status, int n);
void	ft_hdoc_expand_inits(int *i, int *in_quotes,
			int *flag, char *quote_type);
int		ft_isdigit(int c);
char	*ft_str_back(char *s);
int		ft_cd_helper(t_env *h);
int		ft_chdir_fail(char **s, char *past);
int		ft_echo(char **s);
int		ft_env(t_env *h);
int		ft_exit(char **s, t_hdoc_data *h_data, int status);
int		ft_export(char **s, t_env *h, t_tree *tree);
int		ft_pwd(t_env *h);
int		ft_unset(t_env **h, char **s);
int		ft_cd_fail(t_env *n, char **s, char *past);
long	ft_atoi(const char *str);
t_env	*ft_sort_list(t_env *h);
void	ft_f_node(t_env *node);
int		ft_parse(char *s);
void	ft_free_v(char **v, char *tmp);
void	ft_expo_error2(int *status, char *s);
int		ft_equal_count(char *str);
char	**ft_equal_str(char *str);
void	ft_expo_equal_helper(char **v, int act, t_env *f);
int		ft_act_helper(int act, char **v, t_env **new);
int		ft_equal_check(char *str);
void	ft_v_expo(char *s, char ***v);
void	ft_remove_sign(char *str);
void	ft_free_error(char **v, int *status, char *s);
void	ft_expo_error(int *status, char *s);
int		ft_equal(char *s);
int		ft_expo_equal(char *s, t_env *h, int act, int *status);
int		ft_exp_print(t_env *h);
int		remove_dollar(char **str, int i);
int		ft_isalpha(int c);
void	if_dollar_two_inits(int *j, int *in_quotes);
int		if_dollar(int *i, char **line, t_env **env, int *status);
void	reset_var_cmd_split(t_tree **tree);
void	reset_var_remove_quotes(t_tree **tree);
void	ft_free_data(t_hdoc_data *h_data);
void	ft_check_exit(int *check, int status);
void	if_var(t_tree **tree);
void	if_dollar_two(t_tree *tree, int i, t_hdoc_data *h_data);
void	ft_exit_m(int m, t_env **h, char *tmp);
int		ft_modulo(char *str);
int		ft_exit_error(int check, char **s);
int		ft_check_string(char *str);
void	ft_exit_empty(int status, int stat);
char	*ft_skip_space(char *str);
int		ft_pip(t_tree *tree, t_hdoc_data *h_data, char **e, int *check);
void	ft_var_helper(t_tree **tree, char **e, int *check, t_hdoc_data *h_data);
int		ft_op_or(t_tree *tree, t_hdoc_data *h_data, char **e);
int		ft_parenthasis(t_tree *tree, t_hdoc_data *h_data, char **e, int *check);
void	ft_cmd_word(t_tree *tree, t_hdoc_data *h_data, int *check, char **e);
int		ft_cmd_word_check(t_tree *tree);
int		ft_op_and(t_tree *tree, t_hdoc_data *h_data, char **e);
int		variable_search_inlnkedlst(t_tree **tree);
void	ambiguous_set(t_tree **tree);
int		ambiguous_syntax_error(t_tree **tree, t_env **env);
int		ft_cmd_redir(t_tree *tree, t_hdoc_data *h_data);
void	ft_exec_redir(t_tree *tree, t_env **h, char **env);
int		ft_variable(t_tree *tree, t_hdoc_data *h_data, char **e, int *check);
void	ft_word_redir(t_tree *tree, t_hdoc_data *h_data, char **e, int *check);
void	ft_exec_test(t_tree *tree, int *test, t_hdoc_data *h_data);
int		ft_hdoc_count(t_tree *tree);
void	ft_hdoc_exed(void);
void	ft_hdoc_handle(t_tree *tree, t_hdoc_data *h_data, int status);
void	ft_st(t_tree *tree, int stat_flag);
void	ft_signal_exec(void);
void	ft_parsing(char **str, int *flag, t_tree **tree, t_hdoc_data *h_data);
void	handle_else_two(char **str, char **word);
t_list	*node_maker(char *word);
void	handle_new_node(t_list **new_node, t_list **tmp, char **word);
t_list	*new_list_init_two(char *str);
void	lexer_two(t_list **head);
void	print_tree_visual(t_tree *tree, int level, int is_left);

#endif