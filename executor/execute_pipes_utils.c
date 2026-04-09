/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_wait.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:30:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/09 12:42:26 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>

static int	child_status_to_exit_code(int status)
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
