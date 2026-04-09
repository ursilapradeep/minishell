/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:20:17 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/09 11:02:55 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* input: echo hi > out.txt
suppose caller has i at position right after >
function does:
	skips spaces (*i moves to o)
	marks start = *i
	advances until space/operator/end
	returns substring "out.txt" */
char	*skip_spaces_and_extract(char *input, int *i)
{
	char	*token;
	int		start;

	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '>' && input[*i] != '<')
		(*i)++;
	if (*i == start)
		return (NULL);
	token = ft_substr(input, start, *i - start);
	return (token);
}

// Identify redirection operator type and advance index
/*Input: "echo hi >> out.txt" at > → detects >>, moves *i by 2, 
returns 0; caller then reads out.txt.
>> → returns 0, advances by 2
> → returns 1, advances by 1
<< → returns 2, advances by 2
< → returns 3, advances by 1
none → returns -1*/
int	identify_and_skip_operator(char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		*i += 2;
		return (0);
	}
	else if (input[*i] == '>')
	{
		*i += 1;
		return (1);
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		*i += 2;
		return (2);
	}
	else if (input[*i] == '<')
	{
		*i += 1;
		return (3);
	}
	return (-1);
}

/*Input cmd: "echo hi > out.txt" at i pointing to >

detects > and target out.txt
calls redirection execution (dup2 flow inside helpers)
removes " > out.txt" from cmd
result cmd becomes "echo hi" and returns 0*/
int	process_and_remove_redirection(char **cmd_ptr, int *i_ptr)
{
	t_redir_proc_ctx	ctx;

	ctx.cmd = *cmd_ptr;
	ctx.i = *i_ptr;
	ctx.op_pos = ctx.i;
	ctx.op_type = identify_and_skip_operator(ctx.cmd, &ctx.i);
	ctx.target = skip_spaces_and_extract(ctx.cmd, &ctx.i);
	if (execute_redirection(ctx.op_type, ctx.target) == -1)
	{
		free(ctx.target);
		free(ctx.cmd);
		*cmd_ptr = NULL;
		return (-1);
	}
	free(ctx.target);
	ctx.tmp = remove_substring(ctx.cmd, ctx.op_pos, ctx.i);
	free(ctx.cmd);
	*cmd_ptr = ctx.tmp;
	*i_ptr = ctx.op_pos;
	return (0);
}

/*Input: "echo hi > out.txt" → it applies > to redirect stdout to out.txt, 
removes redirection text from command, 
trims spaces, and returns "echo hi". */
char	*apply_redirections(char *input)
{
	int		i;
	char	*cmd;
	char	*tmp;

	i = 0;
	cmd = ft_strdup(input);
	while (cmd && cmd[i])
	{
		if ((cmd[i] == '>' && cmd[i + 1] == '>') || cmd[i] == '>'
			|| (cmd[i] == '<' && cmd[i + 1] == '<') || cmd[i] == '<')
		{
			if (process_and_remove_redirection(&cmd, &i) == -1)
				return (NULL);
			continue ;
		}
		i++;
	}
	tmp = ft_strtrim(cmd, " \t");
	free(cmd);
	return (tmp);
}

/*scans the string and return 1 if it find  > or < else none 0*/
int	contains_redirection(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<')
			return (1);
		i++;
	}
	return (0);
}
