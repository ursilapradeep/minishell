/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:08:13 by us                #+#    #+#             */
/*   Updated: 2026/04/09 12:44:24 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Read heredoc input lines until delimiter and write to pipe  
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

int	handle_heredoc(char *delimiter)
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
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}
