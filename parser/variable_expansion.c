/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 13:38:36 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

/**
 * is_valid_var_char - Check if character is valid in variable name
 * @c: Character to check
 * Return: 1 if valid, 0 otherwise
 */
static int is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * extract_var_name - Extract variable name after $ or ${}
 * @input: String starting after $
 * @len: Pointer to store length of variable name
 * @is_braced: 1 if ${VAR} format, 0 if $VAR format
 * Return: Pointer after variable name
 */
static const char *extract_var_name(const char *input, int *len, int *is_braced)
{
	const char *start;
	const char *end;

	if (!input || !len)
		return (NULL);
	*is_braced = 0;
	*len = 0;
	if (*input == '{')
	{
		*is_braced = 1;
		start = input + 1;
		end = start;
		while (*end && *end != '}')
			end++;
		if (!*end)
		{
			write(STDERR_FILENO, "Error: Unclosed brace in variable\n", 35);
			return (NULL);
		}
		*len = end - start;
		return (end + 1);
	}
	else
	{
		start = input;
		end = start;
		while (*end && is_valid_var_char(*end))
			end++;
		*len = end - start;
		return (end);
	}
}

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
		if (current->key && ft_strlen(current->key) == len
			&& ft_strncmp(current->key, var_name, len) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

/**
 * expand_variable - Expand a single variable reference
 * @input: String starting with $ 
 * @env: Environment list
 * @expanded: Pointer to store expanded value
 * @consumed: Pointer to store characters consumed
 * Return: 0 on success, -1 on error
 */
static int expand_variable(const char *input, t_env *env, 
	char **expanded, int *consumed)
{
	const char	*after_var;
	const char	*var_name;
	int			var_len;
	int			is_braced;
	char		*value;
	int			value_len;

	if (!input || input[0] != '$' || !expanded || !consumed)
		return (-1);
	
	var_name = input + 1;
	after_var = extract_var_name(var_name, &var_len, &is_braced);
	if (!after_var)
		return (-1);
	
	/* Handle $$ (process ID) - placeholder for now */
	if (var_len == 0 && !is_braced)
	{
		*expanded = ft_calloc(2, sizeof(char));
		if (!*expanded)
			return (-1);
		(*expanded)[0] = '$';
		(*expanded)[1] = '\0';
		*consumed = 1;
		return (0);
	}
	
	if (var_len == 0)
	{
		*expanded = ft_calloc(1, sizeof(char));
		if (!*expanded)
			return (-1);
		(*expanded)[0] = '\0';
		*consumed = (is_braced) ? 3 : 1;
		return (0);
	}
	
	const char *actual_var_name = (is_braced) ? var_name + 1 : var_name;
	value = get_env_variable(env, actual_var_name, var_len);
	if (value)
	{
		value_len = ft_strlen(value);
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
	
	*consumed = (after_var - input);
	return (0);
}

/**
 * expand_string - Expand all variables in a string
 * @input: Input string with potential variables
 * @env: Environment list
 * Return: Newly allocated expanded string, NULL on error
 *
 * Variables are expanded only outside single quotes.
 * Inside double quotes, variables are expanded.
 * Unset variables expand to empty string.
 */
char *expand_string(const char *input, t_env *env)
{
	char		*result;
	char		*temp;
	int			result_len;
	const char	*current;
	int			in_single_quote;
	int			in_double_quote;
	char		*var_value;
	int			consumed;

	if (!input || !env)
		return (ft_strdup(input));
	
	result = ft_calloc(4096, sizeof(char));
	if (!result)
		return (NULL);
	
	result_len = 0;
	current = input;
	in_single_quote = 0;
	in_double_quote = 0;
	
	while (current && *current && result_len < 4095)
	{
		/* Handle quotes */
		if (*current == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			current++;
			continue ;
		}
		if (*current == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			current++;
			continue ;
		}
		
		/* Expand variables outside single quotes */
		if (*current == '$' && !in_single_quote)
		{
			var_value = NULL;
			consumed = 0;
			if (expand_variable(current, env, &var_value, &consumed) == 0)
			{
				if (var_value)
				{
					temp = var_value;
					while (*temp && result_len < 4095)
					{
						result[result_len++] = *temp;
						temp++;
					}
					free(var_value);
				}
				current += consumed;
				continue ;
			}
			else
			{
				free(result);
				return (NULL);
			}
		}
		
		/* Regular character */
		result[result_len++] = *current;
		current++;
	}
	
	result[result_len] = '\0';
	return (result);
}

/**
 * expand_token_list - Expand variables in all tokens
 * @tokens: Token list to expand
 * @env: Environment list
 * Return: 0 on success, -1 on error
 */
int expand_token_list(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded;

	if (!tokens || !env)
		return (0);
	
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_string(current->value, env);
			if (!expanded)
				return (-1);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
	
	return (0);
}
