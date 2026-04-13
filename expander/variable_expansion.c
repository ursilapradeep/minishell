/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 12:54:24 by spaipur-          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2026/04/13 12:07:29 by uvadakku         ###   ########.fr       */
=======
/*   Updated: 2026/04/13 12:23:59 by spaipur-         ###   ########.fr       */
>>>>>>> Stashed changes
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

static char	*expand_token_word(t_token *current, t_token *prev, t_env *env)
{
	if (prev && prev->type == TOKEN_HEREDOC)
		return (ft_strdup(current->value));
	return (expand_string(current->value, env));
}

static int	process_word_token(t_token *current, t_token *prev, t_env *env)
{
	char	*expanded;
	char	*unquoted;
	int		should_split;

	expanded = expand_token_word(current, prev, env);
	if (!expanded)
		return (-1);
<<<<<<< Updated upstream
	should_split = has_unquoted_dollar(current->value)
		&& has_unquoted_space(expanded)
			&& !(prev && prev->type == TOKEN_HEREDOC);
=======
	should_split = has_unquoted_dollar(current->value);
	if (should_split)
		should_split = has_unquoted_space(expanded);
	if (should_split && prev && prev->type == TOKEN_HEREDOC)
		should_split = 0;
>>>>>>> Stashed changes
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
