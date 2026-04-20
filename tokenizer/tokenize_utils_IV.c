/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_IV.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:35:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 17:34:45 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * merge_tokens_values - Merge two token values into one
 * @current: Current token to merge into
 * @next: Next token to merge from
 * Return: 1 on success, 0 on error
 */
static int	merge_tokens_values(t_token *current, t_token *next)
{
	char	*merged_value;
	int		merged_len;
	int		had_boundary;

	had_boundary = (next->prev == next);
	merged_len = ft_strlen(current->value) + ft_strlen(next->value) + 1;
	merged_value = ft_calloc(merged_len, sizeof(char));
	if (!merged_value)
		return (0);
	ft_strlcpy(merged_value, current->value, merged_len);
	ft_strlcat(merged_value, next->value, merged_len);
	free(current->value);
	current->value = merged_value;
	if (next->quoted)
		current->quoted = 1;
	current->next = next->next;
	if (next->next && next->next->prev != next->next)
		next->next->prev = current;
	if (had_boundary)
		current->prev = current;
	free(next);
	return (1);
}

/**
 * process_merge_iteration - Process one iteration of merge
 * @current: Current token pointer
 * Return: Next token to process, or NULL if no more
 */
static t_token	*process_merge_iteration(t_token *current)
{
	t_token	*next;
	int		should_stop_merge;

	next = current->next;
	if (next && current->type == TOKEN_WORD && next->type == TOKEN_WORD)
	{
		should_stop_merge = (current->prev == current);
		if (!should_stop_merge)
		{
			if (!merge_tokens_values(current, next))
				return (current->next);
			return (current);
		}
		return (next);
	}
	return (next);
}

/**
 * merge_consecutive_words - Merge consecutive TOKEN_WORD tokens
 * @tokens: Head of token list
 *
 * Description:
 *   Merges adjacent quoted/unquoted strings into single argument.
 *   Respects whitespace boundaries marked by prev==prev.
 *
 * Return: Head of the merged token list
 */
t_token	*merge_consecutive_words(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (tokens);
	current = tokens;
	while (current)
		current = process_merge_iteration(current);
	return (tokens);
}
