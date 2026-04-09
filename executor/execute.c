/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 11:51:52 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/09 16:17:52 by uvadakku         ###   ########.fr       */
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

static int	execute_single_command(t_cmd *cmd, t_env **my_env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
		return (1);
	setup_redirections(cmd);
	if (is_builtin(cmd->args[0]))
		status = execute_builtin(cmd->args, my_env);
	else
		status = run_external(cmd->args, my_env);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

int	execute_commands(t_cmd *cmds, t_env **my_env)
{
	int	cmd_count;
	int	child_count;

	if (!cmds)
		return (0);
	cmd_count = count_commands(cmds);
	if (cmd_count == 1)
		return (execute_single_command(cmds, my_env));
	child_count = fork_and_execute_pipeline(cmds, my_env);
	return (wait_for_children(child_count, cmds));
}
