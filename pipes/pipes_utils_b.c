/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_b.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 15:10:38 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 15:19:47 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_all_pipes(int pipes[][2], int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

/*Each child sets its needed stdin/stdout with dup2(),
then closes all pipe FDs to avoid leaks/hangs.*/
static void	execute_child(t_child_exec_ctx *ctx)
{
	restore_signals();
	if (ctx->i > 0)
		dup2(ctx->pipes[ctx->i - 1][0], STDIN_FILENO);
	if (ctx->i < ctx->cmd_count - 1)
		dup2(ctx->pipes[ctx->i][1], STDOUT_FILENO);
	close_all_pipes(ctx->pipes, ctx->cmd_count);
	execute_pipeline_command_or_exit(ctx->pipeline[ctx->i], ctx->envp);
}

int	fork_one_child(t_child_exec_ctx *ctx, int i, pid_t *last_pid)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), setup_signal_handlers(),
			close_all_pipes(ctx->pipes, ctx->cmd_count), 1);
	if (pid == 0)
	{
		ctx->i = i;
		execute_child(ctx);
	}
	if (i == ctx->cmd_count - 1)
		*last_pid = pid;
	return (0);
}

/*Command: "echo hello | grep h | wc -l"

Fork 1: Child executes "echo hello" (writes to pipes[0])
Fork 2: Child executes "grep h" 
(reads from pipes[0], writes to pipes[1])
Fork 3: Child executes "wc -l" 
(reads from pipes[1], last_pid = this PID)
Parent closes all pipes, waits for children
Returns exit status of last command (wc -l)
# Expected output : 1 */
int	fork_and_execute_all(char **pipeline, t_env **envp, int pipes[][2],
		int cmd_count)
{
	t_child_exec_ctx	ctx;
	pid_t				last_pid;
	int					i;

	i = 0;
	last_pid = -1;
	ctx.pipeline = pipeline;
	ctx.envp = envp;
	ctx.pipes = pipes;
	ctx.cmd_count = cmd_count;
	ignore_signals();
	while (i < cmd_count)
	{
		if (fork_one_child(&ctx, i, &last_pid) == 1)
			return (1);
		i++;
	}
	close_all_pipes(pipes, cmd_count);
	return (wait_pipeline_with_signal_control(cmd_count, last_pid));
}

// Execute pipeline
/*Input: ["echo hello", "grep h", "wc -l"]

cmd_count = 3
Create pipes[0] and pipes[1] (2 pipes for 3 commands)
fork_and_execute_all() forks 3 children:
  - Child 1: echo hello → stdout → pipes[0] write end
  - Child 2: grep h → stdin ← pipes[0] read end, stdout → pipes[1] write end
  - Child 3: wc -l → stdin ← pipes[1] read end, stdout → terminal */
int	execute_pipeline(char **pipeline, t_env **envp)
{
	int		cmd_count;
	int		pipes[1024][2];
	int		i;

	i = 0;
	cmd_count = 0;
	while (pipeline[cmd_count])
		cmd_count++;
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
