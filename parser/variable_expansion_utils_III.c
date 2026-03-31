/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils_III.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:10:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/30 15:45:52 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_status_special_case(const char *input, char **exp, int *con)
{
	if (input[1] != '?')
		return (0);
	*exp = ft_itoa(g_last_status);
	if (!*exp)
		return (-1);
	*con = 2;
	return (1);
}

static int	handle_empty_cases(int *args, char **exp, int *con)
{
	int	len;
	int	is_brac;

	len = args[0];
	is_brac = args[1];
	if (len == 0 && !is_brac)
	{
		*exp = ft_calloc(2, sizeof(char));
		if (!*exp)
			return (-1);
		(*exp)[0] = '$';
		return (*con = 1);
	}
	if (len == 0)
	{
		*exp = ft_calloc(1, sizeof(char));
		if (!*exp)
			return (-1);
		if (is_brac)
			*con = 3;
		else
			*con = 1;
		return (1);
	}
	return (0);
}

int	handle_special_cases(const char *in, int *args,
	char **exp, int *con)
{
	int	status_case;

	status_case = handle_status_special_case(in, exp, con);
	if (status_case != 0)
		return (status_case);
	return (handle_empty_cases(args, exp, con));
}
