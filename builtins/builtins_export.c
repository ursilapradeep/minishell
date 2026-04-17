/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:16:33 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/17 18:19:17 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
