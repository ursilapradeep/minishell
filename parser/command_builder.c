/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:21:40 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/13 13:07:28 by uvadakku         ###   ########.fr       */
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
	cmd->errfd = STDERR_FILENO;
	cmd->heredoc_delimiter = NULL;
	cmd->heredoc_delimiters = NULL;
	cmd->heredoc_count = 0;
	cmd->next_op = OP_NONE;
	cmd->has_pipe = 0;
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

int	process_heredocs(t_cmd *cmds, t_env *env)
{
	t_cmd	*current;

	if (!cmds)
		return (0);
	current = cmds;
	while (current)
	{
		if (current->heredoc_count > 0
			&& process_cmd_heredocs(current, env) < 0)
			return (-1);
		current = current->next;
	}
	return (0);
}
