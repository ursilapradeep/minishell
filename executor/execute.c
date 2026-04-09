/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 11:51:52 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/08 21:30:28 by spaipur-         ###   ########.fr       */
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

int	wait_for_children(int child_count, t_cmd *cmds)
{
	int	exit_status;
	int	i;

	close_all_pipes(cmds);
	exit_status = 0;
	i = 0;
	while (i < child_count)
	{
		waitpid(-1, &exit_status, 0);
		i++;
	}
	setup_signal_handlers();
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (exit_status);
}

int	execute_commands(t_cmd *cmds, t_env **my_env)
{
	int	cmd_count;
	int	child_count;

	if (!cmds)
		return (0);
	cmd_count = count_commands(cmds);
	if (cmd_count == 1)
	{
		setup_redirections(cmds);
		if (is_builtin(cmds->args[0]))
			return (execute_builtin(cmds->args, my_env));
		return (run_external(cmds->args, my_env));
	}
	child_count = fork_and_execute_pipeline(cmds, my_env);
	return (wait_for_children(child_count, cmds));
}
