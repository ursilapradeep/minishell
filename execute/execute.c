/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 11:51:52 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/23 14:28:19 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" // Include command execution, env, and process helper declarations.

static void	free_env_array(char **env_array)
{
	int	index; // Index used to traverse env string array.

	if (!env_array) // Guard against NULL pointer input.
		return ; // Nothing to free.
	index = 0; // Start from first environment string.
	while (env_array[index]) // Free each "KEY=VALUE" entry.
	{
		free(env_array[index]); // Release one environment entry string.
		index++; // Move to next entry.
	}
	free(env_array); // Release top-level char** container.
}

void	execute_child(const char *cmd_path, char **args, char **env_array)
{
	execve(cmd_path, args, env_array); // Replace child process image with target executable.
	perror("execve"); // Reaches here only when execve fails.
	free_env_array(env_array); // Free env snapshot on failure path.
	exit(127); // Exit child with command-not-executable style status.
}

static int	prepare_external(char **args, t_env **envp, char **cmd_path,
		char ***env_array)
{
	*cmd_path = find_command(args[0], envp); // Resolve command path from PATH or direct path.
	if (!*cmd_path) // Command resolution failed.
	{
		printf("minishell: command not found: %s\n", args[0]); // Print shell-like not found error.
		return (127); // Return standard command-not-found status.
	}
	*env_array = build_env_array(*envp); // Build execve-ready environment array.
	if (!*env_array) // Allocation/build failure.
	{
		free(*cmd_path); // Clean resolved command path on failure.
		perror("malloc"); // Report allocation error.
		return (1); // Generic execution preparation failure.
	}
	return (0); // Preparation successful.
}

static int	wait_and_cleanup_external(pid_t pid, char **env_array,
		char *cmd_path)
{
	int	exit_status;

	exit_status = wait_and_get_exit_status(pid); // Parent waits and collects child exit code.
	setup_signal_handlers();
	free_env_array(env_array); // Parent frees env snapshot after child finished.
	free(cmd_path); // Parent frees resolved path string.
	return (exit_status); // Return command status to shell loop.
}

int	run_external(char **args, t_env **envp)
{
	char	*cmd_path; // Absolute or relative executable path to run.
	char	**env_array; // Environment snapshot passed to execve.
	pid_t	pid; // Child process id after fork.
	int		prep_status; // Status from path/env preparation step.

	prep_status = prepare_external(args, envp, &cmd_path, &env_array); // Prepare command path and env array.
	if (prep_status != 0) // Stop if command/env prep failed.
		return (prep_status);
	ignore_signals();
	pid = fork(); 
	if (pid == -1) 
	{
		perror("fork"); // Report system fork error.
		setup_signal_handlers();
		free_env_array(env_array); // Cleanup prepared env array.
		free(cmd_path); // Cleanup resolved command path.
		return (1); // Return failure to caller.
	}
	if (pid == 0) // Child process branch.
	{
		restore_signals(); // Child should use default signal behavior (bash-like).
		execute_child(cmd_path, args, env_array); // Execute target program in child.
	}
	return (wait_and_cleanup_external(pid, env_array, cmd_path));
}

int execute_command(char **args, t_env **envp)
{
	if (!args || !args[0]) // Empty command guard.
		return (0); // Nothing to execute.
	if (is_builtin(args[0])) // Builtin command path.
	{
		return (execute_builtin(args, envp)); // Execute builtin in shell process.
	}
	return (run_external(args, envp)); // Execute non-builtin via fork/exec.
}
