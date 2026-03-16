/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:09:09 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/16 17:09:31 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	var_value_len(char *input, int i, t_env *env, int *skip)
{
	int		key_len;
	char	*key;
	char	*value;
	int		len;

	key_len = 0;
	while (is_var_char(input[i + 1 + key_len]))
		key_len++;
	key = ft_substr(input, i + 1, key_len);
	if (!key)
		return (-1);
	value = get_env_value(env, key);
	len = 0;
	if (value)
		len = ft_strlen(value);
	free(key);
	*skip = key_len + 1;
	return (len);
}

int	count_token_len(char *input, int *i, char *quote,
	t_env *env, int *out_len)
{
	int	skip;
	int	vlen;

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
		vlen = var_value_len(input, *i, env, &skip);
		if (vlen < 0)
			return (0);
		*out_len += vlen;
		*i += skip;
	}
	else
	{
		(*out_len)++;
		(*i)++;
	}
	return (1);
}
