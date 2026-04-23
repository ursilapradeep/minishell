/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:33:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/23 16:15:59 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	print_syntax_error_token(t_token *tokens, char *input)
{
	(void)input;
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR
		|| tokens->type == TOKEN_LPAREN || tokens->type == TOKEN_RPAREN
		|| tokens->type == TOKEN_SPECIAL)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		ft_putstr_fd(tokens->value, 2);
		ft_putstr_fd("'\n", 2);
		return (1);
	}
	return (0);
}

static int	check_consecutive_operators(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_AND || curr->type == TOKEN_OR
			|| curr->type == TOKEN_PIPE)
		{
			if (curr->next && (curr->next->type == TOKEN_AND
					|| curr->next->type == TOKEN_OR))
			{
				ft_putstr_fd("minishell: syntax error near"
					" unexpected token '", 2);
				ft_putstr_fd(curr->next->value, 2);
				ft_putstr_fd("'\n", 2);
				return (1);
			}
		}
		curr = curr->next;
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
		return (free_tokens(tokens), 1);
	if (print_syntax_error_token(tokens, input)
		|| check_consecutive_operators(tokens))
		return (free_tokens(tokens), 2);
	cmds = build_commands(tokens);
	free_tokens(tokens);
	if (!cmds)
		return (print_pipe_error(input), 2);
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
