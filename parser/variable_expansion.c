/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 16:18:05 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

/**
 * get_env_variable - Get variable value from environment
 * @env: Environment list
 * @var_name: Variable name
 * @len: Length of variable name
 * Return: Variable value or NULL if not found
 */
static char *get_env_variable(t_env *env, const char *var_name, int len)
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

// Helper function to allocate and copy expanded variable value
static int allocate_expanded_value(const char *value, int value_len, char **expanded)
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

// Helper function to handle special cases for variable expansion
static int handle_special_cases(const char *input, int var_len, int is_braced, char **expanded, int *consumed)
{
    (void)input; // Marked unused parameter
    if (var_len == 0 && !is_braced) // Handle $$ (process ID)
    {
        *expanded = ft_calloc(2, sizeof(char));
        if (!*expanded)
            return (-1);
        (*expanded)[0] = '$';
        (*expanded)[1] = '\0';
        *consumed = 1;
        return (1);
    }
    if (var_len == 0) // Handle empty variable name
    {
        *expanded = ft_calloc(1, sizeof(char));
        if (!*expanded)
            return (-1);
        (*expanded)[0] = '\0';
        *consumed = (is_braced) ? 3 : 1;
        return (1);
    }
    return (0);
}

/**
 * expand_variable - Expand a single variable reference
 * @input: String starting with $ 
 * @env: Environment list
 * @expanded: Pointer to store expanded value
 * @consumed: Pointer to store characters consumed
 * Return: 0 on success, -1 on error
 */
int expand_variable(const char *input, t_env *env, 
	char **expanded, int *consumed)
{
	const char	*after_var;
	const char	*var_name;
	int			var_len;
	int			is_braced;
	char		*value;

	if (!input || input[0] != '$' || !expanded || !consumed)
		return (-1);
	var_name = input + 1;
	after_var = extract_var_name(var_name, &var_len, &is_braced);
	if (!after_var)
		return (-1);
	int special_case = handle_special_cases(input, var_len, is_braced, expanded, consumed);
	if (special_case != 0)
		return (special_case == 1 ? 0 : -1);

	const char *actual_var_name = (is_braced) ? var_name + 1 : var_name;
	value = get_env_variable(env, actual_var_name, var_len);
	if (allocate_expanded_value(value, value ? ft_strlen(value) : 0, expanded) == -1)
		return (-1);
	*consumed = (after_var - input);
	return (0);
}

