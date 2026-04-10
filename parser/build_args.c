/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:45:51 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/10 13:29:24 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_token_type(t_token *curr, t_token_check check_type)
{
	if (check_type == TOKEN_VALID_ARGUMENT)
	{
		return (curr->type == TOKEN_WORD && (!curr->prev
				|| (curr->prev->type != TOKEN_REDIRECT_IN
					&& curr->prev->type != TOKEN_REDIRECT_OUT
					&& curr->prev->type != TOKEN_REDIRECT_APPEND
					&& curr->prev->type != TOKEN_HEREDOC)));
	}
	else if (check_type == TOKEN_REDIRECT)
	{
		return (curr->type == TOKEN_REDIRECT_IN
			|| curr->type == TOKEN_REDIRECT_OUT
			|| curr->type == TOKEN_REDIRECT_APPEND
			|| curr->type == TOKEN_HEREDOC);
	}
	return (0);
}

int	process_token_for_args(t_token *current, char **args, int *i)
{
	args[*i] = ft_strdup(current->value);
	if (!args[*i])
	{
		free_args(args);
		return (-1);
	}
	(*i)++;
	return (0);
}

int	iterate_tokens_for_args(t_token *tokens, char **args, int arg_count)
{
	int		i;
	int		skip_next;

	i = 0;
	skip_next = 0;
	while (tokens && i < arg_count && tokens->type != TOKEN_PIPE)
	{
		if (skip_next)
		{
			skip_next = 0;
			tokens = tokens->next;
			continue ;
		}
		if (check_token_type(tokens, TOKEN_VALID_ARGUMENT)
			&& process_token_for_args(tokens, args, &i) == -1)
			return (-1);
		else if (check_token_type(tokens, TOKEN_REDIRECT))
			skip_next = 1;
		tokens = tokens->next;
	}
	args[i] = NULL;
	return (0);
}

char	**build_args_array(t_token *tokens, int arg_count)
{
	char	**args;

	if (arg_count == 0)
		return (NULL);
	args = ft_calloc(arg_count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	if (iterate_tokens_for_args(tokens, args, arg_count) == -1)
		return (NULL);
	return (args);
}

int	initialize_cmd_arguments(t_cmd *cmd, t_token **tokens)
{
	int		arg_count;
	t_token	*pipe_token;

	arg_count = count_args(*tokens);
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
	pipe_token = find_next_pipe(*tokens);
	if (pipe_token)
		*tokens = pipe_token->next;
	else
		*tokens = NULL;
	return (0);
}
