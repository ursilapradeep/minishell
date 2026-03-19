# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/13 13:03:32 by spaipur-          #+#    #+#              #
#    Updated: 2026/03/19 17:23:35 by spaipur-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
       env/env_utils.c \
       handle_args/handle_args.c \
       handle_args/args_helpers.c \
       handle_args/read_process_input.c \
       handle_path/find_path.c \
       handle_path/path_helpers.c \
       execute/execute.c \
       builtins/builtins.c \
       builtins/builtin_utils.c \
       pipes/pipes.c \
       pipes/pipes_utils.c\
       parser/commands_builder.c\
       parser/commands_builder_utils.c\
       parser/commands.c\
       parser/build_commands.c\
       parser/parser.c\
       parser/parser_utils.c\
       parser/parse_redirect.c\
       parser/quote_handler.c\
       parser/tokenizer_utils.c\
       parser/token.c\
       parser/tokenizer.c\
       parser/variable_expansion.c \
       parser/variable_expansion_utils.c \
       parser/variable.c
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