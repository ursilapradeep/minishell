#include "../minishell.h"
// Find command in PATH
extern char **environ;

static char	*search_directories(char *path_copy, char *cmd)
{
	char	*dir_start;
	char	*result;
	int		i;

	i = 0;
	dir_start = path_copy; // Start of current directory in PATH
	while (path_copy[i])
	{
		if (path_copy[i] == ':') // End of a directory in PATH
		{
			result = process_directory(path_copy, &dir_start, i, cmd); // Check if cmd exists in this directory
			if (result)
				return (result);
		}
		i++;
	}
	return (check_command_in_dir(dir_start, cmd)); // Check last directory in PATH after loop
}

static char	*search_in_path(char *path_env, char *cmd)
{
	char	*path_copy;
	char	*result;

	path_copy = ft_strdup(path_env); // Duplicate PATH to avoid modifying original environment variable
	if (!path_copy)
		return (NULL);
	result = search_directories(path_copy, cmd);
	free(path_copy);
	return (result);
}

char *find_command(char *cmd)
{
    char *path_env;

    if (access(cmd, X_OK) == 0) // Check if cmd is directly executable
        return (ft_strdup(cmd));
    path_env = getenv("PATH"); //When you type ls, the shell checks: /usr/local/bin/ls (not found)
    if (!path_env)
        return (NULL);
    return (search_in_path(path_env, cmd));
}


