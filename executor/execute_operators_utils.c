/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_operators_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:53:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/13 18:48:04 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	should_short_circuit(int status, t_operator op)
{
	if (op == OP_AND && status != 0)
		return (1);
	if (op == OP_OR && status == 0)
		return (1);
	return (0);
}

int	count_pipeline_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		if (!cmd->next)
			break ;
		if (!cmd->has_pipe)
			break ;
		cmd = cmd->next;
	}
	return (count);
}

t_cmd	*skip_pipeline(t_cmd *cmd)
{
	cmd = cmd->next;
	if (!cmd)
		return (NULL);
	while (cmd->has_pipe && cmd->next)
		cmd = cmd->next;
	if (cmd->next)
		return (cmd->next);
	return (NULL);
}
