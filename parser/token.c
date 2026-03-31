/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 15:59:07 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/31 10:57:48 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/**
 * free_tokens - Free entire token list
 * @tokens: Token list head
 */
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

/**
 * print_tokens - DEBUG: Print all tokens
 * @tokens: Token list head
 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	char	*type_str;

	if (!tokens)
		return ((void)printf("No tokens\n"));
	current = tokens;
	printf("=== TOKENS ===\n");
	while (current)
	{
		type_str = "WORD";
		if (current->type == TOKEN_PIPE)
			type_str = "PIPE";
		else if (current->type == TOKEN_REDIRECT_IN)
			type_str = "REDIRECT_IN (<)";
		else if (current->type == TOKEN_REDIRECT_OUT)
			type_str = "REDIRECT_OUT (>)";
		else if (current->type == TOKEN_REDIRECT_APPEND)
			type_str = "REDIRECT_APPEND (>>)";
		else if (current->type == TOKEN_HEREDOC)
			type_str = "HEREDOC (<<)";
		printf("[%s]: \"%s\"\n", type_str, current->value);
		current = current->next;
	}
	printf("=============\n");
}

/**
 * add_token - Add token to end of linked list
 * @head: Pointer to list head
 * @new_token: Token to add
 */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
}

/**
 * create_token - Create a new token node
 * @value: Token string value
 * @type: Token type enum
 * Return: New token pointer, NULL on error
 */
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}
