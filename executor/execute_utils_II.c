/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_II.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:55:18 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/17 18:01:54 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_commands(t_cmd *cmds)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmds;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	wait_for_children(int child_count, t_cmd *cmds, pid_t last_pid)
{
	int		exit_status;
	int		last_status;
	pid_t	waited_pid;
	int		i;

	close_all_pipes(cmds);
	exit_status = 0;
	last_status = 1;
	i = 0;
	while (i < child_count)
	{
		waited_pid = waitpid(-1, &exit_status, 0);
		if (waited_pid == last_pid)
			last_status = exit_status;
		i++;
	}
	setup_signal_handlers();
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (1);
}
