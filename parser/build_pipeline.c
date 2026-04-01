/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:55:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/31 21:09:53 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	build_pipeline(t_cmd *commands)
{
	(void)commands;
	return (0);
}

/*
 * Finds the first pipe token in the remainder of the token list.
 * This sets the boundary so the next build_single_cmd() knows where to start.
 */
t_token	*find_next_pipe(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			return (current);
		current = current->next;
	}
	return (NULL);
}
