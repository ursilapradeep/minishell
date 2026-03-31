/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 19:42:39 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

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
 * process_redirections_in_tokens - Apply redirections from token list
 * @cmd: Command to update with redirections
 * @tokens: Token list containing redirection tokens
 * Return: 0 on success, -1 on error
 */
int process_redirections_in_tokens(t_cmd *cmd, t_token *tokens)
{
	t_token *current;

	if (!cmd || !tokens)
		return (0);
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_REDIRECT_IN ||
			current->type == TOKEN_REDIRECT_OUT ||
			current->type == TOKEN_REDIRECT_APPEND)
		{
			if (handle_redirection(cmd, current, current->type) < 0)
				return (-1);
			current = current->next;
		}
		current = current->next;
	}
	return (0);
}

