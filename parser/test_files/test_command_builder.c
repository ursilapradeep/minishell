/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:35:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 14:07:05 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Test cases for command builder:
 * 
 * 1. "ls -la"                    - Simple command with args
 * 2. "cat file.txt > output.txt" - Command with output redirection
 * 3. "grep pattern < input.txt"  - Command with input redirection
 * 4. "ls -la | grep test"        - Simple pipeline
 * 5. "cat < in.txt | grep word > out.txt" - Complex pipeline with redirections
 */

int main(void)
{
	t_token	*tokens;
	t_cmd	*commands;
	char	test_cases[][100] = {
		"ls -la",
		"cat file.txt > output.txt",
		"grep pattern < input.txt",
		"ls -la | grep test",
		"cat < in.txt | grep word > out.txt"
	};
	int		i;

	printf("=== COMMAND BUILDER TEST ===\n\n");
	
	i = 0;
	while (i < 5)
	{
		printf("Test %d: \"%s\"\n", i + 1, test_cases[i]);
		printf("----------------------------------------\n");
		
		/* Tokenize */
		tokens = tokenize(test_cases[i]);
		if (!tokens)
		{
			printf("✗ Failed to tokenize\n\n");
			i++;
			continue ;
		}
		
		printf("Tokens:\n");
		print_tokens(tokens);
		printf("\n");
		
		/* Build commands */
		commands = build_commands(tokens);
		if (commands)
		{
			printf("Commands built:\n");
			print_commands(commands);
			free_cmd_list(commands);
		}
		else
		{
			printf("✗ Failed to build commands\n");
		}
		
		free_tokens(tokens);
		printf("\n");
		i++;
	}
	
	return (0);
}
