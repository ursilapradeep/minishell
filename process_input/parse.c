/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:33:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/11 12:32:54 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_pipe_syntax_error(char *input)
{
	int	index;
	int	left;
	int	right;

	index = 0;
	while (input[index])
	{
		if (input[index] == '|')
		{
			left = index - 1;
			while (left >= 0 && (input[left] == ' ' || input[left] == '\t'))
				left--;
			right = index + 1;
			while (input[right] == ' ' || input[right] == '\t')
				right++;
			if (left < 0 || input[right] == '\0'
				|| input[left] == '|' || input[right] == '|')
				return (1);
		}
		index++;
	}
	return (0);
}

static void	print_pipe_syntax_error(char *input)
{
	ft_putstr_fd("bash: line 1: syntax error near unexpected token `|'\n", 2);
	ft_putstr_fd("bash: line 1: `", 2);
	ft_putstr_fd(input, 2);
	ft_putstr_fd("'\n", 2);
}

static int	handle_cmd_build_failure(t_cmd *cmds, char *input)
{
	if (!cmds)
	{
		if (is_pipe_syntax_error(input))
			print_pipe_syntax_error(input);
		return (2);
	}
	return (0);
}

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
	if (handle_cmd_build_failure(cmds, input) != 0)
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

int	parse_input(char *input, t_env **my_env)
{
	return (execute_single_input(input, my_env));
}
