/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:15:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 10:39:42 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

/**
 * create_token - Create a new token node
 * @value: Token string value
 * @type: Token type enum
 * Return: New token pointer, NULL on error
 */
static t_token *create_token(char *value, t_token_type type)
{
	t_token *token;

	if (!value)
		return (NULL);
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

/**
 * add_token - Add token to end of linked list
 * @head: Pointer to list head
 * @new_token: Token to add
 */
static void add_token(t_token **head, t_token *new_token)
{
	t_token *current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
}

/**
 * is_quote - Check if character is a quote
 * @c: Character to check
 * Return: 1 if quote, 0 otherwise
 */
static int is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/**
 * extract_quoted_string - Extract string enclosed in quotes
 * @input: Input string starting with quote
 * @len: Pointer to store length
 * Return: Allocated string without quotes, NULL on error
 */
static char *extract_quoted_string(const char *input, int *len)
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
static char *extract_word(const char *input, int *len)
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
static char *extract_redirect_operator(const char *input, int *len)
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
static const char *skip_whitespace_simple(const char *input)
{
	if (!input)
		return (NULL);
	while (*input && isspace(*input))
		input++;
	return (input);
}

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
t_token *tokenize(char *input)
{
	t_token			*tokens;
	t_token			*new_token;
	const char		*current;
	char			*token_value;
	t_token_type	token_type;
	int				consumed;

	if (!input || !*input)
		return (NULL);
	tokens = NULL;
	current = input;
	while (current && *current)
	{
		current = skip_whitespace_simple(current);
		if (!*current)
			break ;
		consumed = 0;
		token_value = NULL;
		token_type = TOKEN_WORD;
		
		if (*current == '|')
		{
			token_value = ft_calloc(2, sizeof(char));
			if (token_value)
			{
				token_value[0] = '|';
				token_value[1] = '\0';
			}
			token_type = TOKEN_PIPE;
			consumed = 1;
		}
		else if (*current == '>' || *current == '<')
		{
			token_value = extract_redirect_operator(current, &consumed);
			if (*current == '<' && current[1] == '<')
				token_type = TOKEN_HEREDOC;
			else if (*current == '<')
				token_type = TOKEN_REDIRECT_IN;
			else if (current[1] == '>')
				token_type = TOKEN_REDIRECT_APPEND;
			else
				token_type = TOKEN_REDIRECT_OUT;
		}
		else if (is_quote(*current))
		{
			token_value = extract_quoted_string(current, &consumed);
			if (!token_value)
				goto tokenize_error;
		}
		else
		{
			token_value = extract_word(current, &consumed);
			if (!token_value)
			{
				current++;
				continue ;
			}
		}
		if (token_value)
		{
			new_token = create_token(token_value, token_type);
			if (!new_token)
				goto tokenize_error;
			add_token(&tokens, new_token);
		}
		current += consumed;
	}
	return (tokens);
tokenize_error:
	free_tokens(tokens);
	return (NULL);
}

/**
 * free_tokens - Free entire token list
 * @tokens: Token list head
 */
void free_tokens(t_token *tokens)
{
	t_token *current;
	t_token *next;

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

/**
 * print_tokens - DEBUG: Print all tokens
 * @tokens: Token list head
 */
void print_tokens(t_token *tokens)
{
	t_token	*current;
	char	*type_str;

	if (!tokens)
	{
		printf("No tokens\n");
		return ;
	}
	current = tokens;
	printf("=== TOKENS ===\n");
	while (current)
	{
		type_str = "WORD";
		if (current->type == TOKEN_PIPE)
			type_str = "PIPE";
		else if (current->type == TOKEN_REDIRECT_IN)
			type_str = "REDIRECT_IN (<)";
		else if (current->type == TOKEN_REDIRECT_OUT)
			type_str = "REDIRECT_OUT (>)";
		else if (current->type == TOKEN_REDIRECT_APPEND)
			type_str = "REDIRECT_APPEND (>>)";
		else if (current->type == TOKEN_HEREDOC)
			type_str = "HEREDOC (<<)";
		printf("[%s]: \"%s\"\n", type_str, current->value);
		current = current->next;
	}
	printf("=============\n");
}
