/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/30 12:55:58 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	process_quote_removal(const char *str, char *res)
{
	int	i;
	int	j;
	int	in_sq;
	int	in_dq;

	i = 0;
	j = 0;
	in_sq = 0;
	in_dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (str[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
}

/**
 * remove_quotes_string - Strips out unquoted single and double quotes
 * @str: The input string (already expanded)
 * Return: A newly allocated string with outer quotes removed, NULL on error
 */
char	*remove_quotes_string(const char *str)
{
	char	*res;

	if (!str)
		return (NULL);
	res = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!res)
		return (NULL);
	process_quote_removal(str, res);
	return (res);
}
