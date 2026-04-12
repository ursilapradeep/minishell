/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 01:50:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 02:16:18 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static int	key_cmp(t_env *left, t_env *right)
{
	int	cmp_len;

	cmp_len = ft_strlen(left->key) + ft_strlen(right->key) + 1;
	return (ft_strncmp(left->key, right->key, cmp_len));
}

static void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (key_cmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_entry(t_env *entry)
{
	if (ft_strncmp(entry->key, "_", 2) == 0)
		return ;
	if (entry->value)
		printf("declare -x %s=\"%s\"\n", entry->key, entry->value);
	else
		printf("declare -x %s\n", entry->key);
}

void	print_sorted_export(t_env *env)
{
	t_env	**arr;
	int		count;
	int		i;

	count = env_count(env);
	arr = ft_calloc(count, sizeof(t_env *));
	if (!arr)
		return ;
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	sort_env_array(arr, count);
	i = 0;
	while (i < count)
	{
		print_export_entry(arr[i]);
		i++;
	}
	free(arr);
}
