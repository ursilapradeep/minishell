/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmds_without_pipe.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 11:51:52 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/13 16:52:59 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	restore_std_io(int saved_stdin, int saved_stdout, int saved_stderr)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
	return (0);
}

static int	exec_single_logic(t_cmd *cmd, t_env **my_env)
{
	int	is_env_with_args;

	is_env_with_args = (ft_strncmp(cmd->args[0], "env", 4) == 0
			&& cmd->args[0][3] == '\0' && cmd->args[1]);
	if (is_builtin(cmd->args[0]) && !is_env_with_args)
		return (execute_builtin(cmd->args, my_env));
	return (run_external(cmd->args, my_env));
}

static int	handle_variable_assignment_if_needed(t_cmd *cmd, t_env **my_env)
{
	if (cmd->args && cmd->args[0]
		&& ft_strchr(cmd->args[0], '=')
		&& cmd->args[0][0] != '='
		&& !cmd->args[1])
	{
		set_env_value(my_env, cmd->args[0], NULL);
		return (1);
	}
	return (0);
}

static int	backup_std_io(int *saved)
{
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	saved[2] = dup(STDERR_FILENO);
	if (saved[0] < 0 || saved[1] < 0 || saved[2] < 0)
		return (1);
	return (0);
}

int	execute_single_command(t_cmd *cmd, t_env **my_env)
{
	int	saved[3];
	int	status;

	if (backup_std_io(saved))
		return (1);
	if (cmd->infd == -2 || cmd->outfd == -2 || cmd->errfd == -2)
		return (restore_std_io(saved[0], saved[1], saved[2]), 1);
	setup_redirections(cmd);
	if (!cmd->args || !cmd->args[0]
		|| handle_variable_assignment_if_needed(cmd, my_env))
	{
		restore_std_io(saved[0], saved[1], saved[2]);
		return (0);
	}
	status = exec_single_logic(cmd, my_env);
	restore_std_io(saved[0], saved[1], saved[2]);
	return (status);
}
