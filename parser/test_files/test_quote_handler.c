/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_quote_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:05:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 14:02:42 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Test cases for quote handler (from pipex):
 * 
 * 1. "echo hello"              - Simple command
 * 2. "echo 'hello world'"      - Single quotes with space
 * 3. "echo \"hello world\""    - Double quotes with space
 * 4. "cat file.txt"            - Command with argument
 * 5. "echo 'it\\'s' test"     - Escaped quote in single quotes
 * 6. "echo \"$VAR\""           - Variable in double quotes (for later)
 * 7. "ls -la 'My File.txt'"   - Mixed args and quotes
 */

static void print_args(char **args)
{
	int i;

	if (!args)
	{
		printf("  (NULL)\n");
		return ;
	}
	i = 0;
	while (args[i])
	{
		printf("  [%d]: \"%s\"\n", i, args[i]);
		i++;
	}
}

int main(void)
{
	char	*test_cases[] = {
		"echo hello",
		"echo 'hello world'",
		"echo \"hello world\"",
		"cat file.txt",
		"echo test",
		"ls -la 'My File.txt'",
		"grep 'pattern' input.txt"
	};
	char	**args;
	int		i;

	printf("=== QUOTE HANDLER TEST (from pipex) ===\n\n");
	
	i = 0;
	while (i < 7)
	{
		printf("Test %d: \"%s\"\n", i + 1, test_cases[i]);
		
		args = parse_command_args(test_cases[i]);
		if (args)
		{
			printf("Parsed arguments:\n");
			print_args(args);
			free_args(args);
		}
		else
		{
			printf("  Failed to parse\n");
		}
		printf("\n");
		i++;
	}
	
	return (0);
}
