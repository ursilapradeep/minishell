/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 11:51:52 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/31 17:03:17 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_env_array(char **env_array)
{
	int	index;

	if (!env_array)
		return ;
	index = 0;
	while (env_array[index])
	{
		free(env_array[index]);
		index++;
	}
	free(env_array);
}

void	execute_child(const char *cmd_path, char **args, char **env_array)
{
	execve(cmd_path, args, env_array);
	perror("execve");
	free_env_array(env_array);
	exit(127);
}

static int	prepare_external(char **args, t_env **envp, char **cmd_path,
		char ***env_array)
{
	*cmd_path = find_command(args[0], envp);
	if (!*cmd_path)
	{
		printf("minishell: command not found: %s\n", args[0]);
		return (127);
	}
	*env_array = build_env_array(*envp);
	if (!*env_array)
	{
		free(*cmd_path);
		perror("malloc");
		return (1);
	}
	return (0);
}

/*The parent shell ignores Ctrl+C (because of ignore_signals()).
The child process (running cat) has
default signal handling (because of restore_signals()), 
so it is terminated by Ctrl+C.*/
int	run_external(char **args, t_env **envp)
{
	char	*cmd_path;
	char	**env_array;
	pid_t	pid;
	int		prep_status;

	prep_status = prepare_external(args, envp, &cmd_path, &env_array);
	if (prep_status != 0)
		return (prep_status);
	ignore_signals();
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		setup_signal_handlers();
		free_env_array(env_array);
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		restore_signals();
		execute_child(cmd_path, args, env_array);
	}
	return (wait_and_cleanup_external(pid, env_array, cmd_path));
}

int	execute_command(char **args, t_env **envp)
{
	if (!args || !args[0])
		return (0);
	if (is_builtin(args[0]))
	{
		return (execute_builtin(args, envp));
	}
	return (run_external(args, envp));
}
