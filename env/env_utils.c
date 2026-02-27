/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:19:16 by spaipur-          #+#    #+#             */
/*   Updated: 2026/02/27 11:25:22 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_env_node(t_env **env_list, char *env_str)
{
	t_env *new_node;
	char *equal_sign;

	new_node = malloc(sizeof(t_env));

	equal_sign = strchr(env_str, '=');
	
	if (equal_sign)
	{
		size_t key_len = equal_sign - env_str;
		new_node->key = strndup(env_str, key_len);
		new_node->value = strdup(equal_sign + 1);
	}
	else
	{
		new_node->key = strdup(env_str);
		new_node->value = NULL;
	}

	new_node->next = *env_list;
	*env_list = new_node;
}

t_env *init_env(char **envp)
{
	t_env *env_list = NULL;
	int i = 0;

	while (envp[i])
	{
		add_env(&env_list, envp[i]);
		i++;
	}
	return (env_list);
}