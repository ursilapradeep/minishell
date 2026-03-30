/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/30 18:27:41 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	allocate_expanded_value(const char *value, int value_len, char **expanded)
{
	if (value)
	{
		*expanded = ft_calloc(value_len + 1, sizeof(char));
		if (!*expanded)
			return (-1);
		ft_strlcpy(*expanded, value, value_len + 1);
	}
	else
	{
		*expanded = ft_calloc(1, sizeof(char));
		if (!*expanded)
			return (-1);
		(*expanded)[0] = '\0';
	}
	return (0);
}

/**
 * get_env_variable - Get variable value from environment
 * @env: Environment list
 * @var_name: Variable name
 * @len: Length of variable name
 * Return: Variable value or NULL if not found
 */
char	*get_env_variable(t_env *env, const char *var_name, int len)
{
	t_env	*current;

	if (!var_name || len <= 0 || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->key && (int)ft_strlen(current->key) == len
			&& ft_strncmp(current->key, var_name, len) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

char *expand_and_allocate_value(t_env *env, const char *var_name,
	int var_len, int is_braced)
{
	const char	*actual_var_name;
	char		*value;
	int			value_len;
	char		*expanded;

	expanded = NULL;
	if (is_braced)
		actual_var_name = var_name + 1;
	else
		actual_var_name = var_name;
	value = get_env_variable(env, actual_var_name, var_len);
	if (value)
		value_len = ft_strlen(value);
	else
		value_len = 0;
	if (allocate_expanded_value(value, value_len, &expanded) == -1)
		return (NULL);
	return (expanded);
}
