/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 09:48:46 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Test cases for redirection handler:
 * 
 * 1. "ls > output.txt"            - Simple output redirect
 * 2. "cat < input.txt"            - Simple input redirect
 * 3. "echo hello >> log.txt"      - Append redirect
 * 4. "cat < in.txt > out.txt"     - Input and output
 * 5. "grep pattern file > result" - Command with redirect
 */

int main(void)
{
	t_cmd	cmd;
	char	test_cases[][100] = {
		"ls > output.txt",
		"cat < input.txt",
		"echo hello >> log.txt",
		"cat < in.txt > out.txt",
		"grep pattern file > result.txt"
	};
	int		i;

	printf("=== REDIRECTION HANDLER TEST ===\n\n");
	
	i = 0;
	while (i < 5)
	{
		printf("Test %d: \"%s\"\n", i + 1, test_cases[i]);
		
		// Initialize command
		cmd.args = NULL;
		cmd.infd = STDIN_FILENO;
		cmd.outfd = STDOUT_FILENO;
		cmd.next = NULL;
		cmd.prev = NULL;
		
		// Test redirection handling
		if (handle_redirections(&cmd, test_cases[i]) == 0)
		{
			printf("✓ Success\n");
			print_redirections(&cmd);
			
			// Clean up opened files
			if (cmd.infd > 2)
				close(cmd.infd);
			if (cmd.outfd > 2)
				close(cmd.outfd);
		}
		else
		{
			printf("✗ Failed\n");
		}
		printf("\n");
		i++;
	}
	
	return (0);
}
