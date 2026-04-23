/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:02:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 15:29:39 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		*status = 1;
		return (NULL);
	}
	if (!args[1] || ft_strncmp(args[1], "--", 3) == 0)
		return (get_cd_env_target(env, "HOME",
				"minishell: cd: HOME not set\n", status));
	if (ft_strncmp(args[1], "-", 2) == 0)
		return (get_cd_env_target(env, "OLDPWD",
				"minishell: cd: OLDPWD not set\n", status));
	target_dir = args[1];
	return (target_dir);
}

static void	update_pwd_and_maybe_print(t_env **env,
		char cwd[4096], int print_new_pwd)
{
	if (getcwd(cwd, 4096) != NULL)
	{
		set_env_value(env, "PWD", cwd);
		if (print_new_pwd)
		{
			ft_putstr_fd(cwd, 1);
			ft_putstr_fd("\n", 1);
		}
	}
}

static int	change_dir_and_set_oldpwd(t_env **env,
		char *target_dir, char cwd[4096])
{
	char	*old_pwd;

	if (getcwd(cwd, 4096) == NULL)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	old_pwd = ft_strdup(cwd);
	if (chdir(target_dir) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(old_pwd);
		return (1);
	}
	set_env_value(env, "OLDPWD", old_pwd);
	free(old_pwd);
	return (0);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*target_dir;
	char	cwd[4096];
	int		status;
	int		print_new_pwd;

	print_new_pwd = (args[1] && ft_strncmp(args[1], "-", 2) == 0);
	target_dir = resolve_cd_target(args, *env, &status);
	if (status != 0)
		return (status);
	if (change_dir_and_set_oldpwd(env, target_dir, cwd) != 0)
		return (1);
	update_pwd_and_maybe_print(env, cwd, print_new_pwd);
	return (0);
}
