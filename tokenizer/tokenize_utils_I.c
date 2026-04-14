/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_I.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:15:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 15:29:39 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * is_quote - Check if character is a quote
 * @c: Character to check
 * Return: 1 if quote, 0 otherwise
 */
int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/**
 * extract_quoted_string - Extract string enclosed in quotes
 * @input: Input string starting with quote
 * @len: Pointer to store length
 * Return: Allocated string without quotes, NULL on error
 */
char	*extract_quoted_string(const char *input, int *len)
{
	const char	*end;
	char		*result;

	if (!input || !is_quote(*input))
		return (NULL);
	end = input + 1;
	while (*end && *end != *input)
		end++;
	if (!*end)
	{
		write(STDERR_FILENO, "Error: Unclosed quote\n", 23);
		*len = -1;
		return (NULL);
	}
	end++;
	result = ft_calloc((end - input) + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, input, (end - input) + 1);
	*len = end - input;
	return (result);
}

/**
 * extract_word - Extract unquoted word from input
 * @input: Input string
 * @len: Pointer to store consumed characters
 * Return: Allocated word string, NULL on error
 */
char	*extract_word(const char *input, int *len)
{
	const char	*end;
	char		*word;

	if (!input || !*input)
		return (NULL);
	end = input;
	while (*end)
	{
		if (*end == '\\' && end[1])
		{
			end += 2;
			continue ;
		}
		if (*end == ' ' || (*end >= '\t' && *end <= '\r')
			|| *end == '|' || *end == '>' || *end == '<' || is_quote(*end))
			break ;
		end++;
	}
	if (input == end)
		return (NULL);
	word = ft_calloc((end - input) + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, input, (end - input) + 1);
	*len = end - input;
	return (word);
}

/**
 * extract_redirect_operator - Extract redirect operator (>, >>, <, <<)
 * @input: Input string
 * @len: Pointer to store operator length
 * Return: Operator string, NULL on error
 */
static char	*extract_redirect_operator(const char *input, int *len)
{
	char	*op;
	int		op_len;

	if (!input || (*input != '>' && *input != '<'))
		return (NULL);
	op_len = 1;
	if ((input[0] == '>' && (input[1] == '>' || input[1] == '|'))
		|| (input[0] == '<' && input[1] == '<'))
		op_len = 2;
	op = ft_calloc(op_len + 1, sizeof(char));
	if (!op)
		return (NULL);
	ft_strlcpy(op, input, op_len + 1);
	*len = op_len;
	return (op);
}

char	*handle_redirection_token(const char **cur, t_token_type *typ, int *len)
{
	char	*token_value;

	token_value = extract_redirect_operator(*cur, len);
	if (**cur == '<' && (*cur)[1] == '<')
		*typ = TOKEN_HEREDOC;
	else if (**cur == '<')
		*typ = TOKEN_REDIRECT_IN;
	else if ((*cur)[1] == '>')
		*typ = TOKEN_REDIRECT_APPEND;
	else
		*typ = TOKEN_REDIRECT_OUT;
	return (token_value);
}
