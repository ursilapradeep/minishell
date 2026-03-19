/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 14:00:47 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/19 14:01:30 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char *skip_non_redirects(const char *current)
{
	while (*current && *current != ' ' && *current != '\t' 
		&& *current != '|' && *current != '>' && *current != '<')
		current++;
	return (current);
}

static int	parse_redirections_loop(t_cmd *cmd, const char *current)
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

/**
 * get_command_portion - Extract command portion before any redirections
 * @cmd_str: Full command string
 * Return: Allocated string with command portion, NULL on error
 */
char *get_command_portion(char *cmd_str)
{
	const char	*start;
	const char	*end;
	char		*cmd_part;
	int			len;

	if (!cmd_str)
		return (NULL);
	start = skip_whitespace((const char *)cmd_str);
	if (!*start)
		return (NULL);
	end = start;
	while (*end && *end != '>' && *end != '<' && *end != '|')
		end++;
	len = end - start;
	while (len > 0 && (start[len - 1] == ' ' || start[len - 1] == '\t'))
		len--;
	cmd_part = ft_calloc(len + 1, sizeof(char));
	if (!cmd_part)
		return (NULL);
	ft_strlcpy(cmd_part, start, len + 1);
	return (cmd_part);
}

/**
 * print_redirections - DEBUG: Print redirection info
 * @cmd: Command to inspect
 */
void print_redirections(t_cmd *cmd)
{
	if (!cmd)
		return ;
	printf("Command FDs: infd=%d, outfd=%d\n", cmd->infd, cmd->outfd);
}
