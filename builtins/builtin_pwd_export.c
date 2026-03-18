/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:10:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/18 19:59:52 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(char **args)
{
	char	cwd[4096];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	return (0);
}

static int	export_one_arg(char *arg, t_env **env)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
		if (!key || !value)
			return (free(key), free(value), 1);
		set_env_value(env, key, value);
		free(key);
		free(value);
	}
	else if (!get_env_value(*env, arg))
		set_env_value(env, arg, NULL);
	return (0);
}

int	builtin_export(char **args, t_env **env)
{
	int		i;
	t_env	*temp;

	if (!args[1])
	{
		temp = *env;
		while (temp)
		{
			if (temp->value)
				printf("declare -x %s=\"%s\"\n", temp->key, temp->value);
			else
				printf("declare -x %s\n", temp->key);
			temp = temp->next;
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (export_one_arg(args[i], env) != 0)
			return (1);
		i++;
	}
	return (0);
}
