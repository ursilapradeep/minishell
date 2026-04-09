/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:12 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/09 12:26:36 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*search_directories(char *path_copy, char *cmd)
{
	char	*dir_start;
	char	*result;
	int		i;

	i = 0;
	dir_start = path_copy;
	while (path_copy[i])
	{
		if (path_copy[i] == ':')
		{
			result = process_directory(path_copy, &dir_start, i, cmd);
			if (result)
				return (result);
		}
		i++;
	}
	return (check_command_in_dir(dir_start, cmd));
}

static char	*search_in_path(char *path_env, char *cmd)
{
	char	*path_copy;
	char	*result;

	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	result = search_directories(path_copy, cmd);
	free(path_copy);
	return (result);
}

/*envp[0] = "USER=me"
envp[1] = "PATH=/usr/local/bin:/usr/bin:/bin"
envp[2] = "HOME=/home/me" //get_path_from_env(envp) scans each entry, 
finds the one starting with "PATH=", and returns the substring after it:*/
char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*find_command(char *cmd, t_env **envp)
{
	char	*path_env;

	if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path_env = get_env_value(*envp, "PATH");
	if (!path_env)
		return (NULL);
	return (search_in_path(path_env, cmd));
}
