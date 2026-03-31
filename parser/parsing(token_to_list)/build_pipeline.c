/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                       
                         +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 11:04:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/20 11:04:11 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * find_next_pipe - Find the next PIPE token
 * @tokens: Token list to search
 * Return: Pointer to PIPE token or NULL if not found
 */
t_token *find_next_pipe(t_token *tokens)
{
	t_token *current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * build_pipeline - Connect commands in a pipeline with pipes
 * @commands: List of commands to connect
 * Return: 0 on success, -1 on error
 */
int build_pipeline(t_cmd *commands)
{
	t_cmd	*current;
	int		pipe_fds[2];

	if (!commands || !commands->next)
		return (0);
	current = commands;
	while (current && current->next)
	{
		if (pipe(pipe_fds) < 0)
		{
			perror("minishell");
			return (-1);
		}/* Current command writes to pipe */
		if (current->outfd == STDOUT_FILENO)
			current->outfd = pipe_fds[1];
		else
			close(pipe_fds[1]); /* Next command reads from pipe */
		if (current->next->infd == STDIN_FILENO)
			current->next->infd = pipe_fds[0];
		else
			close(pipe_fds[0]);
		current = current->next;
	}
	return (0);
}


