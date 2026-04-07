/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/07 10:46:39 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_redirection_file(t_cmd *cmd, char *filename, int type)
{
	int	fd;

	if (type == TOKEN_REDIRECT_IN)
	{
		if (cmd->infd > 2)
			close(cmd->infd);
		fd = open(filename, O_RDONLY);
	}
	else if (type == TOKEN_REDIRECT_OUT)
	{
		if (cmd->outfd > 2)
			close(cmd->outfd);
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (type == TOKEN_REDIRECT_APPEND)
	{
		if (cmd->outfd > 2)
			close(cmd->outfd);
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
		return (-1);
	if (fd < 0)
		perror("minishell");
	return (fd);
}

static int	handle_heredoc_token(t_cmd *cmd, t_token *current)
{
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		write(STDERR_FILENO, "Error: No delimiter after <<", 29);
		return (-1);
	}
	cmd->heredoc_delimiter = ft_strdup(current->next->value);
	return (1);
}

int	handle_redirection(t_cmd *cmd, t_token *current, int type)
{
	char	*filename;
	int		fd;

	if (type == TOKEN_HEREDOC)
		return (handle_heredoc_token(cmd, current));
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		write(STDERR_FILENO, "Error: No filename after redirect", 34);
		return (-1);
	}
	filename = current->next->value;
	fd = open_redirection_file(cmd, filename, type);
	if (fd < 0)
		return (-1);
	if (type == TOKEN_REDIRECT_IN)
		cmd->infd = fd;
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
