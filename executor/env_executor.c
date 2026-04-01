/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 12:32:05 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/23 17:08:47 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_env_entries(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->key && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

void	free_partial_env_array(char **env_array, int count)
{
	while (count > 0)
	{
		count--;
		free(env_array[count]);
	}
	free(env_array);
}

static char	*make_env_entry(const char *key, const char *value)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	return (result);
}

char	**build_env_array(t_env *env)
{
	char	**env_array;
	int		index;

	env_array = malloc(sizeof(char *) * (count_env_entries(env) + 1));
	if (!env_array)
		return (NULL);
	index = 0;
	while (env)
	{
		if (env->key && env->value)
		{
			env_array[index] = make_env_entry(env->key, env->value);
			if (!env_array[index])
			{
				free_partial_env_array(env_array, index);
				return (NULL);
			}
			index++;
		}
		env = env->next;
	}
	env_array[index] = NULL;
	return (env_array);
}
