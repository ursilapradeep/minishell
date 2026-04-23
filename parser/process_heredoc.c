/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/23 16:02:01 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	write_heredoc_line(int write_fd, char *line, int do_expand,
		t_env *env)
{
	char	*expanded;

	expanded = line;
	if (do_expand)
	{
		expanded = expand_string(line, env);
		if (!expanded)
			expanded = ft_strdup("");
	}
	write(write_fd, expanded, ft_strlen(expanded));
	write(write_fd, "\n", 1);
	if (do_expand)
		free(expanded);
}

static char	*get_heredoc_line(void)
{
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		rl_event_hook = heredoc_sigint_hook;
		line = readline("> ");
		rl_event_hook = NULL;
		return (line);
	}
	return (read_non_interactive_line());
}

static int	read_heredoc_lines(int write_fd, char *delimiter,
		int do_expand, t_env *env)
{
	char	*line;

	while (1)
	{
		line = get_heredoc_line();
		if (g_signal == SIGINT)
			return (free(line), -1);
		if (!line || (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& line[ft_strlen(delimiter)] == '\0'))
			break ;
		write_heredoc_line(write_fd, line, do_expand, env);
		free(line);
	}
	if (line)
		return (free(line), 0);
	ft_putstr_fd("minishell: warning: here-document at line 1"
		" delimited by end-of-file (wanted '", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	return (0);
}

static int	create_heredoc_fd(char *delimiter, t_env *env)
{
	int	pipefd[2];
	int	do_expand;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	do_expand = 1;
	if (delimiter[0] == '\1')
	{
		do_expand = 0;
		delimiter++;
	}
	g_signal = -1;
	if (read_heredoc_lines(pipefd[1], delimiter, do_expand, env) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	g_signal = 0;
	close(pipefd[1]);
	return (pipefd[0]);
}

int	process_cmd_heredocs(t_cmd *cmd, t_env *env)
{
	int	fd;
	int	i;

	i = 0;
	while (i < cmd->heredoc_count)
	{
		fd = create_heredoc_fd(cmd->heredoc_delimiters[i], env);
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
