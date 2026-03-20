/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                       
                         +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 11:04:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/20 11:04:11 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * process_redirections_in_tokens - Apply redirections from token list
 * @cmd: Command to update with redirections
 * @tokens: Token list containing redirection tokens
 * Return: 0 on success, -1 on error
 */
int process_redirections_in_tokens(t_cmd *cmd, t_token *tokens)
{
	t_token *current;

	if (!cmd || !tokens)
		return (0);
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_REDIRECT_IN ||
			current->type == TOKEN_REDIRECT_OUT ||
			current->type == TOKEN_REDIRECT_APPEND)
		{
			if (handle_redirection(cmd, current, current->type) < 0)
				return (-1);
			current = current->next;
		}
		current = current->next;
	}
	return (0);
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
 * initialize_cmd_arguments - Helper function to initialize command arguments
 * @cmd: Command to initialize
 * @tokens: Pointer to token list (updated to point after pipe)
 * Return: 0 on success, -1 on error
 */
int initialize_cmd_arguments(t_cmd *cmd, t_token **tokens)
{
	int arg_count;
	t_token *pipe_token;

	arg_count = count_args(*tokens); /* Count arguments until pipe or end */ /* Build arguments array */
	if (arg_count > 0)
	{
		cmd->args = build_args_array(*tokens, arg_count);
		if (!cmd->args && arg_count > 0)
			return (free(cmd), -1);
	}  /* Process redirections in this command */
	if (process_redirections_in_tokens(cmd, *tokens) < 0)
	{
		free_args(cmd->args);
		free(cmd);
		return (-1);
	} /* Move to next command (skip to after pipe) */
	pipe_token = find_next_pipe(*tokens);
	if (pipe_token)
		*tokens = pipe_token->next;
	else
		*tokens = NULL;
	return (0);
}

/**
 * build_pipeline - Connect commands in a pipeline with pipes
 * @commands: List of commands to connect
 * Return: 0 on success, -1 on error
 */
int build_pipeline(t_cmd *commands)
{
	t_cmd	*current;
	int		pipe_fds[2];

	if (!commands || !commands->next)
		return (0);
	current = commands;
	while (current && current->next)
	{
		if (pipe(pipe_fds) < 0)
		{
			perror("minishell");
			return (-1);
		}/* Current command writes to pipe */
		if (current->outfd == STDOUT_FILENO)
			current->outfd = pipe_fds[1];
		else
			close(pipe_fds[1]); /* Next command reads from pipe */
		if (current->next->infd == STDIN_FILENO)
			current->next->infd = pipe_fds[0];
		else
			close(pipe_fds[0]);
		current = current->next;
	}
	return (0);
}


