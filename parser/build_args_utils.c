/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 19:28:23 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/10 13:29:24 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redirect_token(t_token *token)
{
	return (token->type == TOKEN_REDIRECT_IN
		|| token->type == TOKEN_REDIRECT_OUT
		|| token->type == TOKEN_REDIRECT_APPEND
		|| token->type == TOKEN_HEREDOC);
}

static int	is_valid_argument(t_token *current)
{
	return (current->type == TOKEN_WORD && (!current->prev
			|| !is_redirect_token(current->prev)));
}

int	is_redirect_type(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND
		|| type == TOKEN_HEREDOC);
}

int	is_valid_arg_type(t_token *curr)
{
	int	preceded;
	int	followed;

	preceded = curr->prev
		&& is_redirect_token(curr->prev);
	followed = curr->next
		&& is_redirect_token(curr->next);
	return (curr->type == TOKEN_WORD && !preceded && !followed);
}

int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;
	int		skip_next;

	if (!tokens)
		return (0);
	count = 0;
	current = tokens;
	skip_next = 0;
	while (current && current->type != TOKEN_PIPE)
	{
		if (skip_next)
		{
			skip_next = 0;
			current = current->next;
			continue ;
		}
		if (is_valid_argument(current))
			count++;
		else if (is_redirect_token(current))
			skip_next = 1;
		current = current->next;
	}
	return (count);
}
