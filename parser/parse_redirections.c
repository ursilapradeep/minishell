/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 14:00:47 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 20:47:28 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
const char *skip_non_redirects(const char *current)
{
	while (*current && *current != ' ' && *current != '\t' 
		&& *current != '|' && *current != '>' && *current != '<')
		current++;
	return (current);
}

/**
 * is_redirect_operator - Check if string is a redirection operator
 * @str: String to check
 * Return: Token type if redirect, 0 otherwise
 */
t_token_type is_redirect_operator(const char *str)
{
	if (!str || !*str)
		return (0);
	if (ft_strncmp(str, ">>", 2) == 0)
	{
		write(STDERR_FILENO, "Debug: Detected TOKEN_REDIRECT_APPEND\n", 37);
		return (TOKEN_REDIRECT_APPEND);
	}
	else if (ft_strncmp(str, "<<", 2) == 0)
	{
		write(STDERR_FILENO, "Debug: Detected TOKEN_HEREDOC\n", 31);
		return (TOKEN_HEREDOC);
	}
	else if (*str == '>' && *(str + 1) != '>')
	{
		write(STDERR_FILENO, "Debug: Detected TOKEN_REDIRECT_OUT\n", 36);
		return (TOKEN_REDIRECT_OUT);
	}
	else if (*str == '<' && *(str + 1) != '<')
	{
		write(STDERR_FILENO, "Debug: Detected TOKEN_REDIRECT_IN\n", 35);
		return (TOKEN_REDIRECT_IN);
	}
	write(STDERR_FILENO, "Debug: No redirection operator detected\n", 41);
	return (0);
}

const char	*process_redirection(t_cmd *cmd, const char *current,
	t_token_type type, int *fd_count)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT ||
		type == TOKEN_REDIRECT_APPEND)
	{
		current += (type == TOKEN_REDIRECT_APPEND) ? 2 : 1;
		current = handle_single_redirect(cmd, type, current);
		if (!current)
		{
			write(STDERR_FILENO, "Error: Failed to process redirection\n", 37);
			return (NULL);
		}
		(*fd_count)++;
		return (current);
	}
	else if (type == TOKEN_HEREDOC)
	{
		current += 2;
		write(STDERR_FILENO, "Error: HEREDOC not yet implemented\n", 35);
		return (NULL);
	}
	return (current);
}

int	parse_redirections_loop(t_cmd *cmd, const char *current)
{
	t_token_type	redirect_type;
	int				fd_count;

	fd_count = 0;
	while (current && *current)
	{
		current = skip_whitespace(current);
		if (!*current)
			break;
		redirect_type = is_redirect_operator(current);
		if (redirect_type)
		{
			current = process_redirection(cmd, current, redirect_type, &fd_count);
			if (!current)
				return (-1);
		}
		else
			current = skip_non_redirects(current);
	}
	return (0);
}

/**
 * handle_redirections - Parse and apply all redirections in a command
 * @cmd: Command structure to populate
 * @cmd_str: Command string with potential redirections
 * Return: 0 on success, -1 on error
 */
int handle_redirections(t_cmd *cmd, char *cmd_str)
{
	if (!cmd || !cmd_str)
		return (-1);
	cmd->infd = STDIN_FILENO;
	cmd->outfd = STDOUT_FILENO;
	return (parse_redirections_loop(cmd, cmd_str));
}