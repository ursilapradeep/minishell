/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:02:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/19 10:00:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	check_overflow(char *digits, int neg)
{
	int		len;
	char	*max;

	len = ft_strlen(digits);
	if (len > 19)
		return (1);
	if (len < 19)
		return (0);
	if (neg)
		max = "9223372036854775808";
	else
		max = "9223372036854775807";
	return (ft_strncmp(digits, max, 19) > 0);
}

static long long	parse_exit_code(char *arg, int *overflow)
{
	long long	result;
	int			neg;
	int			i;

	result = 0;
	neg = 0;
	i = 0;
	if (arg[0] == '-')
		neg = 1;
	if (arg[0] == '+' || arg[0] == '-')
		i = 1;
	*overflow = check_overflow(arg + i, neg);
	if (*overflow)
		return (0);
	while (arg[i])
		result = result * 10 + (arg[i++] - '0');
	if (neg)
		return (-result);
	return (result);
}

int	builtin_exit(char **args)
{
	long long	exit_code;
	int			overflow;

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
	exit_code = parse_exit_code(args[1], &overflow);
	if (overflow)
	{
		exit_numeric_error(args[1]);
		exit(2);
	}
	exit((unsigned char)exit_code);
	return (-1);
}
