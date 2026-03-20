/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:15:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/20 12:47:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

/**
 * is_quote - Check if character is a quote
 * @c: Character to check
 * Return: 1 if quote, 0 otherwise
 */
int is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/**
 * extract_quoted_string - Extract string enclosed in quotes
 * @input: Input string starting with quote
 * @len: Pointer to store length
 * Return: Allocated string without quotes, NULL on error
 */
char *extract_quoted_string(const char *input, int *len)
{
	const char	*start;
	const char	*end;
	char		quote_char;
	char		*result;
	int			result_len;

	if (!input || !is_quote(*input))
		return (NULL);
	quote_char = *input;
	start = input + 1;
	end = start;
	while (*end && *end != quote_char)
		end++;
	if (!*end)
	{
		write(STDERR_FILENO, "Error: Unclosed quote\n", 23);
		return (NULL);
	}
	result_len = end - start;
	result = ft_calloc(result_len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, result_len + 1);
	*len = (end - input) + 1;
	return (result);
}

/**
 * extract_word - Extract unquoted word from input
 * @input: Input string
 * @len: Pointer to store consumed characters
 * Return: Allocated word string, NULL on error
 */
char *extract_word(const char *input, int *len)
{
	const char	*start;
	const char	*end;
	int			word_len;
	char		*word;

	if (!input || !*input)
		return (NULL);
	start = input;
	end = start;
	while (*end && !isspace(*end) && *end != '|' && *end != '>' && *end != '<'
		&& !is_quote(*end))
		end++;
	if (start == end)
		return (NULL);
	word_len = end - start;
	word = ft_calloc(word_len + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, start, word_len + 1);
	*len = word_len;
	return (word);
}

/**
 * extract_redirect_operator - Extract redirect operator (>, >>, <, <<)
 * @input: Input string
 * @len: Pointer to store operator length
 * Return: Operator string, NULL on error
 */
char *extract_redirect_operator(const char *input, int *len)
{
	char *operator;

	if (!input || (*input != '>' && *input != '<'))
		return (NULL);
	if ((input[0] == '>' && input[1] == '>') ||
		(input[0] == '<' && input[1] == '<'))
	{
		operator = ft_calloc(3, sizeof(char));
		if (!operator)
			return (NULL);
		operator[0] = input[0];
		operator[1] = input[1];
		operator[2] = '\0';
		*len = 2;
	}
	else
	{
		operator = ft_calloc(2, sizeof(char));
		if (!operator)
			return (NULL);
		operator[0] = input[0];
		operator[1] = '\0';
		*len = 1;
	}
	return (operator);
}

/**
 * skip_whitespace_simple - Skip whitespace
 * @input: String to scan
 * Return: Pointer after whitespace
 */
const char *skip_whitespace_simple(const char *input)
{
	if (!input)
		return (NULL);
	while (*input && isspace(*input))
		input++;
	return (input);
}
