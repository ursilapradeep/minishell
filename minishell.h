/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:28:29 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/30 18:24:13 by uvadakku         ###   ########.fr       */
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
//#include "libft/libft.h"
#include "libft/libft.h"
#include <ctype.h>

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

typedef struct s_word_info {
	char *input;
	int start;
	int end;
} t_word_info;

typedef struct s_var_result {
    char *expanded;
    int consumed;
    const char *var_name;
    int var_len;
    int is_braced;
} t_var_result;

#ifdef rl_done
 rl_done = 0;
#endif

extern int	g_last_status;
extern int	g_sigint_received;

// Helper function to check if a token is a valid argument or a redirect
typedef enum e_token_check
{
    TOKEN_VALID_ARGUMENT,
    TOKEN_REDIRECT
}   t_token_check;

typedef struct s_shell_state {
	int last_status;
	int sigint_received;
} t_shell_state;

extern t_shell_state g_shell;

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
int		execute_pipeline(char **pipeline, t_env **envp);
int		wait_for_pipeline_children(int cmd_count, pid_t last_pid);
void	execute_pipeline_command_or_exit(char *segment, t_env **envp);
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
int	build_pipeline(t_cmd *commands);

// Function prototypes for helpers used before definition

// Only the correct prototype for get_env_variable should be present
char *get_env_variable(t_env *env, const char *var_name, int len);

// Other helper prototypes can be declared as needed, but avoid duplicates.
int handle_empty_variable_name(int is_braced, char **expanded, int *consumed);
int allocate_expanded_value(const char *value, int value_len, char **expanded);

char *expand_and_allocate_value(t_env *env, const char *var_name, int var_len, int is_braced);
const char *handle_braced_var(const char *input, int *len);

// Prototypes for functions from commands.c
t_cmd *create_cmd(void);
void add_cmd(t_cmd **head, t_cmd *new_cmd);
int count_args(t_token *tokens);

// Forward declaration for handle_redirection
int handle_redirection(t_cmd *cmd, t_token *current, int redirection_type);


// Parser - Tokenizer
t_token	*tokenize(char *input);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);

// Parser - Command Builder
int initialize_cmd_arguments(t_cmd *cmd, t_token **tokens);
char **build_args_array(t_token *tokens, int arg_count);
t_cmd	*build_commands(t_token *tokens);
void	free_cmd_list(t_cmd *cmd);
void	print_commands(t_cmd *commands);
int		open_redirection_file(t_cmd *cmd, char *filename, int redirection_type);
t_token *find_next_pipe(t_token *tokens);
int process_tokens_into_commands(t_token *tokens, t_cmd **commands);

// Parser - Variable Expansion
char	*expand_string(const char *input, t_env *env);
int		expand_token_list(t_token *tokens, t_env *env);
t_cmd *build_single_cmd(t_token **tokens);
const char *skip_non_redirects(const char *current);
int	parse_redirections_loop(t_cmd *cmd, const char *current);
int open_redirect_file(t_cmd *cmd, t_token_type operator, const char *filename);
const char *ret_heredoc(const char *rest_of_line);
const char *handle_single_redirect(t_cmd *cmd, t_token_type operator,const char *rest_of_line);
const char	*process_redirection(t_cmd *cmd, const char *current,t_token_type type, int *fd_count);
t_token_type is_redirect_operator(const char *str);
const char *skip_whitespace(const char *str);
char *extract_redirect_filename(const char *input);
int open_input_file(const char *filename);
int open_output_file(const char *filename, int append);
int handle_special_cases(const char *input, int var_len, int is_braced, t_var_result *result);
int	handle_status_special_case(const char *input, char **expanded,
		int *consumed);
// Input/Output handling
char	*read_input(void);
int proc_input(const char *input, char *result, int *result_len, t_env *env);

// Quote/expansion helpers
char	*expand_variables(char *input, t_env *env);
int		is_var_char(char c);
int		count_token_len(char *input, int *i, char *quote,
			t_env *env, int *out_len);
int cnt_args(const char *cmd);
char *get_next_arg(char **cmd);

// Execute helpers
int		wait_and_get_exit_status(pid_t pid);

// Prototypes for tokenizer and token utilities
char *extract_quoted_string(const char *input, int *len);
char *extract_word(const char *input, int *len);
t_token *create_token(char *value, t_token_type type);
void add_token(t_token **head, t_token *new_token);
const char *skip_whitespace_simple(const char *input);

// Prototypes for additional tokenizer utilities
char *extract_redirect_operator(const char *input, int *len);
int is_quote(char c);

// Prototypes for variable expansion utilities
const char *extract_var_name(const char *input, int *len, int *is_braced);
int expand_variable(const char *input, t_env *env, t_var_result *result);

// Added prototypes for handle_quotes and expand_variable_helper to minishell.h.
int handle_quotes(const char **current, int *in_single_quote, int *in_double_quote);
int expand_variable_helper(const char **current, char *result, int *result_len, t_env *env);
int	expanded_len(char *input, t_env *env);
int	process_next_char(char *expanded, int *j, char *input,
		int *i, char *quote, t_env *env);

// Signal handling
void setup_signal_handlers(void);
void ignore_signals(void);
void restore_signals(void);
void signal_handler_sigint(int sig);
void signal_handler_sigquit(int sig);
#endif
