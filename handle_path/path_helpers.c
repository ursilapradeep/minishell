/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:16:23 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/12 17:24:28 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/stat.h>

static int	is_executable_file(char *path)
{
	struct stat	st;

	if (access(path, X_OK) != 0)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (0);
	return (1);
}

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

	len = &path_copy[i] - *dir_start;
	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	ft_strlcpy(dir, *dir_start, len + 1);
	full_path = build_full_path(dir, cmd);
	free(dir);
	if (!full_path)
		return (NULL);
	if (is_executable_file(full_path))
	{
		*dir_start = &path_copy[i + 1];
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
	if (is_executable_file(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}
