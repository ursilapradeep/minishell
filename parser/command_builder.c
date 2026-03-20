/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/20 11:27:56 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

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
 * handle_redirection - Helper function to handle a specific redirection type
 * @cmd: Command to update with redirection
 * @current: Current token representing the redirection
 * @redirection_type: Type of redirection (in, out, append)
 * Return: 1 on success, -1 on error, 0 if not a redirection token
 */
int handle_redirection(t_cmd *cmd, t_token *current, int redirection_type)
{
	char	*filename;
	int		fd;

	if (!current->next || current->next->type != TOKEN_WORD)
	{
		write(STDERR_FILENO, "Error: No filename after redirection\n", 37);
		return (-1);
	}
	filename = current->next->value;

	fd = open_redirection_file(cmd, filename, redirection_type);
	if (fd < 0)
		return (-1);

	if (redirection_type == TOKEN_REDIRECT_IN)
		cmd->infd = fd;
	else
		cmd->outfd = fd;

	return (1);
}

/**
 * open_redirection_file - Helper function to open a file for redirection
 * @cmd: Command structure to update file descriptors
 * @filename: Name of the file to open
 * @redirection_type: Type of redirection (in, out, append)
 * Return: File descriptor on success, -1 on error
 */
int open_redirection_file(t_cmd *cmd, char *filename, int redirection_type)
{
	int fd;

	if (redirection_type == TOKEN_REDIRECT_IN)
	{
		if (cmd->infd > 2)
			close(cmd->infd);
		fd = open(filename, O_RDONLY);
	}
	else if (redirection_type == TOKEN_REDIRECT_OUT)
	{
		if (cmd->outfd > 2)
			close(cmd->outfd);
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (redirection_type == TOKEN_REDIRECT_APPEND)
	{
		if (cmd->outfd > 2)
			close(cmd->outfd);
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
		return (-1);
	if (fd < 0)
		perror("minishell");
	return fd;
}

