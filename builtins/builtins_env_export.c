/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:16:33 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/20 13:33:36 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_export_option_error(char *arg)
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

int	builtin_export(char **args, t_env **env)
{
	int		i;
	int		ret;
	char	*equal_sign;

	if (!args[1])
		return (print_sorted_export(*env), 0);
	i = 0;
	ret = 0;
	while (args[++i])
	{
		if (handle_export_option_error(args[i]))
			return (2);
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign && export_with_equal(args[i], env))
			ret = 1;
		else if (!equal_sign && export_without_equal(args[i], env))
			ret = 1;
	}
	return (ret);
}

int	builtin_env(char **args, t_env *env)
{
	t_env	*current;

	if (!args[1])
	{
		current = env;
		while (current)
		{
			if (current->key && current->value)
				printf("%s=%s\n", current->key, current->value);
			current = current->next;
		}
		return (0);
	}
	return (1);
}