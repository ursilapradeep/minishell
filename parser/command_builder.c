/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 14:18:48 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/**
 * free_args - Free argument array
 * @args: Array of strings to free
 */
void free_args(char **args)
{
	int i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

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
		if (current->type == TOKEN_WORD)
		{
			/* Check if previous token was a redirect */
			if (!current->prev || (current->prev->type != TOKEN_REDIRECT_IN 
				&& current->prev->type != TOKEN_REDIRECT_OUT 
				&& current->prev->type != TOKEN_REDIRECT_APPEND 
				&& current->prev->type != TOKEN_HEREDOC))
				count++;
		}
		else if (current->type == TOKEN_REDIRECT_IN 
			|| current->type == TOKEN_REDIRECT_OUT 
			|| current->type == TOKEN_REDIRECT_APPEND 
			|| current->type == TOKEN_HEREDOC)
		{
			skip_next = 1;
		}
		current = current->next;
	}
	return (count);
}

/**
 * build_args_array - Build argument array from tokens (excluding redirect filenames)
 * @tokens: Token list
 * @arg_count: Number of arguments to collect
 * Return: Newly allocated args array, NULL on error
 */
static char **build_args_array(t_token *tokens, int arg_count)
{
	char	**args;
	int		i;
	t_token *current;
	int		skip_next;

	if (arg_count == 0)
		return (NULL);
	args = ft_calloc(arg_count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	current = tokens;
	skip_next = 0;
	while (current && i < arg_count && current->type != TOKEN_PIPE)
	{
		if (skip_next)
		{
			skip_next = 0;
			current = current->next;
			continue ;
		}
		if (current->type == TOKEN_WORD)
		{
			/* Skip if previous token was a redirect */
			if (!current->prev || (current->prev->type != TOKEN_REDIRECT_IN 
				&& current->prev->type != TOKEN_REDIRECT_OUT 
				&& current->prev->type != TOKEN_REDIRECT_APPEND 
				&& current->prev->type != TOKEN_HEREDOC))
			{
				args[i] = ft_strdup(current->value);
				if (!args[i])
				{
					free_args(args);
					return (NULL);
				}
				i++;
			}
		}
		else if (current->type == TOKEN_REDIRECT_IN 
			|| current->type == TOKEN_REDIRECT_OUT 
			|| current->type == TOKEN_REDIRECT_APPEND 
			|| current->type == TOKEN_HEREDOC)
		{
			skip_next = 1;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

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
		return (NULL);
	
	/* Count arguments until pipe or end */
	arg_count = count_args(*tokens);
	
	/* Build arguments array */
	if (arg_count > 0)
	{
		cmd->args = build_args_array(*tokens, arg_count);
		if (!cmd->args && arg_count > 0)
		{
			free(cmd);
			return (NULL);
		}
	}
	
	/* Process redirections in this command */
	if (process_redirections_in_tokens(cmd, *tokens) < 0)
	{
		free_args(cmd->args);
		free(cmd);
		return (NULL);
	}
	
	/* Move to next command (skip to after pipe) */
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
		}
		/* Current command writes to pipe */
		if (current->outfd == STDOUT_FILENO)
			current->outfd = pipe_fds[1];
		else
			close(pipe_fds[1]);
		
		/* Next command reads from pipe */
		if (current->next->infd == STDIN_FILENO)
			current->next->infd = pipe_fds[0];
		else
			close(pipe_fds[0]);
		
		current = current->next;
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
	t_cmd	*new_cmd;
	t_token *current;

	if (!tokens)
		return (NULL);
	
	commands = NULL;
	current = tokens;
	
	/* Build each command up to pipes */
	while (current)
	{
		new_cmd = build_single_cmd(&current);
		if (!new_cmd && commands)
		{
			/* Error building command, but we have some */
			write(STDERR_FILENO, "Error: Failed to build command\n", 31);
			free_cmd_list(commands);
			return (NULL);
		}
		if (new_cmd)
			add_cmd(&commands, new_cmd);
		else if (!commands)
		{
			write(STDERR_FILENO, "Error: No valid command\n", 24);
			return (NULL);
		}
	}
	
	/* Connect pipes between commands */
	if (build_pipeline(commands) < 0)
	{
		free_cmd_list(commands);
		return (NULL);
	}
	
	return (commands);
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
	{
		printf("No commands\n");
		return ;
	}
	
	printf("=== COMMANDS ===\n");
	current = commands;
	cmd_num = 1;
	while (current)
	{
		printf("Command %d:\n", cmd_num);
		printf("  Args: ");
		if (current->args)
		{
			i = 0;
			while (current->args[i])
			{
				printf("[%s] ", current->args[i]);
				i++;
			}
		}
		else
			printf("(none)");
		printf("\n");
		printf("  Input FD: %d\n", current->infd);
		printf("  Output FD: %d\n", current->outfd);
		printf("\n");
		current = current->next;
		cmd_num++;
	}
	printf("================\n");
}
