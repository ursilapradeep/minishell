/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_a.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 15:10:47 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 12:34:37 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Cleanup: free all already-allocated segments 
(indices 0 to idx-1) since we're aborting.*/
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

int	append_segment_or_fail(t_pipe_seg_ctx *ctx)
{
	ctx->cmds[ctx->idx] = malloc(ctx->end - ctx->start + 1);
	if (!ctx->cmds[ctx->idx])
	{
		free_partial_pipeline(ctx->cmds, ctx->idx);
		return (-1);
	}
	ft_strlcpy(ctx->cmds[ctx->idx], ctx->input + ctx->start,
		ctx->end - ctx->start + 1);
	return (0);
}

int	handle_pipe_delimiter(t_pipe_seg_ctx *ctx, int i, int *j)
{
	ctx->end = i;
	ctx->idx = *j;
	if (append_segment_or_fail(ctx) == -1)
		return (-1);
	(*j)++;
	ctx->start = i + 1;
	return (0);
}

/*Function that splits input by | and fills cmds array. 
Returns 0 on success, -1 on failure. 
Sets *last_idx to index of last command.
start = i + 1; move start PAST the |*/
int	split_pipe_segments(char **cmds, char *input, int *last_idx)
{
	int				i;
	int				j;
	t_pipe_seg_ctx	ctx;

	i = 0;
	j = 0;
	ctx.cmds = cmds;
	ctx.input = input;
	ctx.start = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (handle_pipe_delimiter(&ctx, i, &j) == -1)
				return (-1);
		}
		i++;
	}
	ctx.end = i;
	ctx.idx = j;
	if (append_segment_or_fail(&ctx) == -1)
		return (-1);
	*last_idx = j;
	return (0);
}

// Count pipes in input
/*Count | characters in input. 
Returns number_of_pipes + 1 (e.g., "ls | wc" has 1 pipe → 2 commands).*/
int	count_pipes(char *input)
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
	return (count + 1);
}
