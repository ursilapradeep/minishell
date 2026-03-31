/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:28 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/31 18:39:49 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" 

static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static char	*prepare_command_input(char *input, t_env *my_env)
{
	char	*cleaned_input;
	char	*expanded_input;

	if (contains_redirection(input))
	{
		cleaned_input = apply_redirections(input);
		if (!cleaned_input)
			return (NULL);
	}
	else
		cleaned_input = ft_strdup(input);
	if (!cleaned_input)
		return (NULL);
	expanded_input = expand_variables(cleaned_input, my_env);
	free(cleaned_input);
	return (expanded_input);
}

/*redirection part handles > file1
command part becomes echo hi
args become {"echo", "hi", NULL}
execute command afterward, stdout/stderr restored by restore_fds()*/
static int	apply_and_execute(char *input, t_env **my_env, int stdin_bak,
		int stdout_bak)
{
	char	**args;
	char	*expanded_input;
	int		exit_code;

	(void)stdin_bak;
	(void)stdout_bak;
	expanded_input = prepare_command_input(input, *my_env);
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

int	handle_pipeline(char *input, t_env **my_env)
{
	char	**pipeline;
	int		status;

	pipeline = parse_pipeline(input);
	status = 1;
	if (pipeline)
	{
		status = execute_pipeline(pipeline, my_env);
		free_args(pipeline);
	}
	return (status);
}

/*input = "echo hi > file1" (full raw command string")
my_env = pointer to your environment list (not "echo")
stdin_backup = result of dup(0) (often 3, but can vary)
stdout_backup = result of dup(1) (often 4, but can vary) */

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
