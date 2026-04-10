/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_unset.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:18:00 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/10 22:49:15 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	// First character must be letter or underscore
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	// Rest must be letters, digits, or underscores
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_one_arg(char *arg, t_env **env)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	int		key_len;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key_len = equal_sign - arg;
		// Handle += syntax (key+= value)
		if (key_len > 0 && arg[key_len - 1] == '+')
			key_len--;
		
		key = ft_substr(arg, 0, key_len);
		if (!key)
			return (1);
		// Validate that the key is a valid identifier
		if (!is_valid_identifier(key))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			free(key);
			return (1);
		}
		value = ft_strdup(equal_sign + 1);
		if (!value)
			return (free(key), 1);
		set_env_value(env, key, value);
		free(key);
		free(value);
	}
	else
	{
		// Validate that the arg itself is a valid identifier
		if (!is_valid_identifier(arg))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
		if (!get_env_value(*env, arg))
			set_env_value(env, arg, NULL);
	}
	return (0);
}

int	builtin_export(char **args, t_env **env)
{
	int		i;
	t_env	*temp;

	if (!args[1])
	{
		temp = *env;
		while (temp)
		{
			if (temp->value)
				printf("declare -x %s=\"%s\"\n", temp->key, temp->value);
			else
				printf("declare -x %s\n", temp->key);
			temp = temp->next;
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (export_one_arg(args[i], env) != 0)
			return (1);
		i++;
	}
	return (0);
}

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

int	builtin_unset(char **args, t_env **env)
{
	int		i;

	if (!args[1])
		return (0);
	i = 1;
	// Check for options (-f for function, -v for variable, -n for nameref)
	while (args[i] && args[i][0] == '-' && args[i][1] != '\0')
	{
		if (ft_strncmp(args[i], "-f", 3) == 0)
		{
			// unset -f (unset function) - not implemented, skip
			i++;
			continue;
		}
		else if (ft_strncmp(args[i], "-v", 3) == 0)
		{
			// unset -v (unset variable) - just skip the option
			i++;
			continue;
		}
		else if (ft_strncmp(args[i], "-n", 3) == 0)
		{
			// unset -n (unset nameref) - not implemented, skip
			i++;
			continue;
		}
		else
		{
			// Invalid option
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd(": invalid option\n", 2);
			ft_putstr_fd("unset: usage: unset [-f] [-v] [-n] [name ...]\n", 2);
			return (2);
		}
	}
	while (args[i])
	{
		unset_one_key(env, args[i]);
		i++;
	}
	return (0);
}
