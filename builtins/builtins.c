/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 14:17:23 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/01 11:03:20 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(t_cmd *cmd, t_env **my_env)
{
	char **args;

	args = cmd->args;
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
		return (builtin_env(args, *my_env));
	if (ft_strncmp(args[0], "exit", 5) == 0 && args[0][4] == '\0')
	{
		return (builtin_exit(args));
	}
	return (0);
}
