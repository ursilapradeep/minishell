/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:06:43 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 15:35:21 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Child runs ls successfully → normal exit 0 → function returns 0.
Child killed by SIGINT (2) → function returns 130 (128 + 2*/
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

/*This function waits for a child process to finish, 
restores the shell’s signal handling, frees any allocated 
resources for the command execution, and returns the child’s exit status.*/
int	wait_and_cleanup_external(pid_t pid, char **env_array,
		char *cmd_path)
{
	int	exit_status;

	exit_status = wait_and_get_exit_status(pid);
	setup_signal_handlers();
	free_env_array(env_array);
	free(cmd_path);
	return (exit_status);
}
