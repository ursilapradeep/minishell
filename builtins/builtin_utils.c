#include "../minishell.h" // Include minishell types and function prototypes.

// Get environment variable value by key
char *get_env_value(t_env *env, char *key) // Search linked-list environment for a matching key.
{
	t_env *current; // Iterator for traversing environment nodes.

	if (!env || !key) // Validate input pointers before traversal.
		return (NULL); // Return NULL when env list or key is missing.
	current = env; // Start scan from head node.
	while (current) // Walk through all env nodes.
	{
		if (current->key && ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0) // Compare full key including terminator.
			return (current->value); // Return associated value on exact match.
		current = current->next; // Move to next env node.
	}
	return (NULL); // Key not found.
}

// Update or add environment variable
void set_env_value(t_env **env, char *key, char *value) // Replace existing value or create a new variable.
{
	t_env *current; // Iterator used to find existing key.
	char	*entry;
	char	*tmp;

	if (!env || !key) // Guard against invalid arguments.
		return ; // Do nothing on invalid input.
	current = *env;
	while (current) 
	{
		if (current->key && ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0) // Exact key match found.
		{
			if (!value)
				return ;
			free(current->value); // Free old heap-allocated value string.
			current->value = ft_strdup(value); // Duplicate and store new value.
			return ; // Update complete.
		}
		current = current->next; // Continue searching remaining nodes.
	}  // If not found, add new env variable
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
