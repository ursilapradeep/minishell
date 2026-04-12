/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_pwd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:10:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/01 11:06:14 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(char **args)
{
	char	cwd[4096];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	return (0);
}

static int	is_n_flag(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[0] != '-')
		return (0);
	if (!arg[1])
		return (0);
	i++;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	unsplit_n_prefix_len(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	if (i == 1 || arg[i] != ' ')
		return (0);
	while (arg[i] == ' ')
		i++;
	if (!arg[i])
		return (0);
	return (i);
}

static void	print_echo_args(char **args, int i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;
	int	offset;

	i = 1;
	newline = 1;
	while (args[i] && is_n_flag(args[i]))
		i++;
	if (i > 1)
		newline = 0;
	offset = unsplit_n_prefix_len(args[i]);
	if (offset > 0)
	{
		newline = 0;
		if (args[i][offset])
			ft_putstr_fd(args[i] + offset, 1);
		if (args[i][offset] && args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	print_echo_args(args, i);
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
