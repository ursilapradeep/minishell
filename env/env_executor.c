/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 12:32:05 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/18 14:59:37 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" // Include minishell types, env structs, and helper prototypes.

static int	count_env_entries(t_env *env) // Count only env nodes that have both key and value.
{
	int	count; // Number of valid KEY=VALUE entries.

	count = 0; 
	while (env) // Traverse linked list from head to tail.
	{
		if (env->key && env->value) /* code is counting valid nodes in env linked list*/
			count++; 
		env = env->next; // Move to next node.
	}
	return (count); // Return total valid entries.
}

/*its freeing an array of pointers, not linked list*/
void	free_partial_env_array(char **env_array, int count) // Free already-built env strings after partial failure.
{
	while (count > 0) // Free in reverse order from last valid index. 
	{
		count--; // Point to previous allocated element.
		free(env_array[count]); // Release one KEY=VALUE string.
	}
	free(env_array); // Release the top-level pointer array.
}

// Helper: join key and value as KEY=VALUE
static char *make_env_entry(const char *key, const char *value) // Build one KEY=VALUE string for execve environment.
{
	char *tmp; // Temporary string for "KEY=".
	char *result; // Final concatenated "KEY=VALUE".

	tmp = ft_strjoin(key, "="); // Create prefix with equals sign.
	if (!tmp) 
		return (NULL); 
	result = ft_strjoin(tmp, value); // Append value to complete env entry.
	free(tmp); // Free temporary prefix.
	return (result); // Return newly allocated KEY=VALUE string.
}

	/* USER -> alice 	HOME -> /Users/alice
	then env_array becomes:
	
	env_array[0] = "USER=alice"
	env_array[1] = "HOME=/Users/alice"
	env_array[2] = NULL */
char	**build_env_array(t_env *env) // Convert env linked list into NULL-terminated char** for execve.
{
	char	**env_array; //built from linked list passed to execve
	int		index; // Next write position in env_array. 

	env_array = malloc(sizeof(char *) * (count_env_entries(env) + 1)); // Allocate slots + 1 for final NULL.
	if (!env_array)
		return (NULL); 
	index = 0; // Start writing at first array position.
	while (env) // Walk through linked-list env nodes.
	{
		if (env->key && env->value) // Export only complete key/value pairs.
		{
			env_array[index] = make_env_entry(env->key, env->value); // Build "KEY=VALUE" element.
			if (!env_array[index]) // Handle string allocation failure.
			{
				free_partial_env_array(env_array, index); // Free everything created so far.
				return (NULL); 
			}
			index++; // Advance to next output slot.
		}
		env = env->next; // pointer field inside the current node
	}
	env_array[index] = NULL; // Terminate array for execve contract.
	return (env_array); // Return ready-to-use environment array.
}
