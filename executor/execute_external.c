/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:06:43 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/12 17:24:28 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/signal.h>
#include <errno.h>

int	wait_and_get_exit_status(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

static int	wait_and_cleanup_external(pid_t pid, char **env_array,
		char *cmd_path)
{
	int	exit_status;

	exit_status = wait_and_get_exit_status(pid);
	setup_signal_handlers();
	free_env_array(env_array);
	free(cmd_path);
	return (exit_status);
}

void	execute_child(const char *cmd_path, char **args, char **env_array)
{
	int			exit_code;
	int			saved_errno;
	struct stat	st;

	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		free_env_array(env_array);
		exit(126);
	}
	execve(cmd_path, args, env_array);
	saved_errno = errno;
	perror(args[0]);
	free_env_array(env_array);
	exit_code = 127;
	if (saved_errno == EACCES || saved_errno == EPERM
		|| saved_errno == EISDIR || saved_errno == ENOEXEC)
		exit_code = 126;
	exit(exit_code);
}

static int	prepare_external(char **args, t_env **envp, char **cmd_path,
		char ***env_array)
{
	*cmd_path = find_command(args[0], envp);
	if (!*cmd_path)
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return (127);
	}
	set_env_value(envp, "_", *cmd_path);
	*env_array = build_env_array(*envp);
	if (!*env_array)
	{
		free(*cmd_path);
		perror("malloc");
		return (1);
	}
	return (0);
}

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
		return (perror("fork"), setup_signal_handlers(),
			free_env_array(env_array), free(cmd_path), 1);
	if (pid == 0)
		return (restore_signals(), execute_child(cmd_path, args, env_array), 0);
	return (wait_and_cleanup_external(pid, env_array, cmd_path));
}
