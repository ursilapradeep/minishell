/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:16:36 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/18 11:58:39 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Cleanup: free all already-allocated segments (indices 0 to idx-1) since we're aborting.*/
void	free_partial_pipeline(char **cmds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(cmds[i]);
		i++;
	}
	free(cmds);
}


int	add_segment(char **cmds, char *input, int start, int end, int idx)
{
	cmds[idx] = malloc(end - start + 1); //allocate memory ex: ls
	if (!cmds[idx])
		return (0);
	ft_strlcpy(cmds[idx], input + start, end - start + 1); //copy substring
	return (1);
}
/*Input: "ls | grep"*/
/*append_segment_or_fail(cmds, "ls | grep", 0, 2, 0)	.
result: Call add_segment to extract "ls"
2
add_segment() allocates 3 bytes, copies "ls" → returns 1	
Success*/
int	append_segment_or_fail(char **cmds, char *input, int start, int end,
		int idx)
{
	if (!add_segment(cmds, input, start, end, idx)) //Call add_segment() to allocate memory and copy substring
	{
		free_partial_pipeline(cmds, idx);
		return (-1);
	}
	return (0);
}
/*Function that splits input by | and fills cmds array. 
Returns 0 on success, -1 on failure. Sets *last_idx to index of last command.*/

int	split_pipe_segments(char **cmds, char *input, int *last_idx)
{
	int	i;
	int	j; // indx to place cmds in cmds array
	int	start;

	i = 0; 
	j = 0;
	start = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			/*Extract substring from start to i (not including the |), allocate it, copy it, and place in cmds[j]. 
					If allocation fails, cleanup and return -1.*/
			if (append_segment_or_fail(cmds, input, start, i, j) == -1)
				return (-1);// extract segment up to the |
			j++;
			start = i + 1; //move start PAST the |
		}
		i++;
	}
	if (append_segment_or_fail(cmds, input, start, i, j) == -1)
		return (-1); // Extract segment from start TO END
	*last_idx = j;
	return (0);
}

// Count pipes in input
/*Count | characters in input. 
Returns number_of_pipes + 1 (e.g., "ls | wc" has 1 pipe → 2 commands).*/
static int	count_pipes(char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			count++;
		i++;
	}
	return (count + 1); // Number of commands = pipes + 1
}
// Parse pipeline - split by '|'
/*Function that takes input string and returns array of command strings split by |.
*/
char	**parse_pipeline(char *input)
{
	char	**cmds; //pointer to array of strings --hold seperated commands
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

// Check if input contains pipe
int	contains_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|') /*check the current character is a pipe*/
			return (1); //pipe found return true
		i++;
	}
	return (0);
}
