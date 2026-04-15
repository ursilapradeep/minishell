/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:33:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/15 04:52:33 by spaipur-         ###   ########.fr       */
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
	(void)input;
	ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
}

static int	print_logical_syntax_error(t_token *tokens, char *input)
{
	(void)input;
	if (!tokens)
		return (0);
	if (tokens->type != TOKEN_AND && tokens->type != TOKEN_OR)
		return (0);
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(tokens->value, 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

static int	execute_single_input(char *input, t_env **my_env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	tokens = tokenize(input);
	if (!tokens || expand_token_list(tokens, *my_env) < 0)
		return (free_tokens(tokens), 2);
	if (print_logical_syntax_error(tokens, input))
		return (free_tokens(tokens), 2);
	cmds = build_commands(tokens);
	free_tokens(tokens);
	if (!cmds)
	{
		if (is_pipe_syntax_error(input))
			print_pipe_syntax_error(input);
		return (2);
	}
	if (process_heredocs(cmds, *my_env) < 0)
		return (free_cmd_list(cmds), 2 + (128 * (g_signal == SIGINT)));
	status = execute_commands(cmds, my_env);
	free_cmd_list(cmds);
	return (status);
}

int	parse_input(char *input, t_env **my_env)
{
	if (!input || !*input)
		return (0);
	return (execute_single_input(input, my_env));
}
