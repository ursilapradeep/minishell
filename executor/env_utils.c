/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:19:16 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 14:19:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fill_env_node(t_env *new_node, char *env_str)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(env_str, '=');
	if (equal_sign)
	{
		key_len = equal_sign - env_str;
		new_node->key = ft_substr(env_str, 0, key_len);
		new_node->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		new_node->key = ft_strdup(env_str);
		new_node->value = NULL;
	}
	if (!new_node->key)
	{
		free(new_node->value);
		return (0);
	}
	return (1);
}

void	add_env_node(t_env **env_list, char *env_str)
{
	t_env	*new_node;
	t_env	*last;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	if (!fill_env_node(new_node, env_str))
	{
		free(new_node);
		return ;
	}
	new_node->next = NULL;
	if (!*env_list)
	{
		new_node->prev = NULL;
		*env_list = new_node;
		return ;
	}
	last = *env_list;
	while (last->next)
		last = last->next;
	last->next = new_node;
	new_node->prev = last;
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;
	char	*shlvl;
	char	*new_shlvl;
	int		level;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		add_env_node(&env_list, envp[i]);
		i++;
	}
	shlvl = get_env_value(env_list, "SHLVL");
	level = 1;
	if (shlvl)
		level = ft_atoi(shlvl) + 1;
	new_shlvl = ft_itoa(level);
	if (new_shlvl)
	{
		set_env_value(&env_list, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
	return (env_list);
}
