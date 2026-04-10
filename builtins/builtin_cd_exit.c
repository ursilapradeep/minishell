/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:02:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/10 21:52:05 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_exit(char **args)
{
	int	exit_code;
	int	i;

	if (!args[1])
		exit(0);
	// Check if the first argument is numeric FIRST (before checking arg count)
	i = 0;
	if (args[1][i] == '+' || args[1][i] == '-')
		i++;
	if (args[1][i] == '\0')
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		i++;
	}
	// NOW check if there are too many arguments (after validating the first one)
	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	exit(exit_code);
	return (-1);
}

static char	*get_cd_env_target(t_env *env, char *key,
	char *err_msg, int *status)
{
	char	*target_dir;

	target_dir = get_env_value(env, key);
	if (!target_dir)
	{
		ft_putstr_fd(err_msg, 2);
		*status = 1;
		return (NULL);
	}
	return (target_dir);
}

static char	*resolve_cd_target(char **args, t_env *env, int *status)
{
	char	*target_dir;

	*status = 0;
	if (args[1] && args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		*status = 1;
		return (NULL);
	}
	if (!args[1])
		return (get_cd_env_target(env, "HOME", "cd: HOME not set\n", status));
	if (ft_strncmp(args[1], "-", 2) == 0)
		return (get_cd_env_target(env, "OLDPWD",
				"cd: OLDPWD not set\n", status));
	target_dir = args[1];
	return (target_dir);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*target_dir;
	char	*old_pwd;
	char	cwd[4096];
	int		status;

	target_dir = resolve_cd_target(args, *env, &status);
	if (status != 0)
		return (status);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd");
		return (1);
	}
	old_pwd = ft_strdup(cwd);
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	set_env_value(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_value(env, "PWD", cwd);
	return (0);
}
