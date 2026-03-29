/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:38 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/27 17:11:01 by uvadakku         ###   ########.fr       */
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

// static char	*process_one_redirection(char *input, int *i, int *start)
// {
// 	char	*cmd; // Store command before redirection
// 	char	*target; // Store filename/delimiter for redirection
// 	int		op_type; // Type of redirection operator

// 	cmd = ft_substr(input, *start, *i - *start); // Extract command part before operator
// 	op_type = identify_and_skip_operator(input, i); // Identify operator and advance index
// 	target = skip_spaces_and_extract(input, i); // Extract filename/delimiter after operator
// 	if (execute_redirection(op_type, target) == -1) // Execute the redirection
// 	{
// 		free(target); // Free target string on error
// 		free(cmd); // Free command string on error
// 		return (NULL); // Return NULL to signal error
// 	}
// 	free(target); // Free target string on success
// 	return (cmd); // Return cleaned command
// }
// Parse and apply redirections from command string
// Returns the cleaned command (without redirection tokens)
// Helper: Remove a substring from input (from start to end, exclusive)
char *remove_substring(const char *input, int start, int end)
{
	char *result;
	int len = ft_strlen(input);
	int new_len = len - (end - start);
	result = malloc(new_len + 1);
	if (!result)
		return NULL;
	ft_memcpy(result, input, start);
	ft_memcpy(result + start, input + end, len - end);
	result[new_len] = '\0';
	return result;
}

// Helper: Process a single redirection at position i in cmd, update cmd and i by reference
static int process_and_remove_redirection(char **cmd_ptr, int *i_ptr)
{
	char *cmd = *cmd_ptr;
	int i = *i_ptr;
	int op_pos = i;
	int op_type = identify_and_skip_operator(cmd, &i);
	char *target = skip_spaces_and_extract(cmd, &i);
	char *tmp;
	if (execute_redirection(op_type, target) == -1)
	{
		free(target);
		free(cmd);
		*cmd_ptr = NULL;
		return -1;
	}
	free(target);
	tmp = remove_substring(cmd, op_pos, i);
	free(cmd);
	*cmd_ptr = tmp;
	*i_ptr = op_pos;
	return 0;
}

char *apply_redirections(char *input)
{
	int i = 0;
	char *cmd = ft_strdup(input);
	char *tmp;

	while (cmd && cmd[i])
	{
		if ((cmd[i] == '>' && cmd[i + 1] == '>') || cmd[i] == '>' ||
			(cmd[i] == '<' && cmd[i + 1] == '<') || cmd[i] == '<')
		{
			if (process_and_remove_redirection(&cmd, &i) == -1)
				return NULL;
			continue;
		}
		i++;
	}
	tmp = ft_strtrim(cmd, " \t");
	free(cmd);
	return tmp;
}
