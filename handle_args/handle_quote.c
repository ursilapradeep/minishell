/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 16:10:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/18 15:12:00 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" // Include minishell headers, env accessors, and libft helpers.

int	is_var_char(char c) // Check if character is valid in an environment variable name.
{
	return (ft_isalnum(c) || c == '_'); // Allow letters/digits and underscore.
}

int	expanded_len(char *input, t_env *env) // Compute required length of expanded string before allocation.
{
	int		i; // Read index over input.
	int		len; // Total output length after expansion.
	char	quote; // Active quote context ('\0', '\'', or '"').

	i = 0; // Start from first character.
	len = 0; // Initialize output length counter.
	quote = '\0'; // Start outside quotes.
	while (input[i]) // Walk through input to count expansion result length.
	{
		if (!count_token_len(input, &i, &quote, env, &len)) // Delegate per-token length accounting.
			return (-1); // Propagate error (e.g., allocation failure inside helpers).
	}
	return (len); // Return exact allocation size needed (without null terminator).
}

int	append_var_value(char *expanded, int *j, char *input,
		int *i, t_env *env) // Expand $VAR at input[*i] and append its value into expanded buffer.
{
	int		key_len; // Length of variable key name after '$'.
	char	*key; // Extracted variable key (heap-allocated).
	char	*value; // Value looked up in environment.

	key_len = 0; // Begin with empty variable name length.
	while (is_var_char(input[*i + 1 + key_len])) // Count valid variable-name characters.
		key_len++;
	key = ft_substr(input, *i + 1, key_len); // Copy variable name without '$'.
	if (!key) // Guard allocation failure.
		return (0); // Signal failure to caller.
	value = get_env_value(env, key); // Fetch variable value from environment list.
	if (value) // If variable exists, append full value.
		while (*value)
			expanded[(*j)++] = *value++; // Copy each character into output and advance write index.
	free(key); // Free temporary key string.
	*i += key_len + 1; // Skip '$' plus key name in input stream.
	return (1); // Signal success.
}

int	process_next_char(char *expanded, int *j, char *input,
		int *i, char *quote, t_env *env) // Process one input unit (quote, variable, or literal char).
{
	if (!*quote && (input[*i] == '\'' || input[*i] == '"')) // Opening quote found outside quote mode.
		*quote = input[(*i)++]; // Enter quote mode and consume quote char (not copied).
	else if (*quote && input[*i] == *quote) // Matching closing quote found.
	{
		*quote = '\0'; // Exit quote mode.
		(*i)++; // Consume closing quote (not copied).
	}
	else if (input[*i] == '$' && *quote != '\'' && (ft_isalpha(input[*i + 1])
			|| input[*i + 1] == '_')) // Expand variable unless inside single quotes.
	{
		if (!append_var_value(expanded, j, input, i, env)) // Append variable expansion text.
			return (0); // Signal failure.
	}
	else // Regular character path.
		expanded[(*j)++] = input[(*i)++]; // Copy one literal char and advance indices.
	return (1); // Signal success.
}

/*input = "echo \"$USER\" '$HOME' $PATH"
env contains: USER=alice HOME = /Users/alice PATH =/usr/bin:/bin
$USER inside double quotes expands
$HOME inside single quotes does not expand
$PATH outside quotes expands*/
char	*expand_variables(char *input, t_env *env) // Return a newly allocated string with $VAR expansions applied.
{
	char	*expanded; // Output buffer holding expanded result.
	char	quote; // Current quote state while scanning input.
	int		i; // Input read index.
	int		j; // Output write index.
	int		len; // Required output length.

	len = expanded_len(input, env); // Precompute length to allocate exact buffer size.
	if (len < 0) // Abort on preprocessing failure.
		return (NULL);
	expanded = malloc(len + 1); // Allocate output (+1 for null terminator).
	if (!expanded) // Handle allocation failure.
		return (NULL);
	i = 0; // Initialize input index.
	j = 0; // Initialize output index.
	quote = '\0'; // Start outside any quote context.
	while (input[i]) // Process full input until null terminator.
	{
		if (!process_next_char(expanded, &j, input, &i, &quote, env)) // Handle one token/char at a time.
			return (free(expanded), NULL); // Cleanup and propagate failure.
	}
	expanded[j] = '\0'; // Terminate resulting C string.
	return (expanded); // Return expanded command string.
}
