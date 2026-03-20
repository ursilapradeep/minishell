# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/13 13:03:32 by spaipur-          #+#    #+#              #
#    Updated: 2026/03/20 17:04:54 by spaipur-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
       env/env_utils.c \
       env/env_executor.c \
       handle_args/handle_args.c \
       handle_args/args_helpers.c \
       handle_args/handle_cmds.c \
       handle_args/handle_quote.c \
       handle_args/handle_quote_utils.c \
       handle_args/handle_redirect.c \
       handle_path/find_path.c \
       handle_path/path_helpers.c \
       execute/execute.c \
       execute/execute_utils.c \
       builtins/builtins.c \
       builtins/builtin_cd.c \
       builtins/builtin_pwd_export.c \
       builtins/builtin_unset_exit.c \
       builtins/builtin_utils.c \
       pipes/pipes.c \
       pipes/pipes_utils.c \
       parser/command_builder.c\
       parser/command_pipeline.c\
       parser/command_builder_utils.c\
       parser/commands.c\
       parser/build_commands.c\
       parser/parser.c\
       parser/parser_utils.c\
       parser/parser_redirect.c\
       parser/quote_handler.c\
       parser/tokenizer_utils.c\
       parser/token.c\
       parser/tokenizer.c\
       parser/variable_expansion.c \
       parser/variable_expansion_utils.c \
       parser/variable.c\
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

test_parser:
	$(CC) $(CFLAGS) parser/test_parser.c parser/parser_redirect.o parser/parser_utils.o libft/libft.a -o test_parser
	./test_parser

.PHONY: all clean fclean re test_parser