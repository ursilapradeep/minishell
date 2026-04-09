/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_err_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:34:54 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/09 09:27:48 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;
	int		i;

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
		free(current);
		current = next;
	}
}
