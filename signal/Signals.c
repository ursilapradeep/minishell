/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 10:59:49 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/09 12:38:01 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/signal.h>

/*
** Signal handler for SIGINT (Ctrl+C)
** In interactive mode: displays a new prompt on a new line
** Behavior matches bash
*/
void	signal_handler_sigint(int sig)
{
	(void)sig;
	write(1, "\nminishell$", 12);
	g_shell.last_status = 130;
	g_shell.sigint_received = 1;
}

/*
** Signal handler for SIGQUIT (Ctrl+\)
** In minishell: does nothing (unlike bash which exits)
** Just ignore itj
*/
void	signal_handler_sigquit(int sig)
{
	(void)sig;
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
	signal(SIGQUIT, signal_handler_sigquit);
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
