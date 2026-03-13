/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:08:13 by us                #+#    #+#             */
/*   Updated: 2026/03/13 14:20:02 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// Read heredoc input lines until delimiter and write to pipe
int	read_heredoc_input(int write_fd, char *delimiter)
{
	char	*line; // Buffer to store each input line

	while (1) // Loop indefinitely until delimiter is found
	{
		line = readline("> "); // Read one line from user with prompt
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) // Check if EOF or delimiter reached
		{
			free(line); // Free allocated memory for line
			break ; // Exit loop
		}
		write(write_fd, line, ft_strlen(line)); // Write line to pipe
		write(write_fd, "\n", 1); // Write newline character to pipe
		free(line); // Free allocated memory for line
	}
	return (0); // Return success
}

// Handle heredoc (<<)
int	handle_heredoc(char *delimiter)
{
	int		pipefd[2]; // Array to store pipe file descriptors [read, write]

	if (pipe(pipefd) == -1) // Create a pipe, check for error
	{
		perror("minishell: pipe"); // Print error message
		return (-1); // Return error code
	}
	if (read_heredoc_input(pipefd[1], delimiter) == -1) // Call helper to read heredoc input and write to pipe
	{
		close(pipefd[0]); // Close read end of pipe
		close(pipefd[1]); // Close write end of pipe
		return (-1); // Return error code
	}
	close(pipefd[1]); // Close write end of pipe (no more writing)
	if (dup2(pipefd[0], STDIN_FILENO) == -1) // Redirect stdin to pipe read end
	{
		perror("minishell: dup2"); // Print error if dup2 fails
		close(pipefd[0]); // Close read end of pipe
		return (-1); // Return error code
	}
	close(pipefd[0]); // Close read end of pipe (stdin now points to it)
	return (0); // Return success
}