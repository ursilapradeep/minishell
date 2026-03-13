/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 14:17:23 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 09:13:37 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*split each built-ins into different files and additional code if any implementation is missing*/
#include "../minishell.h"
#include <unistd.h>

// Execute cd builtin
int builtin_cd(char **args, t_env *env)
{
	char	*target_dir;
	char	*old_pwd;
	char	cwd[4096];

	// Check for too many arguments
	if (args[1] && args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}  // Get target directory
	if (!args[1]) //// cd with no args: go to HOME
	{
		target_dir = get_env_value(env, "HOME");
		if (!target_dir)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		// cd - : go to OLDPWD
		target_dir = get_env_value(env, "OLDPWD");
		if (!target_dir)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			return (1);
		}
	}
	else
	{
		target_dir = args[1];
	}

	// Get current PWD before changing
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd");
		return (1);
	}
	old_pwd = ft_strdup(cwd);

	// Change directory
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}

	// Update OLDPWD
	set_env_value(&env, "OLDPWD", old_pwd);
	free(old_pwd);

	// Update PWD
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_value(&env, "PWD", cwd);

	return (0);
}

// Execute pwd builtin
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

// Execute export builtin
int	builtin_export(char **args, t_env **env)
{
	int		i;
	char	*equal_sign;
	char	*key;
	char	*value;

	if (!args[1])
	{
		// export with no args: print all env variables (sorted, but we'll skip sorting for now)
		t_env *temp = *env;
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
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			// export VAR=value
			key = ft_substr(args[i], 0, equal_sign - args[i]);
			value = ft_strdup(equal_sign + 1);
			if (!key || !value)
			{
				free(key);
				free(value);
				return (1);
			}
			set_env_value(env, key, value);
			free(key);
			free(value);
		}
		else
		{
			// export VAR (just mark/ensure it exists)
			if (!get_env_value(*env, args[i]))
				set_env_value(env, args[i], NULL);
		}
		i++;
	}
	return (0);
}

// Execute unset builtin
int	builtin_unset(char **args, t_env **env)
{
	int		i;
	t_env	*current;

	if (!args[1])
		return (0);

	i = 1;
	while (args[i])
	{
		current = *env;
		while (current)
		{
			if (current->key && ft_strncmp(current->key, args[i], 
				ft_strlen(args[i]) + 1) == 0)
			{
				if (current->prev)
					current->prev->next = current->next;
				else
					*env = current->next;
				if (current->next)
					current->next->prev = current->prev;
				free(current->key);
				free(current->value);
				free(current);
				break ;
			}
			current = current->next;
		}
		i++;
	}
	return (0);
}

// Execute exit builtin
int	builtin_exit(char **args)
{
	int	exit_code;

	if (!args[1])
		exit(0);
	exit_code = ft_atoi(args[1]);
	exit(exit_code);
	return (-1);
}

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
		return (builtin_cd(args, *my_env));
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
