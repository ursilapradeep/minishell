#include "../minishell.h"

// Get environment variable value by key
char *get_env_value(t_env *env, char *key)
{
	t_env *current;

	if (!env || !key)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->key && ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

// Update or add environment variable
void set_env_value(t_env **env, char *key, char *value)
{
	t_env *current;

	if (!env || !key || !value)
		return ;
	current = *env;
	while (current)
	{
		if (current->key && ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	// If not found, add new env variable
	add_env_node(env, ft_strjoin(key, "="));
}
