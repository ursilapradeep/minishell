/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:45:51 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/11 23:46:53 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			count_args(t_token *tokens);
char		**build_args_array(t_token *tokens, int arg_count);
int			process_redirections_in_tokens(t_cmd *cmd, t_token *tokens);

static int	process_args_and_redirs(t_cmd *cmd, t_token **tokens, int arg_count)
{
	if (arg_count > 0)
	{
		cmd->args = build_args_array(*tokens, arg_count);
		if (!cmd->args && arg_count > 0)
		{
			free(cmd);
			return (-1);
		}
	}
	if (process_redirections_in_tokens(cmd, *tokens) < 0)
	{
		free_args(cmd->args);
		free(cmd);
		return (-1);
	}
	return (0);
}

static void	advance_tokens(t_token **tokens)
{
	t_token	*sep_token;

	sep_token = find_next_separator(*tokens);
	if (sep_token)
	{
		*tokens = sep_token;
	}
	else
		*tokens = NULL;
}

int	initialize_cmd_arguments(t_cmd *cmd, t_token **tokens)
{
	int	arg_count;

	arg_count = count_args(*tokens);
	if (process_args_and_redirs(cmd, tokens, arg_count) < 0)
		return (-1);
	advance_tokens(tokens);
	return (0);
}
