/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_cmds.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:12 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/12 17:24:28 by uvadakku         ###   ########.fr       */
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
	set_env_value(my_env, "_", cmd_path);
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
	int	is_env_with_args;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	is_env_with_args = (ft_strncmp(cmd->args[0], "env", 4) == 0
			&& cmd->args[0][3] == '\0' && cmd->args[1]);
	if (is_builtin(cmd->args[0]) && !is_env_with_args)
		exit(execute_builtin(cmd->args, my_env));
	execute_external_command(cmd, my_env);
}
