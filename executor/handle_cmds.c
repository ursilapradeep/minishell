/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:28 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/31 21:00:58 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// parse is already done in parser you just have to execute it .
int	handle_single_command(t_cmd *cmd, t_env **my_env);
int	handle_pipeline(t_cmd *cmd, t_env **my_env);                                 

static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);                       // Restore original stdin from backup
	dup2(stdout_backup, STDOUT_FILENO);                     // Restore original stdout from backup
	close(stdin_backup);                                    // Close backup stdin file descriptor
	close(stdout_backup);                                   // Close backup stdout file descriptor
}

static int	apply_and_execute(char *input, t_env **my_env, int stdin_bak,
		int stdout_bak)
{
	char	**args;                                          // Array to store command arguments
	int		exit_code;
	
	if (args)                                               // If argument splitting succeeded
	{
		exit_code = execute_command(args, my_env);          // Execute the command with arguments
		free_args(args);                                    // Free the argument array
		return (exit_code);                                 // Return command's exit status
	}
	return (0);                                             // Return success if no args to execute
}

int	handle_pipeline(char *input, t_env **my_env)
{
	char	**pipeline;                                     // Array to store individual commands from pipeline
	int		status;
	                   // Parse input and split by pipe characters
	status = 1;
	if (pipeline)                                           // If parsing succeeded
	{
		status = execute_pipeline(pipeline, my_env);        // Execute all commands in pipeline with pipes
		free_args(pipeline);                                // Free the pipeline command array
	}
	return (status);                                        // Return pipeline exit status
}

/*input = "echo hi > file1" (full raw command string")
my_env = pointer to your environment list (not "echo")
stdin_backup = result of dup(0) (often 3, but can vary)
stdout_backup = result of dup(1) (often 4, but can vary) */

int	handle_single_command(char *input, t_env **my_env)
{
	int	stdin_backup;                                       /* File descriptor to store original stdin, stdout*/
	int	stdout_backup;                        
	int	exit_code;

	stdin_backup = dup(STDIN_FILENO);                       /* Duplicate stdin, stdout to backup original */
	stdout_backup = dup(STDOUT_FILENO);                     
	exit_code = apply_and_execute(input, my_env, stdin_backup, stdout_backup); // Process redirections and execute command
	restore_fds(stdin_backup, stdout_backup);               // Restore original stdin/stdout and close backups
	return (exit_code);
}
