/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_wait_d.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:30:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 15:35:53 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	child_status_to_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 2)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int	wait_for_pipeline_children(int cmd_count, pid_t last_pid)
{
	pid_t	waited_pid;
	int		status;
	int		last_status;
	int		i;

	i = 0;
	last_status = 1;
	while (i < cmd_count)
	{
		waited_pid = waitpid(-1, &status, 0);
		if (waited_pid == -1)
		{
			perror("waitpid");
			return (1);
		}
		if (waited_pid == last_pid)
			last_status = child_status_to_exit_code(status);
		i++;
	}
	return (last_status);
}

int	wait_pipeline_with_signal_control(int cmd_count, pid_t last_pid)
{
	int	status;

	status = wait_for_pipeline_children(cmd_count, last_pid);
	setup_signal_handlers();
	return (status);
}
