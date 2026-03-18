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
}				t_env;

typedef struct s_cmd
{
	char			**args;
	int				infd;
	int				outfd;
	struct s_cmd	*next;
}				t_cmd;

// Function prototypes
t_env 	*init_env(char **envp);
void 	add_env_node(t_env **env_list, char *env_str);
char	**split_args(char *input);
void	free_args(char **args);
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

// Quote/expansion helpers
char	*expand_variables(char *input, t_env *env);
int		is_var_char(char c);
int		count_token_len(char *input, int *i, char *quote,
			t_env *env, int *out_len);

// Execute helpers
int		wait_and_get_exit_status(pid_t pid);

#endif
