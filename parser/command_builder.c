/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 16:38:39 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>



/**
 * find_next_pipe - Find the next PIPE token
 * @tokens: Token list to search
 * Return: Pointer to PIPE token or NULL if not found
 */
static t_token *find_next_pipe(t_token *tokens)
{
	t_token *current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * process_redirections_in_tokens - Apply redirections from token list
 * @cmd: Command to update with redirections
 * @tokens: Token list containing redirection tokens
 * Return: 0 on success, -1 on error
 */
static int process_redirections_in_tokens(t_cmd *cmd, t_token *tokens)
{
	t_token *current;
	char	*filename;
	int		fd;

	if (!cmd || !tokens)
		return (0);
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_REDIRECT_IN)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				write(STDERR_FILENO, "Error: No filename after <\n", 27);
				return (-1);
			}
			filename = current->next->value;
			if (cmd->infd > 2)
				close(cmd->infd);
			fd = open(filename, O_RDONLY);
			if (fd < 0)
			{
				perror("minishell");
				return (-1);
			}
			cmd->infd = fd;
			current = current->next;
		}
		else if (current->type == TOKEN_REDIRECT_OUT)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				write(STDERR_FILENO, "Error: No filename after >\n", 27);
				return (-1);
			}
			filename = current->next->value;
			if (cmd->outfd > 2)
				close(cmd->outfd);
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("minishell");
				return (-1);
			}
			cmd->outfd = fd;
			current = current->next;
		}
		else if (current->type == TOKEN_REDIRECT_APPEND)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				write(STDERR_FILENO, "Error: No filename after >>\n", 28);
				return (-1);
			}
			filename = current->next->value;
			if (cmd->outfd > 2)
				close(cmd->outfd);
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror("minishell");
				return (-1);
			}
			cmd->outfd = fd;
			current = current->next;
		}
		else
			current = current->next;
	}
	return (0);
}

/**
 * build_single_cmd - Build one command from tokens up to next pipe
 * @tokens: Pointer to token list (updated to point after pipe)
 * Return: New t_cmd structure, NULL on error
 */
static t_cmd *build_single_cmd(t_token **tokens)
{
	t_cmd	*cmd;
	int		arg_count;
	t_token *pipe_token;

	if (!tokens || !*tokens)
		return (NULL);
	
	cmd = create_cmd();
	if (!cmd)
		return (NULL); /* Count arguments until pipe or end */
	arg_count = count_args(*tokens); /* Build arguments array */
	if (arg_count > 0)
	{
		cmd->args = build_args_array(*tokens, arg_count);
		if (!cmd->args && arg_count > 0)
		{
			free(cmd);
			return (NULL);
		}
	}  /* Process redirections in this command */
	if (process_redirections_in_tokens(cmd, *tokens) < 0)
	{
		free_args(cmd->args);
		free(cmd);
		return (NULL);
	} /* Move to next command (skip to after pipe) */
	pipe_token = find_next_pipe(*tokens);
	if (pipe_token)
		*tokens = pipe_token->next;
	else
		*tokens = NULL;
	return (cmd);
}

/**
 * build_pipeline - Connect commands in a pipeline with pipes
 * @commands: List of commands to connect
 * Return: 0 on success, -1 on error
 */
static int build_pipeline(t_cmd *commands)
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

