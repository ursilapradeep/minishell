/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 14:17:23 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/18 20:13:35 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*split each built-ins into different files and additional code if any implementation is missing*/
#include "../minishell.h"

// Execute env builtin
int	builtin_env(char **args, t_env *env)
{
	t_env	*current;

	if (args[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (1);
	}
	current = env;
	while (current)
	{
		if (current->key && current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}

// Execute echo builtin
static int is_n_flag(char *arg)
{
	int i;

	i = 0;
	if (!arg || arg[0] != '-')
		return (0);
	i++;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
int builtin_echo(char **args)
{
	int i = 1;
	int new_line = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		new_line = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (new_line)
		ft_putstr_fd("\n", 1);
	return (0);
}

// Check if command is a builtin
int is_builtin(char *cmd)
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
	return (0);
}

// Execute builtin command (returns 1 if builtin was executed, 0 otherwise)
int execute_builtin(char **args, t_env **my_env)
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
		return (builtin_env(args, *my_env));
	if (ft_strncmp(args[0], "exit", 5) == 0 && args[0][4] == '\0')
	{
		return (builtin_exit(args));
	}
	return (0);
}
