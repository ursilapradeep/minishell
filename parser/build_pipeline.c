/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:55:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/10 13:29:34 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	create_pipe_between_commands(t_cmd *current, t_cmd *next)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	current->outfd = pipefd[1];
	next->infd = pipefd[0];
	return (0);
}

int	build_pipeline(t_cmd *commands)
{
	t_cmd	*current;

	if (!commands)
		return (0);
	current = commands;
	while (current && current->next)
	{
		if (create_pipe_between_commands(current, current->next) == -1)
			return (-1);
		current = current->next;
	}
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
