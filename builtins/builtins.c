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
	}

	// Get target directory
	if (!args[1])
	{
		// cd with no args: go to HOME
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
	{
		// TODO: implement echo builtin
		printf("echo: builtin not yet implemented\n");
		return (1);
	}
	if (ft_strncmp(args[0], "cd", 3) == 0 && args[0][2] == '\0')
		return (builtin_cd(args, *my_env));
	if (ft_strncmp(args[0], "pwd", 4) == 0 && args[0][3] == '\0')
	{
		// TODO: implement pwd builtin
		printf("pwd: builtin not yet implemented\n");
		return (1);
	}
	if (ft_strncmp(args[0], "export", 7) == 0 && args[0][6] == '\0')
	{
		// TODO: implement export builtin
		printf("export: builtin not yet implemented\n");
		return (1);
	}
	if (ft_strncmp(args[0], "unset", 6) == 0 && args[0][5] == '\0')
	{
		// TODO: implement unset builtin
		printf("unset: builtin not yet implemented\n");
		return (1);
	}
	if (ft_strncmp(args[0], "env", 4) == 0 && args[0][3] == '\0')
	{
		// TODO: implement env builtin
		printf("env: builtin not yet implemented\n");
		return (1);
	}
	if (ft_strncmp(args[0], "exit", 5) == 0 && args[0][4] == '\0')
	{
		// TODO: implement exit builtin
		printf("exit: builtin not yet implemented\n");
		return (1);
	}
	return (0);
}
