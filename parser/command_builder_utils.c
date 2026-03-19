/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:21:40 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 17:02:41 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/**
 * build_commands - Convert token list into command structures
 * @tokens: Token list from tokenizer
 * Return: Linked list of t_cmd structures, NULL on error
 *
 * This is the main command builder function.
 * It handles:
 * - Grouping tokens by pipes
 * - Building argument arrays
 * - Processing redirections
 * - Connecting pipes between commands
 */
t_cmd *build_commands(t_token *tokens)
{
	t_cmd	*commands;

	if (!tokens)
		return (NULL);
	commands = NULL;
	if (process_tokens_into_commands(tokens, &commands) < 0)
		return (NULL); /* Connect pipes between commands */
	if (build_pipeline(commands) < 0)
		return (free_cmd_list(commands), NULL);
	return (commands);
}

/**
 * process_tokens_into_commands - Helper function to build commands from tokens
 * @tokens: Token list from tokenizer
 * @commands: Pointer to command list head
 * Return: 0 on success, -1 on error
 *
 * This function processes the token list and builds individual commands.
 * It updates the commands linked list with new command structures.
 */
static int process_tokens_into_commands(t_token *tokens, t_cmd **commands)
{
	t_cmd *new_cmd;
	t_token *current;

	current = tokens;
	while (current)
	{
		new_cmd = build_single_cmd(&current);
		if (!new_cmd && *commands)
		{/* Error building command, but we have some */
			write(STDERR_FILENO, "Error: Failed to build command\n", 31);
			return (-1);
		}
		if (new_cmd)
			add_cmd(commands, new_cmd);
		else if (!*commands)
		{
			write(STDERR_FILENO, "Error: No valid command\n", 24);
			return (-1);
		}
	}
	return (0);
}

/**
 * free_cmd_list - Free entire command list
 * @cmd: Command list head
 */
void free_cmd_list(t_cmd *cmd)
{
	t_cmd *current;
	t_cmd *next;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_args(current->args);
		if (current->infd > 2)
			close(current->infd);
		if (current->outfd > 2)
			close(current->outfd);
		free(current);
		current = next;
	}
}

/**
 * print_commands - DEBUG: Print all commands and their settings
 * @commands: Command list to print
 */
void print_commands(t_cmd *commands)
{
	t_cmd *current;
	int	i;
	int cmd_num;

	if (!commands) 
		return ((void)printf("No commands\n"));
	current = commands;
	cmd_num = 1;
	while (current)
	{
		printf("Command %d:\n", cmd_num);
		if (current->args)
		{
			i = 0;
			while (current->args[i]) 
				printf("[%s] ", current->args[i++]);
		}
		else
			printf("(none)");
		printf("Input FD: %d\n  Output FD: %d\n\n", current->infd, current->outfd);
		current = current->next;
		cmd_num++;
	}
}
