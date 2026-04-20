/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args_utils_II.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:53:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/13 12:26:19 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_token_type(t_token *curr, t_token_check check_type)
{
	if (check_type == TOKEN_VALID_ARGUMENT)
		return (is_valid_arg_type(curr));
	else if (check_type == TOKEN_REDIRECT)
		return (is_redirect_type(curr->type));
	return (0);
}

int	process_token_for_args(t_token *current, char **args, int *i)
{
	if (current->value && current->value[0] == '\0' && !current->quoted)
		return (0);
	args[*i] = ft_strdup(current->value);
	if (!args[*i])
	{
		free_args(args);
		return (-1);
	}
	(*i)++;
	return (0);
}

static int print_paren_syntax_error(t_token *tokens)
{
	if (tokens->type == TOKEN_LPAREN || tokens->type == TOKEN_RPAREN) {
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		if (tokens->next && tokens->next->type == TOKEN_WORD)
			ft_putstr_fd(tokens->next->value, 2);
		else
			ft_putstr_fd(tokens->value, 2);
		return (-2);
	}
	return (0);
}

int	iterate_tokens_for_args(t_token *tokens, char **args, int arg_count)
{
	int	i;
	int	skip_next;

	i = 0;
	skip_next = 0;
	while (tokens && i < arg_count
		&& tokens->type != TOKEN_PIPE
		&& tokens->type != TOKEN_AND
		&& tokens->type != TOKEN_OR)
	{
		int err = print_paren_syntax_error(tokens);
		if (err)
			return err;
		if (skip_next)
		{
			skip_next = 0;
			tokens = tokens->next;
			continue ;
		}
		if (check_token_type(tokens, TOKEN_REDIRECT))
			skip_next = 1;
		else if (process_token_for_args(tokens, args, &i) == -1)
			return (-1);
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
