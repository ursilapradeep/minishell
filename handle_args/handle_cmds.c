/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:28 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 15:08:37 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" 

/*1. Save original stdin/stdout with dup()
2. Redirect stdin/stdout for the command
3. Execute command
4. restore_fds() puts stdin/stdout back to normal*/
void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

/*redirection part handles > file1
command part becomes echo hi
args become {"echo", "hi", NULL}
execute command afterward, stdout/stderr restored by restore_fds()*/
int	apply_and_execute(char *input, t_env **my_env, int stdin_bak,
		int stdout_bak)
{
	char	**args;
	char	*expanded_input;
	int		exit_code;

	(void)stdin_bak;
	(void)stdout_bak;
	expanded_input = prepare_pipeline_segment(input, *my_env);
	if (!expanded_input)
		return (1);
	args = split_args(expanded_input);
	free(expanded_input);
	if (args)
	{
		exit_code = execute_command(args, my_env);
		free_args(args);
		return (exit_code);
	}
	return (0);
}

/*Input: "echo hi > file.txt"

Before:  stdin = original terminal input
After:   stdin = redirected, stdout = file.txt
restore_fds(): Uses stdin_backup to put stdin back to original*/
int	handle_single_command(char *input, t_env **my_env)
{
	int	stdin_backup;
	int	stdout_backup;
	int	exit_code;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	exit_code = apply_and_execute(input, my_env, stdin_backup, stdout_backup);
	restore_fds(stdin_backup, stdout_backup);
	return (exit_code);
}
