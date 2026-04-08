/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_executable_c.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 15:28:28 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/08 13:11:15 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_array(char **env_array)
{
	int	j;

	if (!env_array)
		return ;
	j = 0;
	while (env_array[j])
		free(env_array[j++]);
	free(env_array);
}

/*Input: "echo $USER > out.txt"
It applies > redirection, expands $USER, and 
returns cleaned command like "echo uvadakku" for execution.
*/
char	*prepare_pipeline_segment(char *segment, t_env *env)
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
	expanded = expand_string(cleaned, env);
	free(cleaned);
	return (expanded);
}

void	exec_or_fail(char **args, t_env **envp)
{
	char	*cmd_path;
	char	**env_array;

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

static t_cmd	*build_pipeline_cmd_or_exit(char *segment, t_env *env)
{
	t_token	*tokens;
	t_cmd	*commands;

	tokens = tokenize(segment);
	if (!tokens)
		exit(1);
	if (expand_token_list(tokens, env) < 0)
	{
		free_tokens(tokens);
		exit(1);
	}
	commands = build_commands(tokens);
	free_tokens(tokens);
	if (!commands)
		exit(1);
	return (commands);
}

void	execute_pipeline_command_or_exit(char *segment, t_env **envp)
{
	t_cmd	*commands;
	t_cmd	*cmd;
	int		exit_code;

	commands = build_pipeline_cmd_or_exit(segment, *envp);
	cmd = commands;
	if (!cmd->args || !cmd->args[0])
	{
		free_cmd_list(commands);
		exit(0);
	}
	if (setup_pipeline_cmd_io(cmd) == -1)
	{
		free_cmd_list(commands);
		exit(1);
	}
	exit_code = execute_command(cmd->args, envp);
	free_cmd_list(commands);
	exit(exit_code);
}
