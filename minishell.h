/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:28:29 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/02 16:46:42 by uvadakku         ###   ########.fr       */
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
#include <ctype.h>
#include <signal.h>
#include <sys/signal.h>
# include <sys/types.h>
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

typedef struct s_pipe_seg_ctx
{
	char	**cmds;
	char	*input;
	int		start;
	int		end;
	int		idx;
}t_pipe_seg_ctx;

typedef struct s_child_exec_ctx
{
	char	**pipeline;
	t_env	**envp;
	int		(*pipes)[2];
	int		cmd_count;
	int		i;
}t_child_exec_ctx;

typedef struct s_redir_proc_ctx
{
	char	*cmd;
	int		i;
	int		op_pos;
	int		op_type;
	char	*target;
	char	*tmp;
}t_redir_proc_ctx;

typedef struct s_expand_ctx
{
	char	*expanded;
	char	quote;
	int		i;
	int		j;
}t_expand_ctx;

// Quote/expansion helpers
typedef struct s_token_len_ctx
{
	char	*input;
	int	*i;
	char	*quote;
	t_env	*env;
	int	*out_len;
}t_token_len_ctx;

typedef struct s_var_value_len_ctx
{
	char	*input;
	int		i;
	t_env	*env;
	int		*skip;
	int		len;
	int		key_len;
	char	*key;
	char	*value;
	char	*status_str;
}t_var_value_len_ctx;

typedef struct s_process_char_ctx
{
	char	*expanded;
	int		*j;
	char	*input;
	int		*i;
	char	*quote;
	t_env	*env;
}t_process_char_ctx;

#ifdef rl_done
 rl_done = 0;
#endif

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

char	**split_args(char *input);
void	free_args(char **args);

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

/*--env--*/
char	**build_env_array(t_env *env);
//env_utils.c
t_env 	*init_env(char **envp);
void 	add_env_node(t_env **env_list, char *env_str);
int	fill_env_node(t_env *new_node, char *env_str);
/*--execute--*/
//execute_utils.c
int	wait_and_get_exit_status(pid_t pid);
int	wait_and_cleanup_external(pid_t pid, char **env_array,
		char *cmd_path);
//execute.c
void	run_child(const char *cmd_path, char **args, char **env_array);
int	prepare_external(char **args, t_env **envp, char **cmd_path,
		char ***env_array);
int	run_external(char **args, t_env **envp);
int	execute_command(char **args, t_env **envp);
/*---handle_args---*/
//args_helper.c
int		is_space(char c);
int		word_end(char *input, int i);
int		word_len_without_quotes(char *input, int start, int end);
void	copy_without_quotes(char *dst, char *input, int start, int end);
//handle_args.c
int		count_args_from_input(char *input);
int	add_word(char **args, t_word_info *info, int index);
int	fill_args(char **args, char *input);
char	**split_args(char *input);
//handle_cmds.c
void	restore_fds(int stdin_backup, int stdout_backup);
int	apply_and_execute(char *input, t_env **my_env, int stdin_bak,
		int stdout_bak);
int	handle_single_command(char *input, t_env **my_env);
//quote_utils.c
int		append_var_value(t_process_char_ctx *ctx);
int		process_next_char(t_process_char_ctx *ctx);
//quote_helper.c
int		is_var_char(char c);
int		handle_exit_status_var(t_var_value_len_ctx *ctx);
int		var_value_len(char *input, int i, t_env *env, int *skip);
//handle_quote.c
int	handle_token_len_special_cases(t_token_len_ctx *ctx,
	int *skip, int *vlen);
int		count_token_len(t_token_len_ctx *ctx);
int		expanded_len(char *input, t_env *env);
void	init_process_char_ctx(t_process_char_ctx *char_ctx,
		t_expand_ctx *ctx, char *input, t_env *env);
char	*expand_variables(char *input, t_env *env);
//process_input_utils.c
int	is_invalid_pipe_position(char *input, int i);
int		has_unclosed_quotes(char *input);
int		find_logical_and(char *input);
int	extract_and_trim_commands(char *input, int and_pos,
	char **left_trim, char **right_trim);
int		handle_logical_and(char *input, t_env **my_env, int and_pos);
//process_input.c
int	has_invalid_pipe_syntax(char *input);
int		process_input(char *input, t_env **my_env);
char	*append_char_to_line(char *line, char buf);
char	*read_non_interactive_line(void);
char	*read_input(void);

/*----handle_path---*/
//find_path.c
char	*search_directories(char *path_copy, char *cmd);
char	*search_in_path(char *path_env, char *cmd);
char *get_path_from_env(char **envp);
char *find_command(char *cmd, t_env **envp);
//path_helpers.
char	*build_full_path(char *dir, char *cmd);
char	*process_directory(char *path_copy, char **dir_start, int i, char *cmd);
char	*check_command_in_dir(char *dir, char *cmd);

/*----pipes---*/
//pipes.c
char	**parse_pipeline(char *input);
int		handle_pipeline(char *input, t_env **my_env);
int		contains_pipe(char *input);
//pipes_utils_a.c
void	free_partial_pipeline(char **cmds, int count);
int	append_segment_or_fail(t_pipe_seg_ctx *ctx);
int	handle_pipe_delimiter(t_pipe_seg_ctx *ctx, int i, int *j);
int	split_pipe_segments(char **cmds, char *input, int *last_idx);
int	count_pipes(char *input);
//pipes_utils_b.c
void	close_all_pipes(int pipes[][2], int cmd_count);
int	fork_one_child(t_child_exec_ctx *ctx, int i, pid_t *last_pid);
int	fork_and_execute_all(char **pipeline, t_env **envp, int pipes[][2],
		int cmd_count);
int		execute_pipeline(char **pipeline, t_env **envp);
//pipes_executable_c.c
void	free_env_array(char **env_array);
char	*prepare_pipeline_segment(char *segment, t_env *env);
void	exec_or_fail(char **args, t_env **envp);
void	execute_pipeline_command_or_exit(char *segment, t_env **envp);
//pipes_wait_d.c
int		child_status_to_exit_code(int status);
int		wait_for_pipeline_children(int cmd_count, pid_t last_pid);
int		wait_pipeline_with_signal_control(int cmd_count, pid_t last_pid);

/*----redirections--*/
//execute_redirections.c
char	*remove_substring(const char *input, int start, int end);
int	handle_input_redirect(char *filename);
//heredoc.c
int	read_heredoc_input(int write_fd, char *delimiter);
int	handle_heredoc(char *delimiter);
int	handle_output_redirect(char *filename);
int	handle_append_redirect(char *filename);
int	execute_redirection(int op_type, char *target);
//redirections.c
char	*skip_spaces_and_extract(char *input, int *i);
int	identify_and_skip_operator(char *input, int *i);
int	process_and_remove_redirection(char **cmd_ptr, int *i_ptr);
char	*apply_redirections(char *input);
int	contains_redirection(char *input);

/*----Signal handling--*/
//signal.c
void setup_signal_handlers(void);
void ignore_signals(void);
void restore_signals(void);
void signal_handler_sigint(int sig);
void signal_handler_sigquit(int sig);

#endif
