/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 19:28:23 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 19:50:12 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// Helper function to check if a token is a redirect
static int is_redirect_token(t_token *token)
{
	return (token->type == TOKEN_REDIRECT_IN ||
			token->type == TOKEN_REDIRECT_OUT ||
			token->type == TOKEN_REDIRECT_APPEND ||
			token->type == TOKEN_HEREDOC);
}

// Helper function to check and count a valid argument token
static int is_valid_argument(t_token *current)
{
    return (current->type == TOKEN_WORD && (!current->prev || !is_redirect_token(current->prev)));
}

/**
 * count_args - Count number of WORD tokens in a command (excluding redirect filenames)
 * @tokens: Token list
 * Return: Number of arguments
 */
int count_args(t_token *tokens)
{
	int		count;
	t_token *current;
	int		skip_next;

	if (!tokens)
		return (0);
	count = 0;
	current = tokens;
	skip_next = 0;
	while (current && current->type != TOKEN_PIPE)
	{
		if (skip_next)
		{
			skip_next = 0;
			current = current->next;
			continue ;
		}
		if (is_valid_argument(current))
			count++;
		else if (is_redirect_token(current))
			skip_next = 1;
		current = current->next;
	}
	return (count);
}