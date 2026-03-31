/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_err_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:34:54 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/31 14:42:25 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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