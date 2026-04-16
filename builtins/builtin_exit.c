/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:02:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/11 18:15:48 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exit_numeric_error(char *arg)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static int	exit_validate_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (arg[i] == '\0')
	{
		exit_numeric_error(arg);
		exit(2);
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			exit_numeric_error(arg);
			exit(2);
		}
		i++;
	}
	return (0);
}

int	builtin_exit(char **args)
{
	int	exit_code;

	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!args[1])
		exit(get_last_status());
	exit_validate_arg(args[1]);
	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	exit(exit_code);
	return (-1);
}
