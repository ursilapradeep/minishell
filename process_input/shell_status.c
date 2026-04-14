/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:00:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/14 00:00:00 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	*last_status_ptr(void)
{
	static int	last_status;

	return (&last_status);
}

int	get_last_status(void)
{
	return (*last_status_ptr());
}

void	set_last_status(int status)
{
	*last_status_ptr() = status;
}
