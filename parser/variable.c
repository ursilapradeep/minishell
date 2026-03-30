/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:18:36 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/30 18:20:19 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * is_valid_var_char - Check if character is valid in variable name
 * @c: Character to check
 * Return: 1 if valid, 0 otherwise
 */
static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

const char	*handle_braced_var(const char *input, int *len)
{
	const char	*start;
	const char	*end;

	start = input + 1;
	end = start;
	while (*end && is_valid_var_char(*end))
    end++;
		end++;
	if (!*end)
	{
		write(STDERR_FILENO, "Error: Unclosed brace in variable\n", 35);
		return (NULL);
	}
	*len = end - start;
	return (end + 1);
}

/**
 * extract_var_name - Extract variable name after $ or ${}
 * @input: String starting after $
 * @len: Pointer to store length of variable name
 * @is_braced: 1 if ${VAR} format, 0 if $VAR format
 * Return: Pointer after variable name
 */
const char	*extract_var_name(const char *input, int *len, int *is_braced)
{
	const char	*start;
	const char	*end;

	if (!input || !len)
		return (NULL);
	*is_braced = 0;
	*len = 0;
	if (*input == '{')
	{
		*is_braced = 1;
		return (handle_braced_var(input, len));
	}
	start = input;
	end = start;
	while (*end && is_valid_var_char(*end))
		end++;
	*len = end - start;
	return (end);
}

/**
 * expand_variable - Expand a single variable reference
 * @input: String starting with $ 
 * @env: Environment list
 * @expanded: Pointer to store expanded value
 * @consumed: Pointer to store characters consumed
 * Return: 0 on success, -1 on error
 */

static int	expand_variable_core(t_var_result *result,
	const char *after_var, t_env *env)
{
	result->expanded = expand_and_allocate_value(env, result->var_name,
			result->var_len, result->is_braced);
	if (!result->expanded)
		return (-1);
	result->consumed = (after_var - (result->var_name - 1));
	return (0);
}

int	expand_variable(const char *input, t_env *env, t_var_result *result)
{
	const char	*after_var;
	const char	*var_name;
	int			var_len;
	int			is_braced;
	int			special_case;

	if (!input || input[0] != '$' || !result)
		return (-1);
	var_name = input + 1;
	after_var = extract_var_name(var_name, &var_len, &is_braced);
	if (!after_var)
		return (-1);
	special_case = handle_special_cases(input, var_len,
			is_braced, result);
	if (special_case != 0)
	{
		if (special_case == 1)
			return (0);
		else
			return (-1);
	}
	result->var_name = var_name;
	result->var_len = var_len;
	result->is_braced = is_braced;
	return (expand_variable_core(result, after_var, env));
}
