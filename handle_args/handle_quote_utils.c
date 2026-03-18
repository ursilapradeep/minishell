/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:09:09 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/18 15:33:18 by uvadakku         ###   ########.fr       */
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
			|| input[*i + 1] == '_')) // Expand variable unless inside single quotes.
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
