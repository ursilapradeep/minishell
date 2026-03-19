/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:28:29 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 17:40:31 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
	struct s_env	*prev;
}				t_env;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC
}				t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

typedef struct s_cmd
{
	char			**args;
	int				infd;
	int				outfd;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}				t_cmd;

// Function prototypes
t_env 	*init_env(char **envp);
void 	add_env_node(t_env **env_list, char *env_str);
char	**split_args(char *input);
void	free_args(char **args);
int		is_space(char c);
int		word_end(char *input, int i);
int		word_len_without_quotes(char *input, int start, int end);
void	copy_without_quotes(char *dst, char *input, int start, int end);
char	*find_command(char *cmd, t_env **envp);
int		execute_command(char **args, t_env **my_env);
char	*process_directory(char *path_copy, char **dir_start, int i, char *cmd);
char	*check_command_in_dir(char *dir, char *cmd);
char 	*get_path_from_env(char **envp);
char 	*read_input(void);
int		process_input(char *input, t_env **my_env);

// Args helpers
int 	is_space(char c);
int		word_end(char *input, int i);
int		word_len_without_quotes(char *input, int start, int end);
void 	copy_without_quotes(char *dst, char *input, int start, int end);

//built- ins
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_env **my_env);
int		builtin_cd(char **args, t_env **env);
int		builtin_pwd(char **args);
int		builtin_echo(char **args);
int		builtin_export(char **args, t_env **env);
int		builtin_unset(char **args, t_env **env);
int		builtin_exit(char **args);
int		builtin_env(char **args, t_env *env);
char	*get_env_value(t_env *env, char *key);
void	set_env_value(t_env **env, char *key, char *value);
char	**build_env_array(t_env *env);

// Pipe handling
int		contains_pipe(char *input);
char	**parse_pipeline(char *input);
void	execute_pipeline(char **pipeline, t_env **envp);
// Command processing
int		handle_pipeline(char *input, t_env **my_env);
int		handle_single_command(char *input, t_env **my_env);
int		process_input(char *input, t_env **my_env);
char	*read_input(void);
// Redirection handling
int		handle_output_redirect(char *filename);
int		handle_append_redirect(char *filename);
int		handle_input_redirect(char *filename);
int		handle_heredoc(char *delimiter);
char	*apply_redirections(char *input);
int		contains_redirection(char *input);

// Parser - Redirection Handler
int		handle_redirections(t_cmd *cmd, char *cmd_str);
char	*get_command_portion(char *cmd_str);
void	print_redirections(t_cmd *cmd);

// Parser - Tokenizer
t_token	*tokenize(char *input);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);

// Parser - Command Builder
t_cmd	*build_commands(t_token *tokens);
void	free_cmd_list(t_cmd *cmd);
void	print_commands(t_cmd *commands);

// Parser - Variable Expansion
char	*expand_string(const char *input, t_env *env);
int		expand_token_list(t_token *tokens, t_env *env);

// Parser - Quote Handler (from pipex)
int		is_blank_char(char c);
int		is_quote_char(char c);
int		is_part_of_curr_arg(char c, char curr_quote);
void	update_current_quote(char *curr_quote, char quote_char);
void	remove_quotes(char *str);
void	handle_backslash(char *str, int *src, int *dst);
void	handle_quote_open(char c, char *quote, int *src);
void	handle_quote_close(char *quote, int *src);
void	handle_normal_char(char *str, int *src, int *dst);
char	**parse_command_args(char *cmd_str);

// Input/Output handling
char	*read_input(void);
int		process_input(char *input, t_env **my_env);
// Quote/expansion helpers
char	*expand_variables(char *input, t_env *env);
int		is_var_char(char c);
int		count_token_len(char *input, int *i, char *quote,
			t_env *env, int *out_len);

// Execute helpers
int		wait_and_get_exit_status(pid_t pid);

#endif
