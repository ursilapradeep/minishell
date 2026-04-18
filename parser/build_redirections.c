/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/15 04:52:33 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

static void	print_file_error(char *filename)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	open_redirection_file(t_cmd *cmd, char *filename, int type, int target_fd)
{
	int	fd;

	if (type == TOKEN_REDIRECT_IN)
	{
		if (cmd->infd > 2)
			close(cmd->infd);
		fd = open(filename, O_RDONLY);
	}
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND)
	{
		if (target_fd == STDERR_FILENO && cmd->errfd > 2)
			close(cmd->errfd);
		else if (cmd->outfd > 2)
			close(cmd->outfd);
		if (type == TOKEN_REDIRECT_OUT)
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	}
	else
		return (-1);
	if (fd < 0)
		print_file_error(filename);
	return (fd);
}

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
		return (set_error_fd(cmd, type, target_fd));
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
	while (curr && curr->type != TOKEN_PIPE
		&& curr->type != TOKEN_AND && curr->type != TOKEN_OR)
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
