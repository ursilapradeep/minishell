/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_variable_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:05:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 13:38:33 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Test cases for variable expansion:
 * 
 * 1. "$HOME"                  - Basic variable expansion
 * 2. "${HOME}"                - Braced variable expansion
 * 3. "Path is $PATH"          - Variable in string
 * 4. "$UNDEFINED"             - Undefined variable (expands to empty)
 * 5. "echo 'hello $HOME'"     - Single quotes prevent expansion
 * 6. "echo \"hello $HOME\""   - Double quotes allow expansion
 * 7. "test$$var"             - Multiple variables
 */

/* Create a test environment with a few variables */
static t_env *create_test_env(void)
{
	t_env	*env;
	t_env	*node2;
	t_env	*node3;

	env = ft_calloc(1, sizeof(t_env));
	if (!env)
		return (NULL);
	
	env->key = ft_strdup("HOME");
	env->value = ft_strdup("/home/user");
	env->next = NULL;
	env->prev = NULL;
	
	node2 = ft_calloc(1, sizeof(t_env));
	if (!node2)
	{
		free(env->key);
		free(env->value);
		free(env);
		return (NULL);
	}
	node2->key = ft_strdup("USER");
	node2->value = ft_strdup("testuser");
	node2->next = NULL;
	node2->prev = env;
	env->next = node2;
	
	node3 = ft_calloc(1, sizeof(t_env));
	if (!node3)
	{
		free(node2->key);
		free(node2->value);
		free(node2);
		free(env->key);
		free(env->value);
		free(env);
		return (NULL);
	}
	node3->key = ft_strdup("SHELL");
	node3->value = ft_strdup("/bin/bash");
	node3->next = NULL;
	node3->prev = node2;
	node2->next = node3;
	
	return (env);
}

/* Free test environment */
static void free_test_env(t_env *env)
{
	t_env *current;
	t_env *next;

	if (!env)
		return ;
	current = env;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

int main(void)
{
	t_env	*test_env;
	char	*result;
	char	test_cases[][100] = {
		"$HOME",
		"${HOME}",
		"Path is $HOME and user is $USER",
		"$UNDEFINED",
		"My shell is $SHELL",
		"${HOME}/config",
		"$HOME/$USER"
	};
	int		i;

	printf("=== VARIABLE EXPANSION TEST ===\n\n");
	
	test_env = create_test_env();
	if (!test_env)
	{
		write(STDERR_FILENO, "Error: Failed to create test environment\n", 41);
		return (1);
	}
	
	printf("Test Environment:\n");
	printf("  HOME=%s\n", test_env->value);
	printf("  USER=%s\n", test_env->next->value);
	printf("  SHELL=%s\n", test_env->next->next->value);
	printf("\n");
	
	i = 0;
	while (i < 7)
	{
		printf("Test %d: Input: \"%s\"\n", i + 1, test_cases[i]);
		
		result = expand_string(test_cases[i], test_env);
		if (result)
		{
			printf("        Output: \"%s\"\n", result);
			free(result);
		}
		else
		{
			printf("        ✗ Failed to expand\n");
		}
		printf("\n");
		i++;
	}
	
	free_test_env(test_env);
	return (0);
}
