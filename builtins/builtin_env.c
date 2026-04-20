/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 22:26:24 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/11 18:15:02 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
