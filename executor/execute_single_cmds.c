/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_cmds.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:12 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/11 23:46:53 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_array(char **env_array)
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

static void	execute_external_command(t_cmd *cmd, t_env **my_env)
{
	char	*cmd_path;
	char	**env_array;

	cmd_path = find_command(cmd->args[0], my_env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(127);
	}
	env_array = build_env_array(*my_env);
	if (!env_array)
	{
		free(cmd_path);
		perror("malloc");
		exit(1);
	}
	execve(cmd_path, cmd->args, env_array);
	perror("execve");
	free_env_array(env_array);
	free(cmd_path);
	exit(127);
}

void	execute_ast_command_child(t_cmd *cmd, t_env **my_env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd->args, my_env));
	execute_external_command(cmd, my_env);
}
