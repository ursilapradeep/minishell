/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 18:02:23 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/30 18:04:00 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_empty_variable_name(int is_braced, char **expanded, int *consumed)
{
	*expanded = ft_calloc(1, sizeof(char));
	if (!*expanded)
		return (-1);
	(*expanded)[0] = '\0';
	if (is_braced)
		*consumed = 3;
	else
		*consumed = 1;
	return (1);
}

int	handle_status_special_case(const char *input, char **expanded,
		int *consumed)
{
	if (input[1] != '?')
		return (0);
	*expanded = ft_itoa(g_shell.last_status);
	if (!*expanded)
		return (-1);
	*consumed = 2;
	return (1);
}

int	handle_special_cases(const char *input, int var_len,
	int is_braced, t_var_result *result)
{
	int	status_case;

	status_case = handle_status_special_case(input,
			&result->expanded, &result->consumed);
	if (status_case != 0)
		return (status_case);
	if (var_len == 0 && !is_braced)
	{
		result->expanded = ft_calloc(2, sizeof(char));
		if (!result->expanded)
			return (-1);
		result->expanded[0] = '$';
		result->expanded[1] = '\0';
		result->consumed = 1;
		return (1);
	}
	if (var_len == 0)
		return (handle_empty_variable_name(is_braced,
				&result->expanded, &result->consumed));
	return (0);
}
