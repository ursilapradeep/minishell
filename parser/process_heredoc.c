/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 00:39:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	read_heredoc_lines(int write_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = read_non_interactive_line();
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\0')
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	return (0);
}

static int	read_heredoc_input(int write_fd, char *delimiter)
{
	read_heredoc_lines(write_fd, delimiter);
	return (0);
}

static int	create_heredoc_fd(char *delimiter)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	if (read_heredoc_input(pipefd[1], delimiter) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

static int	process_cmd_heredocs(t_cmd *cmd)
{
	int	fd;
	int	i;

	i = 0;
	while (i < cmd->heredoc_count)
	{
		fd = create_heredoc_fd(cmd->heredoc_delimiters[i]);
		if (fd < 0)
			return (-1);
		if (i == cmd->heredoc_count - 1)
		{
			if (cmd->infd > 2)
				close(cmd->infd);
			cmd->infd = fd;
		}
		else
			close(fd);
		i++;
	}
	return (0);
}

int	process_heredocs(t_cmd *cmds)
{
	t_cmd	*current;

	if (!cmds)
		return (0);
	current = cmds;
	while (current)
	{
		if (current->heredoc_count > 0 && process_cmd_heredocs(current) < 0)
			return (-1);
		current = current->next;
	}
	return (0);
}
