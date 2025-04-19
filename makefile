NAME = minishell
CC = cc
RM = rm -rf
CFLAGS = -Wall -Werror -Wextra #-g3 -fsanitize=address
SRC = ft_split.c  ft_strtrim.c parsing.c  mem_debug.c libft_funcs.c more_libft_funcs.c even_more_libft_funcs.c lexer.c word_extraction.c str_extraction.c parentheses_extraction.c operation_extraction.c list_init.c tree_maker.c process_pipe_tree.c free_everything.c
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
