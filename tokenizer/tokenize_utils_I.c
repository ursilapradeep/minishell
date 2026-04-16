/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_I.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:15:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/14 22:27:17 by spaipur-         ###   ########.fr       */
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
static const char	*find_word_end(const char *input)
{
	const char	*end;

	end = input;
	while (*end)
	{
		if (*end == '\\' && end[1])
		{
			end += 2;
			continue ;
		}
		if (*end == ' ' || (*end >= '\t' && *end <= '\r')
			|| *end == '|' || *end == '>' || *end == '<'
			|| *end == '&' || is_quote(*end))
			break ;
		end++;
	}
	return (end);
}

char	*extract_word(const char *input, int *len)
{
	const char	*end;
	char		*word;

	if (!input || !*input)
		return (NULL);
	end = find_word_end(input);
	if (input == end)
		return (NULL);
	word = ft_calloc((end - input) + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, input, (end - input) + 1);
	*len = end - input;
	return (word);
}

char	*handle_redirection_token(const char **cur, t_token_type *typ, int *len)
{
	char	*token_value;
	int		op_len;

	if (!cur || !*cur || (**cur != '>' && **cur != '<'))
		return (NULL);
	op_len = 1;
	if (((*cur)[0] == '>' && ((*cur)[1] == '>' || (*cur)[1] == '|'))
		|| ((*cur)[0] == '<' && (*cur)[1] == '<'))
		op_len = 2;
	token_value = ft_calloc(op_len + 1, sizeof(char));
	if (!token_value)
		return (NULL);
	ft_strlcpy(token_value, *cur, op_len + 1);
	*len = op_len;
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
