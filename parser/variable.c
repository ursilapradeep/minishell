/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:18:36 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 16:20:10 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/**
 * is_valid_var_char - Check if character is valid in variable name
 * @c: Character to check
 * Return: 1 if valid, 0 otherwise
 */
static int is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// Helper function to handle braced variable names
static const char *handle_braced_var(const char *input, int *len)
{
	const char *start = input + 1;
	const char *end = start;

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
		return (handle_braced_var(input, len));
	}
	start = input;
	end = start;
	while (*end && is_valid_var_char(*end))
		end++;
	*len = end - start;
	return (end);
}
