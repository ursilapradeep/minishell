/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils_IV.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:53:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 17:34:45 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_word_start(const char *pos, const char *input)
{
	if (pos == input)
		return (1);
	if (*(pos - 1) == ' ' || *(pos - 1) == '\t' || *(pos - 1) == '\n')
		return (1);
	return (0);
}

int	expand_tilde_helper(const char **current, char *result,
	int *result_len, t_env *env)
{
	char	*tilde_result;
	int		consumed;

	consumed = 0;
	tilde_result = ft_calloc(4096, sizeof(char));
	if (!tilde_result)
		return (0);
	if (expand_tilde(*current, env, tilde_result, &consumed))
	{
		ft_strlcpy(result + *result_len, tilde_result, 4096 - *result_len);
		*result_len += ft_strlen(tilde_result);
		*current += consumed;
		free(tilde_result);
		return (1);
	}
	free(tilde_result);
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
int	expand_variable_helper(const char **current, char *result,
		int *result_len, t_env *env);
