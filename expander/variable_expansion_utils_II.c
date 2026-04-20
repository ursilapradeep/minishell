/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils_II.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:53:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 17:34:45 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	expand_variable_helper(const char **current, char *result,
	int *result_len, t_env *env)
{
	char	*var_value;
	char	*temp;
	int		consumed;

	var_value = NULL;
	consumed = 0;
	if (expand_variable(*current, env, &var_value, &consumed) == 0)
	{
		if (var_value)
		{
			temp = var_value;
			while (*temp && *result_len < 4095)
			{
				result[(*result_len)++] = *temp;
				temp++;
			}
			free(var_value);
		}
		*current += consumed;
		return (1);
	}
	return (0);
}