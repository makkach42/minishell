NAME = minishell
CC = cc
RM = rm -rf
CFLAGS = -Wall -Werror -Wextra #-g -fsanitize=address
SRC =	ft_strtrim.c parsing.c  libft_funcs.c more_libft_funcs.c even_more_libft_funcs.c lexer.c word_extraction.c str_extraction.c parentheses_extraction.c operation_extraction.c list_init.c tree_maker.c process_pipe_tree.c free_everything.c variables_handle.c process_nested_parentheses.c process_all_redirections.c redirections_opener.c syntax_error.c quotes_handle.c helpers.c main_helper.c lexer_helper.c lexer_helper_two.c list_init_handlers.c lexer_helper_three.c process_nested_parentheses_helper.c syntax_error_helper.c word_extraction_helper.c tree_maker_helper.c tree_maker_helper_two.c tree_maker_helper_three.c process_pipe_tree_helper.c process_pipe_tree_helper_two.c process_all_redirections_helper.c process_all_redirections_helper_two.c variable_handle_helper.c variable_handle_helper_two.c some_other_helpers.c redirections_opener_helper.c some_new_other_helpers.c dynamic_allocations_buffers.c ft_split.c signals.c wild_cards.c wild_cards_helper.c wild_cards_helper_two.c quote_handle_two.c quote_handle_three.c new_list_init.c reset_vars.c export_cases.c new_list_init_handlers.c new_list_init_handlers_helper.c new_list_init_handlers_helper_two.c reset_vars_helper.c reset_vars_helper_two.c wild_cards_helper_three.c wild_cards_helper_four.c wild_cards_helper_five.c wild_cards_helper_six.c\
					ft_builtin.c exec_utils.c ft_exec_utils1.c ft_exec_utils2.c ft_exec_utils3.c ft_exec_utils4.c ft_exec_utils5.c ft_exec_utils6.c ft_exec_utils7.c ft_exec_utils8.c ft_exec_utils9.c ft_hdoc.c ft_exec_utils10.c ft_exec_utils11.c ft_exec_utils12.c ft_exec_utils13.c ft_builtin1.c ft_builtin5.c ft_builtin3.c ft_builtin4.c ft_builtin2.c ft_builtin6.c ft_builtin7.c ft_builtin8.c ft_builtin9.c ft_builtin10.c ft_builtin11.c ft_func.c ft_func1.c ft_func2.c ft_func3.c ft_func4.c ft_exec_utils14.c ft_exec_utils15.c parsing_helper.c new_list_init_two.c new_list_init_two_handlers.c parasyntax_error.c
OBJ = $(SRC:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lreadline -o $(NAME)

%.o:	%.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all
