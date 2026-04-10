/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 15:29:50 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/10 15:35:14 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static const char	*skip_whitespace_simple(const char *input)
{
	if (!input)
		return (NULL);
	while (*input && (*input == ' ' || *input == '	' || *input == '\n'))
		input++;
	return (input);
}

static char	*determine_token_value(const char **current,
		t_token_type *token_type, int *consumed)
{
	char	*token_value;

	token_value = NULL;
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
	return (token_value);
}

static t_token	*process_token(const char **current, t_token **tokens)
{
	t_token			*new_token;
	char			*token_value;
	t_token_type	token_type;
	int				consumed;

	token_value = NULL;
	token_type = TOKEN_WORD;
	consumed = 0;
	token_value = determine_token_value(current, &token_type, &consumed);
	if (!token_value)
	{
		if (consumed == -1)
			return (NULL);
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

static void	mark_whitespace_boundary(t_token **tokens,
		const char *before_skip, const char *after_skip)
{
	t_token	*last_token;

	if (before_skip != after_skip && *tokens)
	{
		last_token = *tokens;
		while (last_token->next)
			last_token = last_token->next;
		last_token->prev = last_token;
	}
}

t_token	*tokenize(const char *input)
{
	const char	*current;
	const char	*before_skip;
	t_token		*tokens;

	tokens = NULL;
	if (!input || !*input)
		return (NULL);
	current = input;
	while (current && *current)
	{
		before_skip = current;
		current = skip_whitespace_simple(current);
		if (!*current)
			break ;
		mark_whitespace_boundary(&tokens, before_skip, current);
		if (!process_token(&current, &tokens))
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	tokens = merge_consecutive_words(tokens);
	return (tokens);
}
