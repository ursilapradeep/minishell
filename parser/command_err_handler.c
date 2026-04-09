/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_err_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:34:54 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/09 12:30:20 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_cmd_resources(t_cmd *current)
{
	int		i;

	if (current->args)
		free_args(current->args);
	if (current->infd > 2)
		close(current->infd);
	if (current->outfd > 2)
		close(current->outfd);
	if (current->heredoc_delimiters)
	{
		i = 0;
		while (i < current->heredoc_count)
		{
			free(current->heredoc_delimiters[i]);
			i++;
		}
		free(current->heredoc_delimiters);
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		next = current->next;
		free_cmd_resources(current);
		free(current);
		current = next;
	}
}
