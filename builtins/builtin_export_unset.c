/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_unset.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:18:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/09 12:08:29 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	export_one_arg(char *arg, t_env **env)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
		if (!key || !value)
			return (free(key), free(value), 1);
		set_env_value(env, key, value);
		free(key);
		free(value);
	}
	else if (!get_env_value(*env, arg))
		set_env_value(env, arg, NULL);
	return (0);
}

int	builtin_export(char **args, t_env **env)
{
	int		i;
	t_env	*temp;

	if (!args[1])
	{
		temp = *env;
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
		if (export_one_arg(args[i], env) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	unset_one_key(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (current->key && ft_strncmp(current->key, key,
				ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_env **env)
{
	int		i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		unset_one_key(env, args[i]);
		i++;
	}
	return (0);
}
