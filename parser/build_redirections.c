/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 12:01:52 by spaipur-         ###   ########.fr       */
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

static int	get_target_fd(t_token *current)
{
	if (current->prev && current->prev->type == TOKEN_WORD
		&& current->prev != current
		&& ft_strncmp(current->prev->value, "2", 2) == 0)
		return (STDERR_FILENO);
	return (STDOUT_FILENO);
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
		perror("minishell");
	return (fd);
}

static int	handle_heredoc_token(t_cmd *cmd, t_token *current)
{
	char	*delimiter;

	if (!current->next || current->next->type != TOKEN_WORD)
	{
		write(STDERR_FILENO, "Error: No delimiter after <<", 29);
		return (-1);
	}
	if (current->next->quoted)
		delimiter = ft_strjoin("\1", current->next->value);
	else
		delimiter = ft_strdup(current->next->value);
	if (!delimiter)
		return (-1);
	cmd->heredoc_delimiters = copy_heredoc_delimiters(cmd, delimiter);
	free(delimiter);
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
	int		target_fd;

	if (type == TOKEN_HEREDOC)
		return (handle_heredoc_token(cmd, current));
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		write(STDERR_FILENO, "Error: No filename after redirect", 34);
		return (-1);
	}
	filename = current->next->value;
	target_fd = get_target_fd(current);
	if ((type == TOKEN_REDIRECT_IN && cmd->infd == -2)
		|| (type != TOKEN_REDIRECT_IN && target_fd == STDERR_FILENO
			&& cmd->errfd == -2)
		|| (type != TOKEN_REDIRECT_IN && target_fd != STDERR_FILENO
			&& cmd->outfd == -2))
		return (1);
	fd = open_redirection_file(cmd, filename, type, target_fd);
	return (process_file_fd(cmd, fd, type, target_fd));
}
