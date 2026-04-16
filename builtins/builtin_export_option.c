/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_option.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 01:50:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 02:16:18 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_export_option_error(char *arg)
{
	if (arg[0] != '-' || arg[1] == '\0' || ft_strncmp(arg, "-p", 3) == 0)
		return (0);
	ft_putstr_fd("export: ", 2);
	if (arg[1] == '-')
		ft_putstr_fd("--", 2);
	else
	{
		ft_putchar_fd('-', 2);
		ft_putchar_fd(arg[1], 2);
	}
	ft_putstr_fd(": invalid option\n", 2);
	ft_putstr_fd("export: usage: export [-fn] [name[=value] ...] ", 2);
	ft_putstr_fd("or export -p\n", 2);
	return (1);
}
