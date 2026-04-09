# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/13 13:03:32 by spaipur-          #+#    #+#              #
#    Updated: 2026/04/09 16:39:40 by uvadakku         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
       executor/env_utils.c \
       executor/env_executor.c \
       executor/execute.c \
       executor/execute_utils.c \
       executor/execute_redirections.c\
       executor/execute_builtins.c\
       executor/execute_single_cmds.c\
       executor/execute_external.c\
       executor/execute_pipes.c\
       executor/execute_pipes_utils.c\
       handle_path/find_path.c \
       handle_path/path_helpers.c \
       builtins/builtin_cd_exit.c \
       builtins/builtin_echo_pwd.c \
       builtins/builtin_export_unset.c \
       builtins/builtin_env.c \
       signal/Signals.c \
       tokenizer/tokenize.c\
       tokenizer/tokenize_utils_I.c\
       tokenizer/tokenize_utils_II.c\
       expander/variable_expansion.c \
       expander/variable_expansion_utils_I.c \
       expander/variable_expansion_utils_II.c \
       expander/variable_expansion_utils_III.c \
       expander/quote_removal.c \
       parser/command_builder.c\
       parser/command_err_handler.c\
       parser/build_args_utils.c\
       parser/build_redirections.c\
       parser/build_pipeline.c\
       parser/build_args.c\
       parser/process_heredoc.c\
       process_input/parse.c \
       process_input/parse_input.c \
       
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