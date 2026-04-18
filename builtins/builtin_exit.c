/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:02:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/18 22:00:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

static void	exit_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static void	exit_validate_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i] || !ft_isdigit(arg[i]))
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
}

int	builtin_exit(char **args)
{
	long long	exit_code;
	char		*end;

	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!args[1])
		exit(get_last_status());
	exit_validate_arg(args[1]);
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	errno = 0;
	exit_code = strtoll(args[1], &end, 10);
	if (errno == ERANGE)
	{
		exit_numeric_error(args[1]);
		exit(2);
	}
	exit((unsigned char)exit_code);
	return (-1);
}
