/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_executable_c.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 15:28:28 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 12:41:07 by uvadakku         ###   ########.fr       */
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
	expanded = expand_variables(cleaned, env);
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

void	execute_pipeline_command_or_exit(char *segment, t_env **envp)
{
	char	**args;
	char	*prepared;

	prepared = prepare_pipeline_segment(segment, *envp);
	if (!prepared)
		exit(1);
	args = split_args(prepared);
	free(prepared);
	if (!args)
		exit(127);
	exec_or_fail(args, envp);
}
