/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder_utils.c                            :+:+:      :+:+:+:    */
/*                                                    +#++#+    +#++#+       */
/*   By: spaipur- <spaipur-@student.42.fr>          +#++#+    +#++#+        */
/*                                                #######  ####### #######   */
/*   Created: 2026/03/19 16:21:40 by spaipur-          #+#    #+#    #+#     */
/*   Updated: 2026/04/11 22:53:00 by spaipur-         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*build_single_cmd(t_token **tokens);
void	add_cmd(t_cmd **head, t_cmd *new_cmd);

t_token	*find_next_separator(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE
			|| tokens->type == TOKEN_AND
			|| tokens->type == TOKEN_OR)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

static int	process_operator_token(t_token **curr, t_cmd **last_cmd,
		int *prev_had_pipe)
{
	if (!*curr || ((*curr)->type != TOKEN_AND && (*curr)->type != TOKEN_OR))
		return (0);
	if (*last_cmd)
	{
		if ((*curr)->type == TOKEN_AND)
			(*last_cmd)->next_op = OP_AND;
		else if ((*curr)->type == TOKEN_OR)
			(*last_cmd)->next_op = OP_OR;
	}
	*prev_had_pipe = 0;
	*curr = (*curr)->next;
	return (1);
}

static int	process_pipe_token(t_token **curr, int *prev_had_pipe)
{
	if (*curr && (*curr)->type == TOKEN_PIPE)
	{
		if (*prev_had_pipe)
			return (*prev_had_pipe = -1, 1);
		*prev_had_pipe = 1;
		*curr = (*curr)->next;
		return (1);
	}
	return (0);
}

static int	process_new_cmd(t_cmd **new_cmd, t_cmd **commands,
		t_cmd **last_cmd, int *prev_had_pipe)
{
	if (!*new_cmd && *commands)
	{
		write(STDERR_FILENO, "Error: Failed to build cmd\n", 27);
		return (-1);
	}
	if (*new_cmd)
	{
		if (*prev_had_pipe)
		{
			if (!*last_cmd)
				return (-1);
			(*last_cmd)->has_pipe = 1;
		}
		*prev_had_pipe = 0;
		add_cmd(commands, *new_cmd);
		*last_cmd = *new_cmd;
	}
	else if (!*commands)
	{
		write(STDERR_FILENO, "Error: No valid command\n", 24);
		return (-1);
	}
	return (0);
}

int	process_tokens_into_commands(t_token *tokens, t_cmd **commands)
{
	t_cmd	*new_cmd;
	t_token	*curr;
	t_cmd	*last_cmd;
	int		prev_had_pipe;

	curr = tokens;
	last_cmd = NULL;
	prev_had_pipe = 0;
	while (curr)
	{
		if (process_operator_token(&curr, &last_cmd, &prev_had_pipe))
			continue ;
		if (process_pipe_token(&curr, &prev_had_pipe))
		{
			if (prev_had_pipe < 0 || !curr)
				return (-1);
			continue ;
		}
		new_cmd = build_single_cmd(&curr);
		if (process_new_cmd(&new_cmd, commands, &last_cmd, &prev_had_pipe) < 0)
			return (-1);
	}
	return (0);
}
