/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:19:16 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/16 11:43:45 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fill_env_node(t_env *new_node, char *env_str)
{
	char	*equal_sign;
	size_t	key_len;

	
	equal_sign = ft_strchr(env_str, '='); // Find '=' in env string (example: "PATH=/usr/bin")
	if (equal_sign) // Case 1: string has '=' -> split into key and value
	{
		key_len = equal_sign - env_str; // Length of key is distance from start to '='
		new_node->key = ft_substr(env_str, 0, key_len); // Copy key part (before '='), e.g. "PATH"
		new_node->value = ft_strdup(equal_sign + 1); // Copy value part (after '='), e.g. "/usr/bin"
	}
	else // Case 2: no '=' -> only key exists, value is NULL
	{
		new_node->key = ft_strdup(env_str); // Whole string is treated as key, e.g. "SHLVL"
		new_node->value = NULL; // No value available
	}
	// If key allocation failed, free value (if allocated) and report failure
	if (!new_node->key)
	{
		free(new_node->value);
		return (0);
	}
	return (1); // Success: key/value were filled correctly
}

void add_env_node(t_env **env_list, char *env_str)
{
	t_env *new_node;                  
	new_node = malloc(sizeof(t_env)); // Allocate memory for one new environment node
	if (!new_node)
		return ; // Stop if allocation fails
	if (!fill_env_node(new_node, env_str)) // Fill node fields (`key`, `value`) from "KEY=VALUE" string
	{
		free(new_node); // Cleanup on parse/fill failure
		return ;
	}
	/* Insert at head of linked list: new node points to current head */
	new_node->next = *env_list;
	*env_list = new_node; // update head to new node
}

/*here char **envp (the environment array from main, e,g. "PATH=...", "HOME=...") */
t_env *init_env(char **envp) 
{
	t_env *env_list; //head pointer to the environment linked list 
	int i = 0;

	env_list = NULL; //empty list at the start
	while (envp[i]) //till the current entry is not null
	{
		add_env_node(&env_list, envp[i]); //&env_lst fn can modify the head pointer , envp[i] is one string "USER=alice"
		i++;
	}
	return (env_list);
}
