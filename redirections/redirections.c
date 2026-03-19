/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:20:17 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/13 14:21:17 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

// Handle output redirection (>)
int	handle_output_redirect(char *filename)
{
	int	fd; // File descriptor for output file

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open file for writing, create if not exists, truncate if exists
	if (fd == -1) // Check if open failed
	{
		perror("minishell: output redirect"); // Print error message
		return (-1); // Return error code
	}
	if (dup2(fd, STDOUT_FILENO) == -1) // Redirect stdout to file descriptor
	{
		perror("minishell: dup2"); // Print error if dup2 fails
		close(fd); // Close file descriptor
		return (-1); // Return error code
	}
	close(fd); // Close file descriptor (stdout now points to file)
	return (0); // Return success
}

// Handle append redirection (>>)
int	handle_append_redirect(char *filename)
{
	int	fd; // File descriptor for append file

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644); // Open file for appending, create if not exists
	if (fd == -1) // Check if open failed
	{
		perror("minishell: append redirect"); // Print error message
		return (-1); // Return error code
	}
	if (dup2(fd, STDOUT_FILENO) == -1) // Redirect stdout to file descriptor
	{
		perror("minishell: dup2"); // Print error if dup2 fails
		close(fd); // Close file descriptor
		return (-1); // Return error code
	}
	close(fd); // Close file descriptor (stdout now points to file)
	return (0); // Return success
}

// Handle input redirection (<)
int	handle_input_redirect(char *filename)
{
	int	fd; // File descriptor for input file

	fd = open(filename, O_RDONLY); // Open file for reading only
	if (fd == -1) // Check if open failed
	{
		perror("minishell: input redirect"); // Print error message
		return (-1); // Return error code
	}
	if (dup2(fd, STDIN_FILENO) == -1) // Redirect stdin to file descriptor
	{
		perror("minishell: dup2"); // Print error if dup2 fails
		close(fd); // Close file descriptor
		return (-1); // Return error code
	}
	close(fd); // Close file descriptor (stdin now points to file)
	return (0); // Return success
}

// Check if input contains redirections
int	contains_redirection(char *input)
{
	int	i; // Loop counter

	i = 0; // Initialize counter
	while (input[i]) // Loop through entire input string
	{
		if (input[i] == '>' || input[i] == '<') // Check for redirection operators
			return (1); // Return 1 (true) if found
		i++; // Move to next character
	}
	return (0); // Return 0 (false) if no redirections found
}
