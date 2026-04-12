/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:28:29 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 15:56:45 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR
}	t_token_type;

typedef enum e_operator
{
	OP_NONE,
	OP_AND,
	OP_OR
}	t_operator;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				quoted;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	int				infd;
	int				outfd;
	int				errfd;
	char			*heredoc_delimiter;
	char			**heredoc_delimiters;
	int				heredoc_count;
	t_operator		next_op;
	int				has_pipe;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_shell_state
{
	int				last_status;
	int				sigint_received;
}	t_shell_state;

extern t_shell_state	g_shell;

typedef enum e_token_check
{
	TOKEN_VALID_ARGUMENT,
	TOKEN_REDIRECT
}	t_token_check;

// process input 
char		*read_input(void);
char		*read_non_interactive_line(void);
void		update_quote_state(char c, int *in_sq, int *in_dq);
int			parse_input(char *input, t_env **my_env);
t_env		*init_env(char **envp);
void		add_env_node(t_env **env_list, char *env_str);

//tokenizer
t_token		*tokenize(const char *input);
int			is_quote(char c);
char		*extract_quoted_string(const char *input, int *len);
char		*extract_word(const char *input, int *len);
char		*handle_redirection_token(const char **cur,
				t_token_type *typ, int *len);
t_token		*create_token(char *value, t_token_type type);
void		add_token(t_token **head, t_token *new_token);
void		free_tokens(t_token *tokens);
t_token		*merge_consecutive_words(t_token *tokens);

//variable_expansion
const char	*extract_var_name(const char *input, int *len, int *is_braced);
int			expand_token_list(t_token *tokens, t_env *env);
int			expand_variable(const char *input,
				t_env *env, char **exp, int *con);
int			expand_tilde(const char *input, t_env *env,
				char *result, int *consumed);
int			handle_special_cases(const char *in, int *args,
				char **exp, int *con);
char		*remove_quotes_string(const char *str);
char		*get_env_variable(t_env *env, const char *var_name, int len);
int			is_word_start(const char *pos, const char *input);
int			expand_tilde_helper(const char **current, char *result,
				int *result_len, t_env *env);
int			expand_variable_helper(const char **current, char *result,
				int *result_len, t_env *env);
char		*expand_string(const char *input, t_env *env);
int			split_unquoted_fields(t_token *current);

//parser
t_cmd		*build_commands(t_token *tokens);
int			build_pipeline(t_cmd *commands);
int			initialize_cmd_arguments(t_cmd *cmd, t_token **tokens);
int			count_args(t_token *tokens);
int			is_redirect_type(t_token_type type);
int			is_valid_arg_type(t_token *curr);
int			handle_redirection(t_cmd *cmd, t_token *current, int type);
int			process_file_fd(t_cmd *cmd, int fd, int type, int target_fd);
void		free_cmd_list(t_cmd *cmd);
t_token		*find_next_pipe(t_token *tokens);
t_token		*find_next_separator(t_token *tokens);
int			process_redirections_in_tokens(t_cmd *cmd, t_token *tokens);
int			process_heredocs(t_cmd *cmds, t_env *env);

//built- ins
int			is_builtin(char *cmd);
int			execute_builtin(char **args, t_env **my_env);
int			builtin_cd(char **args, t_env **env);
int			builtin_pwd(char **args);
int			builtin_echo(char **args);
int			builtin_export(char **args, t_env **env);
int			builtin_unset(char **args, t_env **env);
int			builtin_exit(char **args);
int			builtin_env(char **args, t_env *env);
void		print_sorted_export(t_env *env);
int			handle_export_option_error(char *arg);
char		*get_env_value(t_env *env, char *key);
void		set_env_value(t_env **env, char *key, char *value);
char		**build_env_array(t_env *env);

//executor
int			execute_commands(t_cmd *cmds, t_env **my_env);
int			wait_and_get_exit_status(pid_t pid);
int			run_external(char **args, t_env **envp);
void		free_env_array(char **env_array);
void		setup_redirections(t_cmd *cmd);
void		close_all_pipes(t_cmd *cmds);
void		execute_ast_command_child(t_cmd *cmd, t_env **my_env);
void		execute_child(const char *cmd_path, char **args, char **env_array);
int			is_builtin(char *cmd);
int			count_commands(t_cmd *cmds);
int			fork_and_execute_pipeline(t_cmd *cmds, t_env **my_env,
				pid_t *last_pid);
int			wait_for_children(int child_count, t_cmd *cmds, pid_t last_pid);
int			wait_for_pipeline_children(int cmd_count, pid_t last_pid);
void		execute_pipeline_child(t_cmd *cmd, t_cmd *cmds, t_env **my_env);
int			execute_pipeline(t_cmd *cmds, t_env **envp);

// Signal handling
void		setup_signal_handlers(void);
void		ignore_signals(void);
void		restore_signals(void);
void		signal_handler_sigint(int sig);
void		signal_handler_sigquit(int sig);
int			handle_status_special_case(const char *input, char **exp, int *con);

// path handling
char		*find_command(char *cmd, t_env **envp);
char		*process_directory(char *path_copy,
				char **dir_start, int i, char *cmd);
char		*check_command_in_dir(char *dir, char *cmd);

//error handling
void		free_args(char **args);
#endif