/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 09:42:38 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 14:05:19 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <errno.h>
/**
 * handle_single_redirect - Handle one redirection operator
 * @cmd: Command structure to modify
 * @operator: Redirect operator (>, <, >>, <<)
 * @rest_of_line: Rest of command after operator
 * Return: Pointer to position after filename, NULL on error
 */
static int open_redirect_file(t_cmd *cmd, t_token_type operator, const char *filename)
{
	int fd;

	if (operator == TOKEN_REDIRECT_IN)
	{
		if (cmd->infd > 2)
			close(cmd->infd);
		fd = open_input_file(filename);
		if (fd < 0)
			return (-1);
		cmd->infd = fd;
	}
	else if (operator == TOKEN_REDIRECT_OUT || operator == TOKEN_REDIRECT_APPEND)
	{
		if (cmd->outfd > 2)
			close(cmd->outfd);
		fd = open_output_file(filename, operator == TOKEN_REDIRECT_APPEND);
		if (fd < 0)
			return (-1);
		cmd->outfd = fd;
	}
	return (0);
}

static const char *handle_heredoc(const char *rest_of_line)
{
	write(STDERR_FILENO, "Error: HEREDOC (<<) not yet implemented\n", 41);
	return (NULL);
}

static const char *handle_single_redirect(t_cmd *cmd, t_token_type operator,
	const char *rest_of_line)
{
	char *filename;
	const char *end_pos;

	if (!cmd || !rest_of_line)
		return (NULL);
	filename = extract_redirect_filename(rest_of_line);
	if (!filename)
		return (NULL);
	if (operator == TOKEN_HEREDOC)
	{
		free(filename);
		return handle_heredoc(rest_of_line);
	}
	if (open_redirect_file(cmd, operator, filename) < 0)
	{
		free(filename);
		return (NULL);
	}
	end_pos = skip_whitespace(rest_of_line);
	while (*end_pos && *end_pos != ' ' && *end_pos != '\t' && *end_pos != '|'
		&& *end_pos != '>' && *end_pos != '<')
		end_pos++;
	free(filename);
	return (end_pos);
}

static const char	*process_redirection(t_cmd *cmd, const char *current,
	t_token_type type, int *fd_count)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT ||
		type == TOKEN_REDIRECT_APPEND)
	{
		current += (type == TOKEN_REDIRECT_APPEND) ? 2 : 1;
		current = handle_single_redirect(cmd, type, current);
		if (current)
			(*fd_count)++;
		return (current);
	}
	else if (type == TOKEN_HEREDOC)
	{
		current += 2;
		write(STDERR_FILENO, "Error: HEREDOC not yet implemented\n", 35);
		return (NULL);
	}
	return (current);
}
