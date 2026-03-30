/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:09:09 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/23 11:27:09 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" // Include minishell definitions and quote/expansion helpers.

/*ex: the key is USER (without the $).
i points to the $ character
input[i + 1] starts at U (first char after $)
loop counts: U, S, E, R → key_len = 4
ft_substr(input, i + 1, key_len) extracts 4 chars starting from i+1
Result: key = "USER"
*/
int	var_value_len(char *input, int i, t_env *env, int *skip) // Compute expanded length for variable starting at input[i] ('$').
{
	int		key_len; // Length of variable key after '$'.
	char	*key; // Temporary heap string containing key name.
	char	*value; // Matched environment value for key.
	int		len; // Length of value to add to output.
	char	*status_str;

	if (input[i + 1] == '?')
	{
		status_str = ft_itoa(g_shell.last_status);
		if (!status_str)
			return (-1);
		len = ft_strlen(status_str);
		free(status_str);
		*skip = 2;
		return (len);
	}

	key_len = 0; // Start counting key chars.
	while (is_var_char(input[i + 1 + key_len])) // Count valid variable-name characters.
		key_len++;
	key = ft_substr(input, i + 1, key_len); // Extract key without '$'.
	if (!key) // Handle allocation failure.
		return (-1); 
	value = get_env_value(env, key); // Lookup variable value in environment list.
	len = 0; // Default length when key not found.
	if (value) // If variable exists, use its string length.
		len = ft_strlen(value);
	free(key); // Release temporary key string.
	*skip = key_len + 1; // Tell caller how many input chars were consumed ('$' + key).
	return (len); // Return contribution size to expanded output.
}

/*Example:

Input: echo "$USER" '$HOME'
env: USER=alice, HOME=/Users/alice

Scanning:
e,c,h,o,  → 5 literal chars
" → enter double-quote (not counted)
$USER → gets value length = 5 (alice)
" → exit double-quote (not counted)
  → 1 char (space)
' → enter single-quote (not counted)
$HOME → NOT expanded (inside single quotes) = 5 literal chars
' → exit single-quote (not counted)
Total output length = 5 + 5 + 1 + 5 = 16 chars

Then allocate 17 bytes (16 + null terminator) and fill in second pass.*/
int	count_token_len(char *input, int *i, char *quote,
	t_env *env, int *out_len) // Update output length for one token/char while scanning input.
{
	int	skip; // Number of source chars to skip after variable expansion.
	int	vlen; // Expanded variable value length.

	if (!*quote && (input[*i] == '\'' || input[*i] == '"')) // Enter quote mode on opening quote.
		*quote = input[(*i)++]; // Consume opening quote (not counted in output).
	else if (*quote && input[*i] == *quote) // Detect matching closing quote.
	{
		*quote = '\0'; // Exit quote mode.
		(*i)++; // Consume closing quote (not counted in output).
	}
	else if (input[*i] == '$' && *quote != '\'' && (ft_isalpha(input[*i + 1])
			|| input[*i + 1] == '_' || input[*i + 1] == '?')) // Expand variable unless inside single quotes.
	{
		vlen = var_value_len(input, *i, env, &skip); // Get value length and consumed source width.
		if (vlen < 0) // Stop on allocation/error from helper.
			return (0);
		*out_len += vlen; // Add expanded value length to output length.
		*i += skip; // Advance past '$' and variable name.
	}
	else // Normal literal character path.
	{
		(*out_len)++; // Count one output character.
		(*i)++; // Advance one input character.
	}
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
