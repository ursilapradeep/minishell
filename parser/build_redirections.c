/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/10 20:04:54 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**copy_heredoc_delimiters(t_cmd *cmd, char *delimiter)
{
	char	**new_delimiters;
	int		i;

	new_delimiters = ft_calloc(cmd->heredoc_count + 2, sizeof(char *));
	if (!new_delimiters)
		return (NULL);
	i = 0;
	while (i < cmd->heredoc_count)
	{
		new_delimiters[i] = cmd->heredoc_delimiters[i];
		i++;
	}
	new_delimiters[cmd->heredoc_count] = ft_strdup(delimiter);
	if (!new_delimiters[cmd->heredoc_count])
	{
		free(new_delimiters);
		return (NULL);
	}
	new_delimiters[cmd->heredoc_count + 1] = NULL;
	return (new_delimiters);
}

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
	cmd->heredoc_delimiters = copy_heredoc_delimiters(cmd,
			current->next->value);
	if (!cmd->heredoc_delimiters)
		return (-1);
	cmd->heredoc_count++;
	cmd->heredoc_delimiter = cmd->heredoc_delimiters[cmd->heredoc_count - 1];
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
	if ((type == TOKEN_REDIRECT_IN && cmd->infd == -2)
		|| (type != TOKEN_REDIRECT_IN && cmd->outfd == -2))
		return (1);
	fd = open_redirection_file(cmd, filename, type);
	return (process_file_fd(cmd, fd, type));
}
