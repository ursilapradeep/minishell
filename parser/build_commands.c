/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:45:51 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 16:55:50 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/**
 * free_args - Free argument array
 * @args: Array of strings to free
 */
void free_args(char **args)
{
	int i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// Helper function to check if a token is a valid argument or a redirect
typedef enum e_token_check
{
    TOKEN_VALID_ARGUMENT,
    TOKEN_REDIRECT
}   t_token_check;

static int check_token_type(t_token *current, t_token_check check_type)
{
    if (check_type == TOKEN_VALID_ARGUMENT)
    {
        return (current->type == TOKEN_WORD && (!current->prev ||
                (current->prev->type != TOKEN_REDIRECT_IN &&
                 current->prev->type != TOKEN_REDIRECT_OUT &&
                 current->prev->type != TOKEN_REDIRECT_APPEND &&
                 current->prev->type != TOKEN_HEREDOC)));
    }
    else if (check_type == TOKEN_REDIRECT)
    {
        return (current->type == TOKEN_REDIRECT_IN ||
                current->type == TOKEN_REDIRECT_OUT ||
                current->type == TOKEN_REDIRECT_APPEND ||
                current->type == TOKEN_HEREDOC);
    }
    return (0);
}

// Helper function to process a single token and add to args array
static int process_token_for_args(t_token *current, char **args, int *i)
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

// Helper function to iterate through tokens and build args array
static int iterate_tokens_for_args(t_token *tokens, char **args, int arg_count)
{
    t_token *current = tokens;
    int     i = 0;
    int     skip_next = 0;

    while (current && i < arg_count && current->type != TOKEN_PIPE)
    {
        if (skip_next && !(skip_next = 0) && (current = current->next))
            continue;
        if (check_token_type(current, TOKEN_VALID_ARGUMENT))
        {
            if (process_token_for_args(current, args, &i) == -1)
                return (-1);
        }
        else if (check_token_type(current, TOKEN_REDIRECT))
            skip_next = 1;
        current = current->next;
    }
    args[i] = NULL;
    return (0);
}

/**
 * build_args_array - Build argument array from tokens (excluding redirect filenames)
 * @tokens: Token list
 * @arg_count: Number of arguments to collect
 * Return: Newly allocated args array, NULL on error
 */
static char **build_args_array(t_token *tokens, int arg_count)
{
    char **args;

    if (arg_count == 0)
        return (NULL);

    args = ft_calloc(arg_count + 1, sizeof(char *));
    if (!args)
        return (NULL);

    if (iterate_tokens_for_args(tokens, args, arg_count) == -1)
        return (NULL);

    return (args);
}

