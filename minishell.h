/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makkach <makkach@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:46:34 by makkach           #+#    #+#             */
/*   Updated: 2025/05/09 18:13:32 by makkach          ###   ########.fr       */
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
void		syntax_error_two(t_tree **tree);
void		syntax_error(t_list **head);
int			even_more_ifs(char *prev_token, char *prev_data, t_list *tmp);
void		lexer_to_tree(char *str, t_tree **tree);
void		tree_to_rediropen(t_tree *tree);
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
void		print_syntax_error(char *str2);
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
t_tree		*create_command_node(t_list **node_list, int offset);
int			count_total_nodes(t_list *head, t_list **last);
void		if_prev(t_list *tmp, t_list **prev_part,
				t_tree **tree, t_list **head);
t_tree		*setup_operation_command(t_list **next_list, int offset);
void		handle_operation_command(t_tree **tree, t_list *tmp,
				int total_nodes, int i);
void		process_operation_node(t_list **head, t_tree **tree, t_list *tmp,
				int *node_info);
void		process_pipe_node(t_list **head, t_tree **tree, t_list *tmp,
				int *node_info);
int			skip_spaces(char *str, int start);
int			check_quotes(char *str);
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
int			count_filtered_length(char *old_str);
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
int			process_regular_char(char c, t_dynbuf *command_buf,
				int in_quotes_or_paren);
void		cleanup_and_assign(t_dynbuf *command_buf, t_dynbuf *redir_buf,
				char **cmd_part, char **redir_part);
int			initialize_buffers(t_extract *v, char *cmd_str);
int			process_command_char(t_extract *v, char *cmd_str);
int			process_command_string_two(t_extract *v, char *cmd_str);
int			process_redirection_helper(char *cmd_str, int *i,
				t_dynbuf *redir_buf);
int			determine_redirection_flag(char *redirection, int i);
void		skip_redirection_and_spaces(char *redirection, int *i);
void		extract_filename(char **old_redirs, char **target_name);
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
void		quote_parse(char **str);
int			countwords(char *s, char c);
void		ambiguous_set(t_tree **tree);
int			variable_search_instr(char *str);
int			new_syntax_error(t_list **head);
int			ambiguous_syntax_error(t_tree **tree);
void		quote_set(t_tree **tree);
void		tree_empty_error(t_tree **tree);
void		redirections_list_maker(t_tree **tree);
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
char		**ft_split(char *s, char c);
void		handle_signal(int sig);
int			has_wild_cards_comarr(t_tree **tree);
int			has_wild_cards_fdlst(t_tree **tree);
void		handle_wildcards_in_cmdarr(t_tree **tree);
void		handle_wildcards_in_fdlst(t_tree **tree);
void		quote_remove_lst(t_tree **tree);
void		quote_remove_lst_two(t_tree **tree);
int			expandableornot(char *str);

#endif