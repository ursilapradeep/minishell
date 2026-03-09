NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
       env/env_utils.c \
       handle_args/handle_args.c \
       handle_args/args_helpers.c \
       handle_args/handle_cmds.c \
       handle_args/handle_redirect.c \
       handle_path/find_path.c \
       handle_path/path_helpers.c \
       execute/execute.c \
       builtins/builtins.c \
       builtins/builtin_utils.c \
       pipes/pipes.c \
       pipes/pipes_utils.c \
       redirections/redirections.c \
       redirections/heredoc.c \
       redirections/execute_redirections.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C libft

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C libft clean
	rm -f $(OBJS)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re