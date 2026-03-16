/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:28 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/16 15:51:59 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"                                   
// Restore stdin/stdout and close backup file descriptors
static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);                       // Restore original stdin from backup
	dup2(stdout_backup, STDOUT_FILENO);                     // Restore original stdout from backup
	close(stdin_backup);                                    // Close backup stdin file descriptor
	close(stdout_backup);                                   // Close backup stdout file descriptor
}

// Apply redirections, split args, and execute command
static int	apply_and_execute(char *input, t_env **my_env, int stdin_bak,
		int stdout_bak)
{
	char	**args;                                          // Array to store command arguments
	char	*cleaned_input;                                 // Input string after processing redirections
	char	*expanded_input;
	int		exit_code;                                      // Exit status from command execution

	(void)stdin_bak;                                        // Mark parameter as unused (avoid warning)
	(void)stdout_bak;                                       // Mark parameter as unused (avoid warning)
	if (contains_redirection(input))                        // Check if input has redirection operators
	{
		cleaned_input = apply_redirections(input);          // Process redirections and return cleaned command
		if (!cleaned_input)                                 // If redirection processing failed
			return (-1);                                    // Return error code
	}
	else
		cleaned_input = ft_strdup(input);                   // No redirections, duplicate input as-is
	expanded_input = expand_variables(cleaned_input, *my_env);
	free(cleaned_input);                                    // Free cleaned input string (no longer needed)
	if (!expanded_input)
		return (1);
	args = split_args(expanded_input);                      // Split cleaned input into argument array
	free(expanded_input);
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

	pipeline = parse_pipeline(input);                       // Parse input and split by pipe characters
	if (pipeline)                                           // If parsing succeeded
	{
		execute_pipeline(pipeline, my_env);                 // Execute all commands in pipeline with pipes
		free_args(pipeline);                                // Free the pipeline command array
	}
	return (0);                                             // Return success status
}

int	handle_single_command(char *input, t_env **my_env)
{
	int	stdin_backup;                                       // File descriptor to store original stdin
	int	stdout_backup;                                      // File descriptor to store original stdout
	int	exit_code;

	stdin_backup = dup(STDIN_FILENO);                       // Duplicate stdin to backup original
	stdout_backup = dup(STDOUT_FILENO);                     // Duplicate stdout to backup original
	exit_code = apply_and_execute(input, my_env, stdin_backup, stdout_backup); // Process redirections and execute command
	restore_fds(stdin_backup, stdout_backup);               // Restore original stdin/stdout and close backups
	return (exit_code);
}
