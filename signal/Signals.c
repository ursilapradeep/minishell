/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 10:59:49 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/15 18:25:49 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*
** Signal handler for SIGINT (Ctrl+C)
** In interactive mode: displays a new prompt on a new line
** Behavior matches bash
*/
void	signal_handler_sigint(int sig)
{
	int	was_in_heredoc;

	was_in_heredoc = 0;
	if (g_signal < 0)
		was_in_heredoc = 1;
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	if (was_in_heredoc)
	{
		rl_replace_line("", 0);
		rl_done = 1;
		return ;
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/*
** Initialize signal handlers for interactive mode
** Setup:
** - SIGINT (Ctrl+C): display new prompt on new line
** - SIGQUIT (Ctrl+\): do nothing
** - SIGTERM: default behavior
*/
void	setup_signal_handlers(void)
{
	signal(SIGINT, signal_handler_sigint);
	signal(SIGQUIT, SIG_IGN); 
}

/*
** Ignore signals in child processes (after fork)
** Child processes should use default signal handling
*/
void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/*
** Restore default signal handling
** Used when we need to let child inherit standard behavior
*/
void	restore_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
