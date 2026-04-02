/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:17 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 17:58:59 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

int	count_args_from_input(char *input)
{
	int	count;
	int	i;
	int	end;

	count = 0;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i])
		{
			count++;
			end = word_end(input, i);
			i = end;
		}
	}
	return (count);
}
/*Input string: "ls  -la /tmp"
    Process:
    Skip spaces → find "ls" → copy into args[0]
    Skip spaces → find "-la" → copy into args[1]
    Skip spaces → find "/tmp" → copy into args[2]
    Set args[3] = NULL */

static int	add_word(char **args, t_word_info *info, int index)
{
	int	len;

	len = word_len_without_quotes(info->input, info->start, info->end);
	args[index] = malloc(len + 1);
	if (!args[index])
	{
		free_args(args);
		return (0);
	}
	copy_without_quotes(args[index], info->input, info->start, info->end);
	return (1);
}

static int	fill_args(char **args, char *input)
{
	int			i;
	int			j;
	int			end;
	t_word_info	info;

	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i])
		{
			end = word_end(input, i);
			info.input = input;
			info.start = i;
			info.end = end;
			if (!add_word(args, &info, j))
				return (0);
			i = end;
			j++;
		}
	}
	args[j] = NULL;
	return (1);
}

char	**split_args(char *input)
{
	char	**args;
	char	*copy;
	int		count;

	if (!input)
		return (NULL);
	copy = ft_strdup(input);
	if (!copy)
		return (NULL);
	count = count_args_from_input(copy);
	args = malloc((sizeof(char *) * (count + 1)));
	if (!args)
	{
		free(copy);
		return (NULL);
	}
	if (!fill_args(args, copy))
	{
		free(copy);
		free_args(args);
		return (NULL);
	}
	free(copy);
	return (args);
}
