/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 14:17:23 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 17:24:28 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_dot(char **args)
{
	if (!args[1])
	{
		ft_putstr_fd(".: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		return (2);
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (1);
	if (cmd[0] == '.' && cmd[1] == '\0')
		return (1);
	return (0);
}

int	execute_builtin(char **args, t_env **my_env)
{
	if (!args || !args[0])
		return (0);
	if (ft_strncmp(args[0], "echo", 5) == 0 && args[0][4] == '\0')
		return (builtin_echo(args));
	if (ft_strncmp(args[0], "cd", 3) == 0 && args[0][2] == '\0')
		return (builtin_cd(args, my_env));
	if (ft_strncmp(args[0], "pwd", 4) == 0 && args[0][3] == '\0')
		return (builtin_pwd(args));
	if (ft_strncmp(args[0], "export", 7) == 0 && args[0][6] == '\0')
		return (builtin_export(args, my_env));
	if (ft_strncmp(args[0], "unset", 6) == 0 && args[0][5] == '\0')
		return (builtin_unset(args, my_env));
	if (ft_strncmp(args[0], "env", 4) == 0 && args[0][3] == '\0')
	{
		if (args[1])
			return (0);
		return (builtin_env(args, *my_env));
	}
	if (ft_strncmp(args[0], "exit", 5) == 0 && args[0][4] == '\0')
		return (builtin_exit(args));
	if (args[0][0] == '.' && args[0][1] == '\0')
		return (builtin_dot(args));
	return (0);
}
