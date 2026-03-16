/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 16:10:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/16 17:08:12 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	expanded_len(char *input, t_env *env)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	len = 0;
	quote = '\0';
	while (input[i])
	{
		if (!count_token_len(input, &i, &quote, env, &len))
			return (-1);
	}
	return (len);
}

int	append_var_value(char *expanded, int *j, char *input,
		int *i, t_env *env)
{
	int		key_len;
	char	*key;
	char	*value;

	key_len = 0;
	while (is_var_char(input[*i + 1 + key_len]))
		key_len++;
	key = ft_substr(input, *i + 1, key_len);
	if (!key)
		return (0);
	value = get_env_value(env, key);
	if (value)
		while (*value)
			expanded[(*j)++] = *value++;
	free(key);
	*i += key_len + 1;
	return (1);
}

int	process_next_char(char *expanded, int *j, char *input,
		int *i, char *quote, t_env *env)
{
	if (!*quote && (input[*i] == '\'' || input[*i] == '"'))
		*quote = input[(*i)++];
	else if (*quote && input[*i] == *quote)
	{
		*quote = '\0';
		(*i)++;
	}
	else if (input[*i] == '$' && *quote != '\'' && (ft_isalpha(input[*i + 1])
			|| input[*i + 1] == '_'))
	{
		if (!append_var_value(expanded, j, input, i, env))
			return (0);
	}
	else
		expanded[(*j)++] = input[(*i)++];
	return (1);
}

char	*expand_variables(char *input, t_env *env)
{
	char	*expanded;
	char	quote;
	int		i;
	int		j;
	int		len;

	len = expanded_len(input, env);
	if (len < 0)
		return (NULL);
	expanded = malloc(len + 1);
	if (!expanded)
		return (NULL);
	i = 0;
	j = 0;
	quote = '\0';
	while (input[i])
	{
		if (!process_next_char(expanded, &j, input, &i, &quote, env))
			return (free(expanded), NULL);
	}
	expanded[j] = '\0';
	return (expanded);
}
