/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils_VI.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:53:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 17:34:45 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_dollar_special(const char **current, char *result,
		int *result_len, int state)
{
	if (**current == '$' && ((*current)[1] == '"' || (*current)[1] == '\'')
		&& !(state & 1))
	{
		if (state & 2 && (*current)[1] == '"')
			result[(*result_len)++] = '$';
		(*current)++;
		return (1);
	}
	return (0);
}

static int	proc_input(const char *input, char *result, int *result_len, t_env *env)
{
	const char	*current;
	int			in_sq;
	int			in_dq;

	current = input;
	in_sq = 0;
	in_dq = 0;
	while (current && *current && *result_len < 4095)
	{
		update_quote_state(*current, &in_sq, &in_dq);
		if (handle_dollar_special(&current, result, result_len,
				in_sq + (in_dq << 1)))
			continue ;
		if (*current == '~' && !in_sq && is_word_start(current, input))
			if (expand_tilde_helper(&current, result, result_len, env))
				continue ;
		if (*current == '$' && !in_sq
			&& expand_variable_helper(&current, result, result_len, env))
			continue ;
		result[(*result_len)++] = *current;
		current++;
	}
	return (0);
}

char	*expand_string(const char *input, t_env *env)
{
	char	*result;
	int		result_len;

	if (!input || !env)
		return (ft_strdup(input));
	result = ft_calloc(4096, sizeof(char));
	if (!result)
		return (NULL);
	result_len = 0;
	if (proc_input(input, result, &result_len, env) == -1)
	{
		free(result);
		return (NULL);
	}
	result[result_len] = '\0';
	return (result);
}
