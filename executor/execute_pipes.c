/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:16:56 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/12 17:24:28 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_inherited_fds(t_cmd *cmds)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		if (current->infd != STDIN_FILENO)
			close(current->infd);
		if (current->outfd != STDOUT_FILENO)
			close(current->outfd);
		current = current->next;
	}
}

/*After dup2, you close all pipe fds because:
They are no longer needed in that process
stdin/stdout now already point to the needed pipe ends.
Avoid fd leaks
Keeping extra pipe descriptors open wastes resources. */
static void	execute_pipeline_cmd(t_cmd *cmd, t_env **envp)
{
	restore_signals();
	if (cmd->infd != STDIN_FILENO)
		dup2(cmd->infd, STDIN_FILENO);
	if (cmd->outfd != STDOUT_FILENO)
		dup2(cmd->outfd, STDOUT_FILENO);
	execute_pipeline_child(cmd, NULL, envp);
}

static int	wait_pipeline_with_signal_control(int cmd_count, pid_t last_pid)
{
	int	status;

	status = wait_for_pipeline_children(cmd_count, last_pid);
	setup_signal_handlers();
	return (status);
}

static int	fork_all_commands(t_cmd *cmds, t_env **envp, pid_t *last_pid,
		int *cmd_count)
{
	t_cmd	*current;
	pid_t	pid;

	current = cmds;
	ignore_signals();
	while (current)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			setup_signal_handlers();
			close_inherited_fds(cmds);
			return (1);
		}
		if (pid == 0)
			execute_pipeline_cmd(current, envp);
		if (current->next == NULL)
			*last_pid = pid;
		current = current->next;
		(*cmd_count)++;
	}
	close_inherited_fds(cmds);
	return (0);
}

int	execute_pipeline(t_cmd *cmds, t_env **envp)
{
	pid_t	last_pid;
	int		cmd_count;

	if (!cmds)
		return (0);
	cmd_count = 0;
	last_pid = -1;
	if (fork_all_commands(cmds, envp, &last_pid, &cmd_count) == 1)
		return (1);
	return (wait_pipeline_with_signal_control(cmd_count, last_pid));
}
