/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_split.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 23:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/12 00:39:01 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	set_first_field(t_token *current, char *old, int *index)
{
	int	start;

	while (old[*index] == ' ' || old[*index] == '\t')
		(*index)++;
	start = *index;
	while (old[*index] && old[*index] != ' ' && old[*index] != '\t')
		(*index)++;
	current->value = ft_substr(old, start, (size_t)(*index - start));
	if (!current->value)
		return (-1);
	return (0);
}

static int	append_field_token(t_token **last, char *old, int start, int end)
{
	t_token	*new_token;

	new_token = create_token(ft_substr(old, start, (size_t)(end - start)),
			TOKEN_WORD);
	if (!new_token)
		return (-1);
	new_token->next = (*last)->next;
	if ((*last)->next)
		(*last)->next->prev = new_token;
	new_token->prev = *last;
	(*last)->next = new_token;
	*last = new_token;
	return (0);
}

int	split_unquoted_fields(t_token *current)
{
	char	*old;
	int		index;
	int		start;
	t_token	*last;

	old = current->value;
	index = 0;
	if (set_first_field(current, old, &index) < 0)
		return (-1);
	last = current;
	while (old[index])
	{
		while (old[index] == ' ' || old[index] == '\t')
			index++;
		if (!old[index])
			break ;
		start = index;
		while (old[index] && old[index] != ' ' && old[index] != '\t')
			index++;
		if (append_field_token(&last, old, start, index) < 0)
			return (free(old), -1);
	}
	free(old);
	return (0);
}
