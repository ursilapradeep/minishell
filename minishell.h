#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

// Function prototypes
char	**split_args(char *input);
void	free_args(char **args);
char	*find_command(char *cmd);
void	execute_command(char **args);
char	*process_directory(char *path_copy, char **dir_start, int i, char *cmd);
char	*check_command_in_dir(char *dir, char *cmd);

#endif
