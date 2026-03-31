/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 15:29:50 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 17:17:44 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/**
 * tokenize - Parse input string into token linked list
 * @input: User input string
 * Return: Token list head, NULL on error
 *
 * Handles:
 * - Regular words (commands, arguments, filenames)
 * - Quoted strings (preserves spaces inside quotes)
 * - Pipes (|)
 * - Redirections (>, >>, <, <<)
 */

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

static char *determine_token_value(const char **current, t_token_type *token_type, int *consumed)
{
	char *token_value = NULL;

	if (**current == '|')
	{
		token_value = ft_calloc(2, sizeof(char));
		if (token_value)
		{
			token_value[0] = '|';
			token_value[1] = '\0';
		}
		*token_type = TOKEN_PIPE;
		*consumed = 1;
	}
	else if (**current == '>' || **current == '<')
		token_value = handle_redirection_token(current, token_type, consumed);
	else if (is_quote(**current))
		token_value = extract_quoted_string(*current, consumed);
	else
		token_value = extract_word(*current, consumed);
	return token_value;
}

static t_token *process_token(const char **current, t_token **tokens)
{
	t_token *new_token;
	char *token_value = NULL;
	t_token_type token_type = TOKEN_WORD;
	int consumed = 0;

	token_value = determine_token_value(current, &token_type, &consumed);
	if (!token_value)
	{
		(*current)++;
		return (*tokens);
	}
	new_token = create_token(token_value, token_type);
	if (!new_token)
		return (NULL);
	add_token(tokens, new_token);
	*current += consumed;
	return (*tokens);
}

static t_token *handle_tokenize_error(t_token *tokens)
{
	free_tokens(tokens);
	return (NULL);
}

t_token *tokenize(char *input)
{
	t_token *tokens = NULL;
	const char *current;

	if (!input || !*input)
		return (NULL);
	current = input;
	while (current && *current)
	{
		current = skip_whitespace_simple(current);
		if (!*current)
			break;
		if (!process_token(&current, &tokens))
			return handle_tokenize_error(tokens);
	}
	return (tokens);
}


