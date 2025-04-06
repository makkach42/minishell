NAME = minishell
CC = cc
RM = rm -rf
CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address
SRC = ft_split.c ft_strdup.c ft_strjoin.c ft_strtrim.c parsing.c ft_strlen.c ft_substr.c ft_strcmp.c ft_strchr.c ft_strncpy.c ft_strcpy.c ft_strcat.c ft_strncmp.c
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
