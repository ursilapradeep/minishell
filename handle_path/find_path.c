/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:12 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/11 21:40:08 by spaipur-         ###   ########.fr       */
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

char	*find_command(char *cmd, t_env **envp)
{
	char	*path_env;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value(*envp, "PATH");
	if (!path_env)
		return (NULL);
	return (search_in_path(path_env, cmd));
}
