/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:08:13 by us                #+#    #+#             */
/*   Updated: 2026/04/01 19:02:25 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* cat << EOF
> hello
> world
> EOF
read_heredoc_input() collects:
- "hello" → write to pipe
- "world" → write to pipe
- "EOF" → stop (matches delimiter)*/
int	read_heredoc_input(int write_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
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

/*Example with cat << EOF:
handle_heredoc("EOF") creates a pipe:
	pipefd[1] = write end
	pipefd[0] = read end
	User input: > hello world EOF , 
	read_heredoc_input() writes "hello\nworld\n" to pipefd[1].
	Close write end, then dup2(pipefd[0], STDIN_FILENO).
	Now command cat reads from that pipe and prints: */
int	handle_heredoc(char *delimiter)
{
	int		pipefd[2];

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
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}

/*Command: echo hi > out.txt

Opens out.txt with truncate mode (O_TRUNC)
Redirects STDOUT to file
Result: out.txt contains only latest output (hi\n)*/
int	handle_output_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: output redirect");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_append_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell: append redirect");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*0 --> Append redirection >> // Call append handler
	1--> Output redirection > // Call output handler
	2--> Heredoc << // Call heredoc handler 
 3-->Input redirection < // Call input handler
	-1 unknown */
int	execute_redirection(int op_type, char *target)
{
	if (op_type == 0)
		return (handle_append_redirect(target));
	else if (op_type == 1)
		return (handle_output_redirect(target));
	else if (op_type == 2)
		return (handle_heredoc(target));
	else if (op_type == 3)
		return (handle_input_redirect(target));
	return (-1);
}
