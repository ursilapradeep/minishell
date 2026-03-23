/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:16:56 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/23 14:28:17 by uvadakku         ###   ########.fr       */
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
	restore_signals(); // Child should react to Ctrl+C/Ctrl+\ with default behavior.
	if (i > 0) // If not first command, read stdin from previous pipe
		dup2(pipes[i - 1][0], STDIN_FILENO); // Redirect STDIN to previous pipe read end
	if (i < cmd_count - 1) // If not last command, write stdout to current pipe
		dup2(pipes[i][1], STDOUT_FILENO); // Redirect STDOUT to current pipe write end
	close_all_pipes(pipes, cmd_count); // Close all inherited pipe fds after dup2 setup
	execute_pipeline_command_or_exit(pipeline[i], envp);
}

static int	wait_pipeline_with_signal_control(int cmd_count, pid_t last_pid)
{
	int	status;

	status = wait_for_pipeline_children(cmd_count, last_pid);
	setup_signal_handlers();
	return (status);
}

static int	fork_and_execute_all(char **pipeline, t_env **envp, int pipes[][2],
		int cmd_count)
{
	pid_t	pid;
	pid_t	last_pid;
	int		i; 	// Start from first command in the pipeline
	
	i = 0;
	last_pid = -1;
	ignore_signals();
	while (i < cmd_count) // Create one child process per command
	{
		pid = fork(); 	// Fork current process
		if (pid == -1) // If fork fails, print error and stop creating more children
		{
			perror("fork");
			setup_signal_handlers();
			close_all_pipes(pipes, cmd_count);
			return (1);
		}
		if (pid == 0) 	// Child process executes its command with proper pipe redirection
			execute_child(pipeline, envp, pipes, cmd_count, i);
		if (i == cmd_count - 1)
			last_pid = pid;
		i++; // Parent moves to next command
	}
	close_all_pipes(pipes, cmd_count); // Parent closes all pipe file descriptors after forking all children
	return (wait_pipeline_with_signal_control(cmd_count, last_pid));
}

// Execute pipeline
/*pipeline (array of command strings, envp: variable passed to executed commands))*/
//pipes[i] has 2 ends: [0] read end, [1] write end
int	execute_pipeline(char **pipeline, t_env **envp)
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
			return (1);
		}
		i++;
	}
	return (fork_and_execute_all(pipeline, envp, pipes, cmd_count));
}
