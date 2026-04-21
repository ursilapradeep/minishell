/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 00:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/21 00:00:00 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	skip_pipe_after_redirect(char *input, int *i)
{
	int	left;

	left = *i - 1;
	while (left >= 0 && (input[left] == ' ' || input[left] == '\t'))
		left--;
	if (left >= 0 && (input[left] == '>' || input[left] == '<'))
	{
		if (input[*i + 1] == '|')
			(*i)++;
		(*i)++;
		return (1);
	}
	return (0);
}

static char	*classify_pipe_token(char *input, int i)
{
	int	left;
	int	right;

	left = i - 1;
	while (left >= 0 && (input[left] == ' ' || input[left] == '\t'))
		left--;
	right = i + 1;
	while (input[right] == ' ' || input[right] == '\t')
		right++;
	if (input[right] == '\0')
		return ("newline");
	if (input[right] == '|')
	{
		if (input[right + 1] == '|')
			return ("||");
		return ("|");
	}
	if (input[right] == '&' && input[right + 1] == '&')
		return ("&&");
	if (left < 0)
		return ("|");
	return (NULL);
}

static char	*get_pipe_error_token(char *input)
{
	int		i;
	char	*token;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (skip_pipe_after_redirect(input, &i))
				continue ;
			token = classify_pipe_token(input, i);
			if (token)
				return (token);
		}
		i++;
	}
	return (NULL);
}

static int	has_trailing_and_or(char *input)
{
	int	i;
	int	j;

	i = (int)ft_strlen(input) - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t'))
		i--;
	if (i >= 1 && ((input[i] == '&' && input[i - 1] == '&')
			|| (input[i] == '|' && input[i - 1] == '|')))
	{
		j = i - 2;
		while (j >= 0 && (input[j] == ' ' || input[j] == '\t'))
			j--;
		if (j >= 0 && (input[j] == '>' || input[j] == '<'))
			return (0);
		return (1);
	}
	return (0);
}

void	print_pipe_error(char *input)
{
	char	*token;

	if (has_trailing_and_or(input))
	{
		ft_putstr_fd("minishell: syntax error near unexpected"
			" token 'newline'\n", 2);
		return ;
	}
	token = get_pipe_error_token(input);
	if (!token)
		return ;
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}
