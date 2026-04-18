/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_II.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:14:28 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/17 18:14:28 by uvadakku         ###   ########.fr       */
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
		ft_putstr_fd("minishell: export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	return (0);
}

static int	handle_append(t_export *exp, t_env *env)
{
	char	*existing;
	char	*combined;

	if (exp->is_append)
	{
		existing = get_env_value(env, exp->key);
		if (existing)
		{
			combined = ft_strjoin(existing, exp->value);
			free(exp->value);
			exp->value = combined;
			if (!exp->value)
				return (1);
		}
	}
	return (0);
}

int	export_with_equal(char *arg, t_env **env)
{
	t_export	exp;
	char		*eq;
	int			k_len;

	eq = ft_strchr(arg, '=');
	k_len = eq - arg;
	exp.is_append = (k_len > 0 && arg[k_len - 1] == '+');
	if (exp.is_append)
		k_len--;
	exp.key = ft_substr(arg, 0, k_len);
	if (!exp.key)
		return (1);
	if (export_validate_key(arg, exp.key))
		return (free(exp.key), 1);
	exp.value = ft_strdup(eq + 1);
	if (!exp.value || handle_append(&exp, *env))
		return (free(exp.key), free(exp.value), 1);
	set_env_value(env, exp.key, exp.value);
	return (free(exp.key), free(exp.value), 0);
}

int	export_without_equal(char *arg, t_env **env)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (!get_env_value(*env, arg))
		set_env_value(env, arg, NULL);
	return (0);
}
