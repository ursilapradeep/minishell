/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 12:54:24 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/07 18:13:09 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * expand_variable_helper - Helper function to expand variables
 * @current: Pointer to the current character in the input string
 * @result: The result string being built
 * @result_len: Pointer to the length of the result string
 * @env: Environment list
 * Return: 1 on successful variable expansion, 0 on failure
 */
int	expand_variable_helper(const char **current, char *result,
	int *result_len, t_env *env)
{
	char	*var_value;
	char	*temp;
	int		consumed;

	var_value = NULL;
	consumed = 0;
	if (expand_variable(*current, env, &var_value, &consumed) == 0)
	{
		if (var_value)
		{
			temp = var_value;
			while (*temp && *result_len < 4095)
			{
				result[(*result_len)++] = *temp;
				temp++;
			}
			free(var_value);
		}
		*current += consumed;
		return (1);
	}
	return (0);
}

int	proc_input(const char *input, char *result, int *result_len, t_env *env)
{
	const char	*current;
	int			in_sq;
	int			in_dq;

	current = input;
	in_sq = 0;
	in_dq = 0;
	while (current && *current && *result_len < 4095)
	{
		if (*current == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (*current == '"' && !in_sq)
			in_dq = !in_dq;
		if (*current == '$' && !in_sq)
		{
			if (expand_variable_helper(&current, result, result_len, env))
				continue ;
			else
				return (-1);
		}
		result[(*result_len)++] = *current;
		current++;
	}
	return (0);
}

/**
 * expand_string - Expand all variables in a string
 * @input: Input string with potential variables
 * @env: Environment list
 * Return: Newly allocated expanded string, NULL on error
 */
char	*expand_string(const char *input, t_env *env)
{
	char	*result;
	int		result_len;

	if (!input || !env)
		return (ft_strdup(input));
	result = ft_calloc(4096, sizeof(char));
	if (!result)
		return (NULL);
	result_len = 0;
	if (proc_input(input, result, &result_len, env) == -1)
	{
		free(result);
		return (NULL);
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
int	expand_token_list(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded;
	char	*unquoted;

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
			unquoted = remove_quotes_string(expanded);
			free(expanded);
			free(current->value);
			current->value = unquoted;
			if (!unquoted)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
