/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:16:56 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/18 11:01:00 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_all_pipes(int pipes[][2], int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]); //close read descriptor
		close(pipes[i][1]); //close write
		i++;
	}
}
/*After dup2, you close all pipe fds because:
They are no longer needed in that process
stdin/stdout now already point to the needed pipe ends.
Avoid fd leaks
Keeping extra pipe descriptors open wastes resources. */

static void	execute_child(char **pipeline, t_env **envp, int pipes[][2], // Child handler for one command in pipeline
 int cmd_count, int i) // cmd_count = total commands, i = current command index
{
	char	**args; // Tokenized arguments for current command (e.g., ["ls", "-l", NULL])
	char	*cmd_path; // Full executable path (e.g., "/bin/ls")

	if (i > 0) // If not first command, read stdin from previous pipe
		dup2(pipes[i - 1][0], STDIN_FILENO); // Redirect STDIN to previous pipe read end
	if (i < cmd_count - 1) // If not last command, write stdout to current pipe
		dup2(pipes[i][1], STDOUT_FILENO); // Redirect STDOUT to current pipe write end
	close_all_pipes(pipes, cmd_count); // Close all inherited pipe fds after dup2 setup
	args = split_args(pipeline[i]); // Split current pipeline command string into argv array
	if (args) // Proceed only if argument parsing succeeded
	{
		cmd_path = find_command(args[0], envp); // Resolve command name using PATH/environment
		if (cmd_path) // If executable path found
		{
			char **env_array = build_env_array(*envp); // Build env array from shell's t_env list
			execve(cmd_path, args, env_array); // Pass environment to child process
			perror("execve"); // Runs only if execve fails
			if (env_array) // Free env array on execve failure
			{
				int j = 0;
				while (env_array[j])
					free(env_array[j++]);
				free(env_array);
			}
			free(cmd_path); // Free allocated command path on failure
		}
		else // If command path could not be resolved
						printf("minishell: command not found: %s\n", args[0]); // Print shell-style error
		free_args(args); // Free parsed argument array
	}
	exit(127); // Exit child with "command execution failed" status
}

static void	fork_and_execute_all(char **pipeline, t_env **envp, int pipes[][2],
		int cmd_count)
{
	pid_t	pid;
	int		i; 	// Start from first command in the pipeline
	
	i = 0;
	while (i < cmd_count) // Create one child process per command
	{
		pid = fork(); 	// Fork current process
		if (pid == -1) // If fork fails, print error and stop creating more children
		{
			perror("fork");
			return ;
		}
		if (pid == 0) 	// Child process executes its command with proper pipe redirection
			execute_child(pipeline, envp, pipes, cmd_count, i);
		i++; // Parent moves to next command
	}
	close_all_pipes(pipes, cmd_count); // Parent closes all pipe file descriptors after forking all children
	i = 0; // Wait for all child processes to finish
	while (i < cmd_count)
	{
		wait(NULL);
		i++;
	}
}

// Execute pipeline
/*pipeline (array of command strings, envp: variable passed to executed commands))*/
//pipes[i] has 2 ends: [0] read end, [1] write end
void	execute_pipeline(char **pipeline, t_env **envp)
{
	int		cmd_count; //stores how many cmds are in the pipeline
	int		pipes[1024][2]; //array supports upto 1024 pipes (about 1025 commands)
	int		i;

	i = 0;
	cmd_count = 0;
	while (pipeline[cmd_count]) //loops until a null entry is found
		cmd_count++;
	i = 0;
	/*Because for N commands, you need N - 1 pipes to connect them.
	Example:
	cmd1 | cmd2 = 2 commands → 1 pipe needed
	cmd1 | cmd2 | cmd3 = 3 commands → 2 pipes needed */
	while (i < cmd_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return ;
		}
		i++;
	}
	fork_and_execute_all(pipeline, envp, pipes, cmd_count);
}
