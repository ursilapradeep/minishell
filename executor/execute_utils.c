/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:06:43 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/12 17:24:28 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (1);
	return (0);
}

void	close_all_pipes(t_cmd *cmds)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		if (current->infd != STDIN_FILENO)
			close(current->infd);
		if (current->outfd != STDOUT_FILENO)
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

int	fork_and_execute_pipeline(t_cmd *cmds, t_env **my_env, pid_t *last_pid)
{
	t_cmd	*current;
	pid_t	pid;
	int		i;

	current = cmds;
	i = 0;
	while (current)
	{
		ignore_signals();
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
			execute_pipeline_child(current, cmds, my_env);
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
