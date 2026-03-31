/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:21:40 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/31 11:52:11 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infd = STDIN_FILENO;
	cmd->outfd = STDOUT_FILENO;
	cmd->heredoc_delimiter = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

void	add_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*cur;

	if (!head || !new_cmd)
		return ;
	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new_cmd;
	new_cmd->prev = cur;
}

t_cmd	*build_single_cmd(t_token **tokens)
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

int	process_tokens_into_commands(t_token *tokens, t_cmd **commands)
{
	t_cmd		*new_cmd;
	t_token		*curr;

	curr = tokens;
	while (curr)
	{
		new_cmd = build_single_cmd(&curr);
		if (!new_cmd && *commands)
		{
			write(STDERR_FILENO, "Error: Failed to build cmd\n", 27);
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

t_cmd	*build_commands(t_token *tokens)
{
	t_cmd	*commands;

	if (!tokens)
		return (NULL);
	commands = NULL;
	if (process_tokens_into_commands(tokens, &commands) < 0)
		return (NULL);
	if (build_pipeline(commands) < 0)
		return (free_cmd_list(commands), NULL);
	return (commands);
}
