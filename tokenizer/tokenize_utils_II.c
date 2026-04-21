/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_II.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:53:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/21 13:14:59 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_and_operator(const char **current,
			t_token_type *token_type, int *consumed)
{
	if (**current == '&' && *(*current + 1) == '&')
	{
		*token_type = TOKEN_AND;
		*consumed = 2;
		return (ft_strdup("&&"));
	}
	return (NULL);
}

static char	*handle_or_operator(const char **current,
			t_token_type *token_type, int *consumed)
{
	if (**current == '|' && *(*current + 1) == '|')
	{
		*token_type = TOKEN_OR;
		*consumed = 2;
		return (ft_strdup("||"));
	}
	return (NULL);
}

static char	*handle_pipe_token(const char **current,
			t_token_type *token_type, int *consumed)
{
	char	*token_value;

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
		return (token_value);
	}
	return (NULL);
}

static char	*handle_special_token(const char **current,
			t_token_type *token_type, int *consumed)
{
	if (**current == '(')
	{
		*token_type = TOKEN_LPAREN;
		*consumed = 1;
		return (ft_strdup("("));
	}
	if (**current == ')')
	{
		*token_type = TOKEN_RPAREN;
		*consumed = 1;
		return (ft_strdup(")"));
	}
	if (**current == '&')
	{
		*token_type = TOKEN_SPECIAL;
		*consumed = 1;
		return (ft_strdup("&"));
	}
	return (NULL);
}

char	*determine_token_value(const char **current,
		t_token_type *token_type, int *consumed)
{
	char	*token_value;

	token_value = handle_and_operator(current, token_type, consumed);
	if (token_value)
		return (token_value);
	token_value = handle_or_operator(current, token_type, consumed);
	if (token_value)
		return (token_value);
	token_value = handle_pipe_token(current, token_type, consumed);
	if (token_value)
		return (token_value);
	token_value = handle_special_token(current, token_type, consumed);
	if (token_value)
		return (token_value);
	if (**current == '>' || **current == '<')
		return (handle_redirection_token(current, token_type, consumed));
	if (is_quote(**current))
		return (extract_quoted_string(*current, consumed));
	return (extract_word(*current, consumed));
}
