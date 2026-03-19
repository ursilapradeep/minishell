/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:39:10 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 16:45:22 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * create_cmd - Create a new command structure
 * Return: New t_cmd pointer, NULL on error
 */
static t_cmd *create_cmd(void)
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
static void add_cmd(t_cmd **head, t_cmd *new_cmd)
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
static int count_args(t_token *tokens)
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


