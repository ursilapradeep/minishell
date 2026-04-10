/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:38 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/10 19:55:44 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_redirections(t_cmd *cmd)
{
	if (cmd->infd > 2)
	{
		dup2(cmd->infd, STDIN_FILENO);
		close(cmd->infd);
	}
	else if (cmd->infd == -2)
	{
		exit(1);
	}
	if (cmd->outfd > 2)
	{
		dup2(cmd->outfd, STDOUT_FILENO);
		close(cmd->outfd);
	}
	else if (cmd->outfd == -2)
	{
		exit(1);
	}
}
