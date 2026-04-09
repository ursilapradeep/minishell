/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/09 09:44:10 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	read_heredoc_input(int write_fd, char *delimiter)
{
	char	*line;
	int		tty_fd;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd < 0)
	{
		perror("minishell: /dev/tty");
		return (-1);
	}
	rl_instream = fdopen(tty_fd, "r");
	if (!rl_instream)
	{
		close(tty_fd);
		perror("minishell: fdopen");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
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
	fclose(rl_instream);
	rl_instream = stdin;
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

int	process_heredocs(t_cmd *cmds)
{
	t_cmd	*current;
	int		fd;
	int		i;

	if (!cmds)
		return (0);
	current = cmds;
	while (current)
	{
		if (current->heredoc_count > 0)
		{
			i = 0;
			while (i < current->heredoc_count)
			{
				fd = create_heredoc_fd(current->heredoc_delimiters[i]);
				if (fd < 0)
					return (-1);
				if (i == current->heredoc_count - 1)
				{
					if (current->infd > 2)
						close(current->infd);
					current->infd = fd;
				}
				else
					close(fd);
				i++;
			}
		}
		current = current->next;
	}
	return (0);
}
