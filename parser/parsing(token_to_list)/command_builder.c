/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:21:40 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 19:34:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * create_cmd - Create a new command structure
 * Return: New t_cmd pointer, NULL on error
 */
t_cmd *create_cmd(void)
{
	t_cmd *cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infd = STDIN_FILENO;
	cmd->outfd = STDOUT_FILENO;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

/**
 * add_cmd - Add command to end of linked list
 * @head: Pointer to list head
 * @new_cmd: Command to add
 */
void add_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd *current;

	if (!head || !new_cmd)
		return ;
	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
	new_cmd->prev = current;
}

/**
 * build_single_cmd - Build one command from tokens up to next pipe
 * @tokens: Pointer to token list (updated to point after pipe)
 * Return: New t_cmd structure, NULL on error
 */
t_cmd *build_single_cmd(t_token **tokens)
{
	t_cmd	*cmd;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = create_cmd();
	if (!cmd)
		return (NULL);
	if (initialize_cmd_arguments(cmd, tokens) < 0)
		return (NULL);
	return (cmd);
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
int process_tokens_into_commands(t_token *tokens, t_cmd **commands)
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




