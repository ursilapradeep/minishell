/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 16:10:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/30 11:29:48 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h" // Include minishell headers, env accessors, and libft helpers.

// int	is_var_char(char c) // Check if character is valid in an environment variable name.
// {
// 	return (ft_isalnum(c) || c == '_'); // Allow letters/digits and underscore.
// }

// int	expanded_len(char *input, t_env *env) // Compute required length of expanded string before allocation.
// {
// 	int		i; // Read index over input.
// 	int		len; // Total output length after expansion.
// 	char	quote; // Active quote context ('\0', '\'', or '"').

// 	i = 0; // Start from first character.
// 	len = 0; // Initialize output length counter.
// 	quote = '\0'; // Start outside quotes.
// 	while (input[i]) // Walk through input to count expansion result length.
// 	{
// 		if (!count_token_len(input, &i, &quote, env, &len)) // Delegate per-token length accounting.
// 			return (-1); // Propagate error (e.g., allocation failure inside helpers).
// 	}
// 	return (len); // Return exact allocation size needed (without null terminator).
// }

// static int	append_last_status(char *expanded, int *j, int *i)
// {
// 	char	*status_str;
// 	char	*tmp;

// 	status_str = ft_itoa(g_last_status);
// 	if (!status_str)
// 		return (0);
// 	tmp = status_str;
// 	while (*tmp)
// 		expanded[(*j)++] = *tmp++;
// 	free(status_str);
// 	*i += 2;
// 	return (1);
// }

// int	append_var_value(char *expanded, int *j, char *input,
// 		int *i, t_env *env) // Expand $VAR at input[*i] and append its value into expanded buffer.
// {
// 	int		key_len; // Length of variable key name after '$'.
// 	char	*key; // Extracted variable key (heap-allocated).
// 	char	*value; // Value looked up in environment.

// 	if (input[*i + 1] == '?')
// 		return (append_last_status(expanded, j, i));
// 	key_len = 0; // Begin with empty variable name length.
// 	while (is_var_char(input[*i + 1 + key_len])) // Count valid variable-name characters.
// 		key_len++;
// 	key = ft_substr(input, *i + 1, key_len); // Copy variable name without '$'.
// 	if (!key) // Guard allocation failure.
// 		return (0); // Signal failure to caller.
// 	value = get_env_value(env, key); // Fetch variable value from environment list.
// 	if (value) // If variable exists, append full value.
// 		while (*value)
// 			expanded[(*j)++] = *value++; // Copy each character into output and advance write index.
// 	free(key); // Free temporary key string.
// 	*i += key_len + 1; // Skip '$' plus key name in input stream.
// 	return (1); // Signal success.
// }

// int	process_next_char(char *expanded, int *j, char *input,
// 		int *i, char *quote, t_env *env) // Process one input unit (quote, variable, or literal char).
// {
// 	if (!*quote && (input[*i] == '\'' || input[*i] == '"')) // Opening quote found outside quote mode.
// 		*quote = input[(*i)++]; // Enter quote mode and consume quote char (not copied).
// 	else if (*quote && input[*i] == *quote) // Matching closing quote found.
// 	{
// 		*quote = '\0'; // Exit quote mode.
// 		(*i)++; // Consume closing quote (not copied).
// 	}
// 	else if (input[*i] == '$' && *quote != '\'' && (ft_isalpha(input[*i + 1])
// 			|| input[*i + 1] == '_' || input[*i + 1] == '?')) // Expand variable unless inside single quotes.
// 	{
// 		if (!append_var_value(expanded, j, input, i, env)) // Append variable expansion text.
// 			return (0); // Signal failure.
// 	}
// 	else // Regular character path.
// 		expanded[(*j)++] = input[(*i)++]; // Copy one literal char and advance indices.
// 	return (1); // Signal success.
// }


