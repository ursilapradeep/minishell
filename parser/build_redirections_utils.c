/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirections_utils.c                         :::      :::    :::   */
/*                                                    :::      :::    :::   */
/*   By: spaipur- <spaipur-@student.42.fr>          :::      :::    :::   */
/*                                                :::      :::    :::   */
/*   Created: 2026/03/13 12:30:00 by spaipur-          :::      :::    :::   */
/*   Updated: 2026/04/11 18:37:08 by spaipur-         :::      :::    :::   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	set_error_fd(t_cmd *cmd, int type, int target_fd)
{
	if (type == TOKEN_REDIRECT_IN)
		cmd->infd = -2;
	else if (target_fd == STDERR_FILENO)
		cmd->errfd = -2;
	else
		cmd->outfd = -2;
	return (1);
}

int	process_file_fd(t_cmd *cmd, int fd, int type, int target_fd)
{
	if (fd < 0)
	{
		perror("minishell");
		return (set_error_fd(cmd, type, target_fd));
	}
	if (type == TOKEN_REDIRECT_IN)
		cmd->infd = fd;
	else if (target_fd == STDERR_FILENO)
		cmd->errfd = fd;
	else
		cmd->outfd = fd;
	return (1);
}

int	process_redirections_in_tokens(t_cmd *cmd, t_token *tokens)
{
	t_token	*curr;

	if (!cmd || !tokens)
		return (0);
	curr = tokens;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_REDIRECT_IN
			|| curr->type == TOKEN_REDIRECT_OUT
			|| curr->type == TOKEN_REDIRECT_APPEND
			|| curr->type == TOKEN_HEREDOC)
		{
			if (handle_redirection(cmd, curr, curr->type) < 0)
				return (-1);
			curr = curr->next;
		}
		curr = curr->next;
	}
	return (0);
}
