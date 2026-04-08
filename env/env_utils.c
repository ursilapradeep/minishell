/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:19:16 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/08 14:47:17 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
It looks for '=' in env_str.
If found, it splits into:
	key = part before '='
	value = part after '='
If not found, it stores the whole string as key and sets value = NULL.
If key allocation fails, it frees value (if allocated) and returns 0 (failure).
Otherwise it returns 1 (success).*/

int	fill_env_node(t_env *new_node, char *env_str)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(env_str, '=');
	if (equal_sign)
	{
		key_len = equal_sign - env_str;
		new_node->key = ft_substr(env_str, 0, key_len);
		new_node->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		new_node->key = ft_strdup(env_str);
		new_node->value = NULL;
	}
	if (!new_node->key)
	{
		free(new_node->value);
		return (0);
	}
	return (1);
}

void	add_env_node(t_env **env_list, char *env_str)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	if (!fill_env_node(new_node, env_str))
	{
		free(new_node);
		return ;
	}
	new_node->next = *env_list;
	new_node->prev = NULL;
	if (*env_list)
		(*env_list)->prev = new_node;
	*env_list = new_node;
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		add_env_node(&env_list, envp[i]);
		i++;
	}
	return (env_list);
}
