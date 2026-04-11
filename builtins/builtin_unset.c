/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:18:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/11 18:12:18 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	unset_one_key(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (current->key && ft_strncmp(current->key, key,
				ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static int	unset_handle_options(char **args, int i)
{
	if (ft_strncmp(args[i], "-f", 3) == 0)
		return (0);
	else if (ft_strncmp(args[i], "-v", 3) == 0)
		return (0);
	else if (ft_strncmp(args[i], "-n", 3) == 0)
		return (0);
	ft_putstr_fd("unset: ", 2);
	ft_putstr_fd(args[i], 2);
	ft_putstr_fd(": invalid option\n", 2);
	ft_putstr_fd("unset: usage: unset [-f] [-v] [-n] [name ...]\n", 2);
	return (2);
}

int	builtin_unset(char **args, t_env **env)
{
	int	i;
	int	ret;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] != '\0')
	{
		ret = unset_handle_options(args, i);
		if (ret != 0)
			return (ret);
		i++;
	}
	while (args[i])
	{
		unset_one_key(env, args[i]);
		i++;
	}
	return (0);
}
