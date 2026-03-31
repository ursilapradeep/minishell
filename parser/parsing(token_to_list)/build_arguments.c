/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:45:51 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 20:02:17 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_token_type(t_token *current, t_token_check check_type)
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
int process_token_for_args(t_token *current, char **args, int *i)
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
int iterate_tokens_for_args(t_token *tokens, char **args, int arg_count)
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
char **build_args_array(t_token *tokens, int arg_count)
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


/**
 * initialize_cmd_arguments - Helper function to initialize command arguments
 * @cmd: Command to initialize
 * @tokens: Pointer to token list (updated to point after pipe)
 * Return: 0 on success, -1 on error
 */
int initialize_cmd_arguments(t_cmd *cmd, t_token **tokens)
{
	int arg_count;
	t_token *pipe_token;

	arg_count = count_args(*tokens); /* Count arguments until pipe or end */ /* Build arguments array */
	if (arg_count > 0)
	{
		cmd->args = build_args_array(*tokens, arg_count);
		if (!cmd->args && arg_count > 0)
			return (free(cmd), -1);
	}  /* Process redirections in this command */
	if (process_redirections_in_tokens(cmd, *tokens) < 0)
	{
		free_args(cmd->args);
		free(cmd);
		return (-1);
	} /* Move to next command (skip to after pipe) */
	pipe_token = find_next_pipe(*tokens);
	if (pipe_token)
		*tokens = pipe_token->next;
	else
		*tokens = NULL;
	return (0);
}

