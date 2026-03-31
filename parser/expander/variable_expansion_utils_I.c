/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils_I.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/30 15:33:53 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_env_variable(t_env *env, const char *var_name, int len)
{
	t_env	*current;

	if (!var_name || len <= 0 || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->key && (int)ft_strlen(current->key) == len
			&& ft_strncmp(current->key, var_name, len) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

static int	allocate_expanded_value(const char *value, char **expanded)
{
	int	value_len;

	if (value)
	{
		value_len = ft_strlen(value);
		*expanded = ft_calloc(value_len + 1, sizeof(char));
		if (!*expanded)
			return (-1);
		ft_strlcpy(*expanded, value, value_len + 1);
	}
	else
	{
		*expanded = ft_calloc(1, sizeof(char));
		if (!*expanded)
			return (-1);
		(*expanded)[0] = '\0';
	}
	return (0);
}

int	expand_variable(const char *input, t_env *env, char **exp, int *con)
{
	const char	*v[3];
	int			i[3];
	char		*value;

	if (!input || input[0] != '$' || !exp || !con)
		return (-1);
	v[0] = input + 1;
	v[1] = extract_var_name(v[0], &i[0], &i[1]);
	if (!v[1])
		return (-1);
	i[2] = handle_special_cases(input, i, exp, con);
	if (i[2] != 0)
	{
		if (i[2] == 1)
			return (0);
		return (-1);
	}
	v[2] = v[0];
	if (i[1])
		v[2] = v[0] + 1;
	value = get_env_variable(env, v[2], i[0]);
	if (allocate_expanded_value(value, exp) == -1)
		return (-1);
	*con = (v[1] - input);
	return (0);
}
