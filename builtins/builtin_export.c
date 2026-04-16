/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:18:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/11 18:12:58 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_validate_key(char *arg, char *key)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	return (0);
}

static int	export_with_equal(char *arg, t_env **env)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	int		key_len;
	int		is_append;
	char	*existing;
	char	*combined;

	equal_sign = ft_strchr(arg, '=');
	key_len = equal_sign - arg;
	is_append = (key_len > 0 && arg[key_len - 1] == '+');
	if (is_append)
		key_len--;
	key = ft_substr(arg, 0, key_len);
	if (!key)
		return (1);
	if (export_validate_key(arg, key))
		return (free(key), 1);
	value = ft_strdup(equal_sign + 1);
	if (!value)
		return (free(key), 1);
	if (is_append)
	{
		existing = get_env_value(*env, key);
		if (existing)
		{
			combined = ft_strjoin(existing, value);
			free(value);
			value = combined;
			if (!value)
				return (free(key), 1);
		}
	}
	set_env_value(env, key, value);
	free(key);
	free(value);
	return (0);
}

static int	export_without_equal(char *arg, t_env **env)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (!get_env_value(*env, arg))
		set_env_value(env, arg, NULL);
	return (0);
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
