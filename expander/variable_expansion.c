/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 12:54:24 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/18 22:00:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	has_unquoted_dollar(const char *value)
{
	int		in_sq;
	int		in_dq;

	in_sq = 0;
	in_dq = 0;
	while (value && *value)
	{
		if (*value == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (*value == '"' && !in_sq)
			in_dq = !in_dq;
		else if (*value == '$' && !in_sq && !in_dq)
			return (1);
		value++;
	}
	return (0);
}

static int	has_unquoted_space(const char *value)
{
	int	in_sq;
	int	in_dq;

	in_sq = 0;
	in_dq = 0;
	while (value && *value)
	{
		if (*value == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (*value == '"' && !in_sq)
			in_dq = !in_dq;
		else if ((*value == ' ' || *value == '\t') && !in_sq && !in_dq)
			return (1);
		value++;
	}
	return (0);
}

static int	handle_ambiguous_redirect(char *original,
		char *expanded, t_token *prev)
{
	int	should_split;

	should_split = has_unquoted_dollar(original);
	if (should_split)
		should_split = has_unquoted_space(expanded);
	if (should_split && prev && prev->type == TOKEN_HEREDOC)
		should_split = 0;
	if (should_split && prev && is_redirect_type(prev->type))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(original, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		return (-1);
	}
	return (should_split);
}

static int	process_word_token(t_token *current, t_token *prev, t_env *env)
{
	char	*expanded;
	char	*unquoted;
	int		should_split;

	if (prev && prev->type == TOKEN_HEREDOC)
		expanded = ft_strdup(current->value);
	else
		expanded = expand_string(current->value, env);
	if (!expanded)
		return (-1);
	should_split = handle_ambiguous_redirect(current->value, expanded, prev);
	if (should_split < 0)
		return (free(expanded), -1);
	unquoted = remove_quotes_string(expanded);
	free(expanded);
	if (!unquoted)
		return (-1);
	free(current->value);
	current->value = unquoted;
	if (should_split && split_unquoted_fields(current) < 0)
		return (-1);
	return (0);
}

int	expand_token_list(t_token *tokens, t_env *env)
{
	t_token	*current;
	t_token	*prev;

	if (!tokens || !env)
		return (0);
	current = tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_WORD
			&& process_word_token(current, prev, env) < 0)
			return (-1);
		prev = current;
		current = current->next;
	}
	return (0);
}
