#include "../minishell.h"

static char	*build_full_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}
char	*process_directory(char *path_copy, char **dir_start, int i, char *cmd)
{
	char	*dir;
	char	*full_path;
	int		len;

	len = &path_copy[i] - *dir_start; // Calculate length of current directory in PATH
	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	ft_strlcpy(dir, *dir_start, len + 1); // Copy directory from PATH into dir variable
	full_path = build_full_path(dir, cmd);
	free(dir);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
	{
		*dir_start = &path_copy[i + 1]; // Move dir_start to the next directory in PATH for the next iteration
		return (full_path);
	}
	free(full_path);
	*dir_start = &path_copy[i + 1];
	return (NULL);
}

char	*check_command_in_dir(char *dir, char *cmd)
{
	char	*full_path;

	full_path = build_full_path(dir, cmd);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}
