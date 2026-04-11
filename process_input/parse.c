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

int	parse_input(char *input, t_env **my_env)
{
	return (execute_single_input(input, my_env));
}
