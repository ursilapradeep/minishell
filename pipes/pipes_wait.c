/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_wait.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:30:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/23 11:16:59 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>

static void	free_env_array(char **env_array)
{
	int	j;

	if (!env_array)
		return ;
	j = 0;
	while (env_array[j])
		free(env_array[j++]);
	free(env_array);
}

static char	*prepare_pipeline_segment(char *segment, t_env *env)
{
	char	*cleaned;
	char	*expanded;

	if (contains_redirection(segment))
	{
		cleaned = apply_redirections(segment);
		if (!cleaned)
			return (NULL);
	}
	else
		cleaned = ft_strdup(segment);
	if (!cleaned)
		return (NULL);
	expanded = expand_variables(cleaned, env);
	free(cleaned);
	return (expanded);
}

void	execute_pipeline_command_or_exit(char *segment, t_env **envp)
{
	char	**args;
	char	*prepared;
	char	*cmd_path;
	char	**env_array;

	prepared = prepare_pipeline_segment(segment, *envp);
	if (!prepared)
		exit(1);
	args = split_args(prepared);
	free(prepared);
	if (!args)
		exit(127);
	cmd_path = find_command(args[0], envp);
	if (!cmd_path)
	{
		printf("minishell: command not found: %s\n", args[0]);
		free_args(args);
		exit(127);
	}
	env_array = build_env_array(*envp);
	execve(cmd_path, args, env_array);
	perror("execve");
	free_env_array(env_array);
	free(cmd_path);
	free_args(args);
	exit(127);
}

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
