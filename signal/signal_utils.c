/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:00:54 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/23 16:02:23 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_sigint_hook(void)
{
	if (g_signal == SIGINT)
	{
		rl_done = 1;
		rl_event_hook = NULL;
	}
	return (0);
}
