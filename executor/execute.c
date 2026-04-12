/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:+:      :+:+:+:    */
/*                                                    +#++#+    +#++#+       */
/*   By: spaipur- <spaipur-@student.42.fr>          +#++#+    +#++#+        */
/*                                                #######  ####### #######   */
/*   Created: 2026/03/03 11:51:52 by spaipur-          #+#    #+#    #+#     */
/*   Updated: 2026/04/11 22:53:00 by spaipur-         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_single_command(t_cmd *cmd, t_env **my_env);

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
	int		exit_status;
	int		i;
	t_cmd	*last_cmd;

	close_all_pipes(cmds);
	exit_status = 0;
	i = 0;
	last_cmd = cmds;
	while (last_cmd && last_cmd->next)
		last_cmd = last_cmd->next;
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

int	execute_single_command(t_cmd *cmd, t_env **my_env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0 || saved_stderr < 0)
		return (1);
	if (cmd->infd == -2 || cmd->outfd == -2 || cmd->errfd == -2)
		return (restore_std_io(saved_stdin, saved_stdout, saved_stderr), 1);
	setup_redirections(cmd);
	if (!cmd->args || !cmd->args[0]
		|| (cmd->args[0][0] == '\0' && !cmd->args[1]))
	{
		restore_std_io(saved_stdin, saved_stdout, saved_stderr);
		return (0);
	}
	status = exec_single_logic(cmd, my_env);
	restore_std_io(saved_stdin, saved_stdout, saved_stderr);
	return (status);
}
