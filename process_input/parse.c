/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:33:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/09 18:03:43 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	execute_single_input(char *input, t_env **my_env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	tokens = tokenize(input);
	if (!tokens)
		return (2);
	if (expand_token_list(tokens, *my_env) < 0)
	{
		free_tokens(tokens);
		return (2);
	}
	cmds = build_commands(tokens);
	free_tokens(tokens);
	if (!cmds)
		return (2);
	if (process_heredocs(cmds) < 0)
	{
		free_cmd_list(cmds);
		return (2);
	}
	status = execute_commands(cmds, my_env);
	free_cmd_list(cmds);
	return (status);
}

static int	find_next_and_operator(const char *input, int start)
{
	int			i;
	char		quote;

	i = start;
	quote = '\0';
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '"'))
		{
			if (quote == '\0')
				quote = input[i];
			else if (quote == input[i])
				quote = '\0';
		}
		else if (quote == '\0' && input[i] == '&' && input[i + 1] == '&')
			return (i);
		i++;
	}
	return (-1);
}

static int	process_input_segment(const char *input, int start, int end,
		t_env **my_env)
{
	char	*segment;
	char	*trimmed;
	int		status;

	if (end == -1)
		segment = ft_substr(input, start, ft_strlen(input) - start);
	else
		segment = ft_substr(input, start, end - start);
	if (!segment)
		return (2);
	trimmed = ft_strtrim(segment, " \t\n");
	free(segment);
	if (!trimmed)
		return (2);
	status = 0;
	if (*trimmed)
		status = execute_single_input(trimmed, my_env);
	free(trimmed);
	return (status);
}

int	parse_input(char *input, t_env **my_env)
{
	int		status;
	int		start;
	int		end;

	status = 0;
	start = 0;
	while (input[start])
	{
		end = find_next_and_operator(input, start);
		status = process_input_segment(input, start, end, my_env);
		if (status != 0 || end == -1)
			break ;
		start = end + 2;
	}
	return (status);
}
