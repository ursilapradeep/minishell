/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmds_with_pipes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:06:43 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/14 22:27:17 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_redirections(t_cmd *cmd)
{
	if (cmd->infd > 2)
	{
		dup2(cmd->infd, STDIN_FILENO);
		close(cmd->infd);
	}
	else if (cmd->infd == -2)
	{
		exit(1);
	}
	if (cmd->outfd > 2)
	{
		dup2(cmd->outfd, STDOUT_FILENO);
		close(cmd->outfd);
	}
	else if (cmd->outfd == -2)
	{
		exit(1);
	}
	if (cmd->errfd > 2)
	{
		dup2(cmd->errfd, STDERR_FILENO);
		close(cmd->errfd);
	}
	else if (cmd->errfd == -2)
		exit(1);
}

void	close_all_pipes(t_cmd *cmds)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		if (current->infd > 2)
			close(current->infd);
		if (current->outfd > 2)
			close(current->outfd);
		if (current->errfd > 2)
			close(current->errfd);
		current = current->next;
	}
}

void	execute_pipeline_child(t_cmd *cmd, t_cmd *cmds, t_env **my_env)
{
	setup_redirections(cmd);
	close_all_pipes(cmds);
	restore_signals();
	execute_ast_command_child(cmd, my_env);
}

static int	fork_one_pipeline_cmd(t_cmd *current, t_cmd *cmds,
		t_env **my_env, pid_t *pid)
{
	ignore_signals();
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (*pid == 0)
		execute_pipeline_child(current, cmds, my_env);
	return (0);
}

int	fork_and_execute_pipeline(t_cmd *cmds, t_env **my_env, pid_t *last_pid)
{
	t_cmd	*current;
	pid_t	pid;
	int		i;

	current = cmds;
	i = 0;
	while (current)
	{
		if (fork_one_pipeline_cmd(current, cmds, my_env, &pid) < 0)
			return (1);
		i++;
		if (!current->has_pipe)
		{
			*last_pid = pid;
			break ;
		}
		current = current->next;
	}
	return (i);
}
