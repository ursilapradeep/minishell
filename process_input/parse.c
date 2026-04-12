/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:33:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/12 12:52:30 by spaipur-         ###   ########.fr       */
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

static int	execute_single_input(char *input, t_env **my_env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	tokens = tokenize(input);
	if (!tokens || expand_token_list(tokens, *my_env) < 0)
		return (free_tokens(tokens), 2);
	cmds = build_commands(tokens);
	free_tokens(tokens);
	if (!cmds)
	{
		if (is_pipe_syntax_error(input))
			print_pipe_syntax_error(input);
		return (2);
	}
	if (process_heredocs(cmds) < 0)
		return (free_cmd_list(cmds), 2);
	status = execute_commands(cmds, my_env);
	free_cmd_list(cmds);
	return (status);
}

static void	execute_segment(char **seg, char *current,
			t_env **my_env, int *status)
{
	*current = '\0';
	if (**seg)
		*status = execute_single_input(*seg, my_env);
	*seg = current + 1;
}

int	parse_input(char *input, t_env **my_env)
{
	int		quote[2];
	int		status;
	char	*seg;

	quote[0] = 0;
	quote[1] = 0;
	status = 0;
	seg = input;
	while (*input)
	{
		if (*input == '\'' && !quote[1])
			quote[0] = !quote[0];
		else if (*input == '"' && !quote[0])
			quote[1] = !quote[1];
		else if (*input == ';' && !quote[0] && !quote[1])
			execute_segment(&seg, input, my_env, &status);
		input++;
	}
	if (*seg)
		status = execute_single_input(seg, my_env);
	return (status);
}
