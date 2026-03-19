/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:17 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/17 17:45:07 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int count_args(char *input)
{
    int count;
    int i; // Index to traverse input
    int end;

    count = 0;
    i = 0;
    while (input[i])
    {
        while (input[i] &&  is_space(input[i]))
            i++; // Skip leading spaces
        if (input[i])
        {
            count++; // Found an argument
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
static int add_word(char **args, char *input, int start, int end, int index)
{
	int	len;

    len = word_len_without_quotes(input, start, end);
	args[index] = malloc(len + 1); 
	if (!args[index])
    {
        free_args(args); // Free previously allocated memory on failure
        return (0);
    }
    copy_without_quotes(args[index], input, start, end);
	return (1);
}

static int fill_args(char **args, char *input)
{
	int	i;
	int	j;
    int	end;

	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++; // Skip leading spaces
		if (input[i])
		{
            end = word_end(input, i);
            if (!add_word(args, input, i, end, j)) // Add word to args array
				return (0);
            i = end;
			j++;
		}
        
	}
	args[j] = NULL;
	return (1);
}

char **split_args(char *input)// Split input into arguments
{
    char **args;
    char *copy;
    int count;

    if (!input)
        return (NULL);
    copy = ft_strdup(input); // Duplicate input to avoid modifying original string
    if (!copy)
        return (NULL);
    count = count_args(copy); // Count number of arguments to allocate array
    args = malloc((sizeof(char *) * (count + 1))); // +1 for NULL terminator
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
