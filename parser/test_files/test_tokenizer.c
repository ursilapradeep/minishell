/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:20:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 10:20:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Test cases for tokenizer:
 * 
 * 1. "ls -la"                         - Basic command with args
 * 2. "echo hello world"               - Multiple arguments
 * 3. "cat file.txt > output.txt"      - Output redirection
 * 4. "grep pattern < input.txt"       - Input redirection
 * 5. "ls -la | grep test"             - Pipe
 * 6. "cat < in.txt | grep word > out" - Complex pipeline
 * 7. "echo 'hello world'"             - Single quoted string
 * 8. "echo \"hello world\""           - Double quoted string
 * 9. "echo hello >> log.txt"          - Append redirect
 */

int main(void)
{
	t_token	*tokens;
	char	test_cases[][100] = {
		"ls -la",
		"echo hello world",
		"cat file.txt > output.txt",
		"grep pattern < input.txt",
		"ls -la | grep test",
		"cat < in.txt | grep word > out.txt",
		"echo 'hello world'",
		"echo \"hello world\"",
		"echo hello >> log.txt"
	};
	int		i;

	printf("=== TOKENIZER TEST ===\n\n");
	
	i = 0;
	while (i < 9)
	{
		printf("Test %d: \"%s\"\n", i + 1, test_cases[i]);
		
		tokens = tokenize(test_cases[i]);
		if (tokens)
		{
			print_tokens(tokens);
			free_tokens(tokens);
		}
		else
		{
			printf("✗ Failed to tokenize\n");
		}
		printf("\n");
		i++;
	}
	
	return (0);
}
