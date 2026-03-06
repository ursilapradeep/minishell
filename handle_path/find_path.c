#include "../minishell.h"
// Find command in PATH

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
/*envp[0] = "USER=me"
envp[1] = "PATH=/usr/local/bin:/usr/bin:/bin"
envp[2] = "HOME=/home/me" //get_path_from_env(envp) scans each entry, finds the one starting with "PATH=", and returns the substring after it:*/

char *get_path_from_env(char **envp)
{
	int i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char *find_command(char *cmd, char **envp)
{
	char *path_env;

	if (access(cmd, X_OK) == 0) // Check if cmd is executable
		return (ft_strdup(cmd));
	path_env = get_path_from_env(envp);
	if (!path_env)
		return (NULL); //path does not exist
	return (search_in_path(path_env, cmd));
}

