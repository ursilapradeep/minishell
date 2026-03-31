/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:05:43 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/31 11:24:55 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	if (!env || !key)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->key
			&& ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static int	update_existing_env_value(t_env *env, char *key, char *value)
{
	while (env)
	{
		if (env->key && ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
		{
			if (!value)
				return (1);
			free(env->value);
			env->value = ft_strdup(value);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	set_env_value(t_env **env, char *key, char *value)
{
	char	*entry;
	char	*tmp;

	if (!env || !key)
		return ;
	if (update_existing_env_value(*env, key, value))
		return ;
	if (value)
	{
		tmp = ft_strjoin(key, "=");
		if (!tmp)
			return ;
		entry = ft_strjoin(tmp, value);
		free(tmp);
	}
	else
		entry = ft_strdup(key);
	if (!entry)
		return ;
	add_env_node(env, entry);
	free(entry);
}
