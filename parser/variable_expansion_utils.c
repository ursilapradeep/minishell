#include <stdlib.h> // For NULL and memory functions
#include "minishell.h" // For t_env, t_token, and TOKEN_WORD

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
	int			result_len;

	if (!input || !env)
		return (ft_strdup(input));
	result = ft_calloc(4096, sizeof(char));
	if (!result)
		return (NULL);
	result_len = 0;
	if (process_input(input, result, &result_len, env) == -1)
	{
		free(result);
		return (NULL);
	}
	result[result_len] = '\0';
	return (result);
}

// Helper function to process the input string
static int process_input(const char *input, char *result, int *result_len, t_env *env)
{
    const char *current = input;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (current && *current && *result_len < 4095)
    {// Handle quotes
        if (handle_quotes(&current, &in_single_quote, &in_double_quote))
            continue; // Quote state was toggled, move to next character
        if (*current == '$' && !in_single_quote)
        {
            if (expand_variable_helper(&current, result, result_len, env))
                continue;
            else
                return (-1);
        } // Regular character
        result[(*result_len)++] = *current;
        current++;
    }
    return (0);
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

/**
 * handle_quotes - Helper function to manage quote states
 * @current: Pointer to the current character in the input string
 * @in_single_quote: Pointer to the single quote state
 * @in_double_quote: Pointer to the double quote state
 * Return: 1 if a quote was handled, 0 otherwise
 *
 * This function toggles the state of single and double quotes
 * when encountering quote characters. It also advances the
 * current character pointer.
 */
static int handle_quotes(const char **current, int *in_single_quote, int *in_double_quote)
{
	if (**current == '\'' && !(*in_double_quote))
	{
		*in_single_quote = !(*in_single_quote);
		(*current)++;
		return (1);
	}
	if (**current == '"' && !(*in_single_quote))
	{
		*in_double_quote = !(*in_double_quote);
		(*current)++;
		return (1);
	}
	return (0);
}

/**
 * expand_variable_helper - Helper function to expand variables
 * @current: Pointer to the current character in the input string
 * @result: The result string being built
 * @result_len: Pointer to the length of the result string
 * @env: Environment list
 * Return: 1 on successful variable expansion, 0 on failure
 *
 * This function expands variables by calling the expand_variable
 * function and appending the result to the output string. It also
 * advances the current character pointer.
 */
static int expand_variable_helper(const char **current, char *result, int *result_len, t_env *env)
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
