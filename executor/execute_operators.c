/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:53:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/14 22:27:17 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	execute_current_cmd(t_cmd **current, t_env **my_env)
{
	int		pipeline_count;
	int		child_count;
	int		status;
	pid_t	last_pid;

	pipeline_count = count_pipeline_cmds(*current);
	if (pipeline_count == 1)
		status = execute_single_command(*current, my_env);
	else
	{
		last_pid = -1;
		child_count = fork_and_execute_pipeline(*current, my_env, &last_pid);
		status = wait_for_children(child_count, *current, last_pid);
	}
	return (status);
}

static t_cmd	*get_pipeline_end(t_cmd *current)
{
	while (current->has_pipe && current->next)
		current = current->next;
	return (current);
}

static t_cmd	*advance_current(t_cmd *current, int status)
{
	t_cmd	*end;
	t_cmd	*next_segment;

	end = get_pipeline_end(current);
	if (should_short_circuit(status, end->next_op))
	{
		next_segment = end->next;
		if (!next_segment)
			return (NULL);
		while (next_segment->has_pipe && next_segment->next)
			next_segment = next_segment->next;
		return (next_segment->next);
	}
	return (end->next);
}

int	execute_commands(t_cmd *cmds, t_env **my_env)
{
	int		status;
	t_cmd	*current;

	if (!cmds)
		return (0);
	status = 0;
	current = cmds;
	while (current)
	{
		status = execute_current_cmd(&current, my_env);
		current = advance_current(current, status);
		if (!current)
			return (status);
	}
	return (status);
}
