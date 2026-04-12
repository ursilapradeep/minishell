/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils_I.c                       :::      ::::::::   */
/*                                                    :::      :::    :::   */
/*   By: spaipur- <spaipur-@student.42.fr>          :::      :::    :::   */
/*                                                :::      :::    :::   */
/*   Created: 2026/03/13 11:00:00 by spaipur-          :::      :::    :::   */
/*   Updated: 2026/04/11 19:30:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_variable(t_env *env, const char *var_name, int len)
{
	t_env	*current;

	if (!var_name || len <= 0 || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->key && (int)ft_strlen(current->key) == len
			&& ft_strncmp(current->key, var_name, len) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

static int	handle_tilde_plus(t_env *env, char *result, int *consumed)
{
	char	*value;
	int		value_len;

	value = get_env_variable(env, "PWD", 3);
	if (value)
	{
		value_len = ft_strlen(value);
		ft_strlcpy(result, value, value_len + 1);
		*consumed = 2;
		return (1);
	}
	return (0);
}

static int	handle_tilde_minus(t_env *env, char *result, int *consumed)
{
	char	*value;
	int		value_len;

	value = get_env_variable(env, "OLDPWD", 6);
	if (value)
	{
		value_len = ft_strlen(value);
		ft_strlcpy(result, value, value_len + 1);
		*consumed = 2;
		return (1);
	}
	return (0);
}

static int	handle_tilde_home(t_env *env, char *result, int *consumed)
{
	char	*home;
	int		value_len;

	home = get_env_variable(env, "HOME", 4);
	if (home)
	{
		value_len = ft_strlen(home);
		ft_strlcpy(result, home, value_len + 1);
		*consumed = 1;
		return (1);
	}
	return (0);
}

int	expand_tilde(const char *input, t_env *env, char *result, int *consumed)
{
	*consumed = 0;
	if (!input || input[0] != '~')
		return (0);
	if (input[1] == '+' && (input[2] == '/' || input[2] == '\0'))
		return (handle_tilde_plus(env, result, consumed));
	else if (input[1] == '-' && (input[2] == '/' || input[2] == '\0'))
		return (handle_tilde_minus(env, result, consumed));
	else if (input[1] == '/' || input[1] == '\0')
		return (handle_tilde_home(env, result, consumed));
	return (0);
}
