/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:38 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/18 13:13:03 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* input: echo hi >   out.txt
suppose caller has i at position right after >
function does:
	skips spaces (*i moves to o)
	marks start = *i
	advances until space/operator/end
	returns substring "out.txt" */
static char	*skip_spaces_and_extract(char *input, int *i)
{
	char	*token; // Buffer to store extracted token
	int		start; // Mark start position of token

	while (input[*i] == ' ' || input[*i] == '\t') // Skip whitespace characters
		(*i)++;
	start = *i; // Mark where token begins
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t' // Continue while not whitespace or operator
		&& input[*i] != '>' && input[*i] != '<') // Stop at redirection operators
		(*i)++; // Move to next character
	token = ft_substr(input, start, *i - start); // Extract substring as token
	return (token); // Return extracted token
}

// Determine operator type and execute appropriate handler
static int	execute_redirection(int op_type, char *target)
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

// Identify redirection operator type and advance index
/*cat > infile.txt
> redirects stdout to file
cat reads from terminal (stdin) and writes into infile.txt
file is overwritten (or created)
cat < infile.txt
< redirects file to stdin
cat reads from infile.txt and writes to terminal stdout*/
static int	identify_and_skip_operator(char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>') // Check for >>
	{
		*i += 2; // Skip both characters
		return (0); //(write to file in append mode (keep old content), not overwrite)
	}
	else if (input[*i] == '>') 
	{
		*i += 1; 
		return (1); // Return type 1 (output)
	}
	else if (input[*i] == '<' && input[*i + 1] == '<') // Check for << cat << EOF
	{
		*i += 2; // Skip both characters
		return (2); // Return type 2 (shell reads input lines from terminal until delimiter word appears heredoc)
	}
	else if (input[*i] == '<') // Check for <
	{
		*i += 1; 
		return (3); // Return type 3 (input)
	}
	return (-1); // Return error if not an operator
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

// Parse and apply redirections from command string
// Returns the cleaned command (without redirection tokens)
char	*apply_redirections(char *input)
{
	int		i; // Index to traverse input
	int		start; // Mark start of current command segment
	char	*cmd; // Store result command

	i = 0; // Initialize index
	start = 0; // Initialize start position
	while (input[i]) // Loop through entire input string
	{
		if ((input[i] == '>' && input[i + 1] == '>') // Check for >>
			|| input[i] == '>' // Check for >
			|| (input[i] == '<' && input[i + 1] == '<') // Check for <<
			|| input[i] == '<') // Check for <
		{
			cmd = process_one_redirection(input, &i, &start); // Process found redirection
			if (cmd == NULL) // Check for error
				return (NULL); // Return NULL on error
			return (cmd); // Return cleaned command
		}
		i++; // Move to next character
	}
	return (ft_strdup(input)); // Return duplicate of input if no redirections found
}