/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:38 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/31 20:35:51 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// Determine operator type and execute appropriate handler
int	execute_redirection(int op_type, char *target)
{
	if (op_type == 0) // Append redirection >>
		return (handle_append_redirect(target)); // Call append handler
	else if (op_type == 1) // Output redirection >
		return (handle_output_redirect(target)); // Call output handler
	else if (op_type == 2) // Heredoc <<
		return (handle_heredoc(target)); // Call heredoc handler
	else if (op_type == 3) // Input redirection <
		return (handle_input_redirect(target)); // Call input handler
	return (-1); // Return error if unknown operator
}

static char	*process_one_redirection(char *input, int *i, int *start)
{
	char	*cmd; // Store command before redirection
	char	*target; // Store filename/delimiter for redirection
	int		op_type; // Type of redirection operator

	cmd = ft_substr(input, *start, *i - *start); // Extract command part before operator
	op_type = identify_and_skip_operator(input, i); // Identify operator and advance index
	target = skip_spaces_and_extract(input, i); // Extract filename/delimiter after operator
	if (execute_redirection(op_type, target) == -1) // Execute the redirection
	{
		free(target); // Free target string on error
		free(cmd); // Free command string on error
		return (NULL); // Return NULL to signal error
	}
	free(target); // Free target string on success
	return (cmd); // Return cleaned command
}
