/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:54:40 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 16:00:08 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Parse pipeline - split by '|'
/*Function that takes input string and
returns array of command strings split by |.
cmds -->pointer to array of strings --hold seperated commands*/
char	**parse_pipeline(char *input)
{
	char	**cmds;
	int		cmd_count;
	int		j;

	cmd_count = count_pipes(input);
	cmds = malloc(sizeof(char *) * (cmd_count + 1));
	if (!cmds)
		return (NULL);
	if (split_pipe_segments(cmds, input, &j) == -1)
		return (NULL);
	cmds[j + 1] = NULL;
	return (cmds);
}

int	handle_pipeline(char *input, t_env **my_env)
{
	char	**pipeline;
	int		status;

	pipeline = parse_pipeline(input);
	status = 1;
	if (pipeline)
	{
		status = execute_pipeline(pipeline, my_env);
		free_args(pipeline);
	}
	return (status);
}

/* Check if input contains pipe */
/*check the current character is a pipe*/
int	contains_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			return (1);
		i++;
	}
	return (0);
}
