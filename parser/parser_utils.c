/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 09:42:38 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 10:39:42 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <errno.h>

/**
 * is_redirect_operator - Check if string is a redirection operator
 * @str: String to check
 * Return: Token type if redirect, 0 otherwise
 */
static t_token_type is_redirect_operator(const char *str)
{
	if (!str || !*str)
		return (0);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (TOKEN_REDIRECT_APPEND);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	else if (*str == '>' && *(str + 1) != '>')
		return (TOKEN_REDIRECT_OUT);
	else if (*str == '<' && *(str + 1) != '<')
		return (TOKEN_REDIRECT_IN);
	return (0);
}

/**
 * skip_whitespace - Skip whitespace characters
 * @str: String pointer
 * Return: Pointer to first non-whitespace char
 */
static const char *skip_whitespace(const char *str)
{
	if (!str)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	return (str);
}

/**
 * extract_redirect_filename - Extract filename after redirect operator
 * @input: Input string after redirect operator
 * Return: Allocated filename string, NULL on error
 */
static char *extract_redirect_filename(const char *input)
{
	const char	*start;
	const char	*end;
	int			len;
	char		*filename;

	if (!input)
		return (NULL);
	start = skip_whitespace(input);
	if (!*start)
	{
		write(STDERR_FILENO, "Error: No filename after redirection operator\n", 45);
		return (NULL);
	}
	end = start;
	while (*end && *end != ' ' && *end != '\t' && *end != '|'
		&& *end != '>' && *end != '<')
		end++;
	len = end - start;
	filename = ft_calloc(len + 1, sizeof(char));
	if (!filename)
		return (NULL);
	ft_strlcpy(filename, start, len + 1);
	return (filename);
}

/**
 * open_input_file - Open file for input redirection (<)
 * @filename: File to open
 * Return: File descriptor on success, -1 on error
 */
static int open_input_file(const char *filename)
{
	int fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	return (fd);
}

/**
 * open_output_file - Open file for output redirection (>)
 * @filename: File to open
 * @append: 1 for append (>>), 0 for truncate (>)
 * Return: File descriptor on success, -1 on error
 */
static int open_output_file(const char *filename, int append)
{
	int flags;
	int fd;

	if (!filename)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	return (fd);
}

/**
 * handle_single_redirect - Handle one redirection operator
 * @cmd: Command structure to modify
 * @operator: Redirect operator (>, <, >>, <<)
 * @rest_of_line: Rest of command after operator
 * Return: Pointer to position after filename, NULL on error
 */
static const char *handle_single_redirect(t_cmd *cmd, t_token_type operator,
	const char *rest_of_line)
{
	char		*filename;
	int			fd;
	const char	*end_pos;

	if (!cmd || !rest_of_line)
		return (NULL);
	filename = extract_redirect_filename(rest_of_line);
	if (!filename)
		return (NULL);
	if (operator == TOKEN_REDIRECT_IN)
	{
		if (cmd->infd > 2)
			close(cmd->infd);
		fd = open_input_file(filename);
		if (fd < 0)
		{
			free(filename);
			return (NULL);
		}
		cmd->infd = fd;
	}
	else if (operator == TOKEN_REDIRECT_OUT || operator == TOKEN_REDIRECT_APPEND)
	{
		if (cmd->outfd > 2)
			close(cmd->outfd);
		fd = open_output_file(filename, operator == TOKEN_REDIRECT_APPEND);
		if (fd < 0)
		{
			free(filename);
			return (NULL);
		}
		cmd->outfd = fd;
	}
	else if (operator == TOKEN_HEREDOC)
	{
		write(STDERR_FILENO, "Error: HEREDOC (<<) not yet implemented\n", 41);
		free(filename);
		return (NULL);
	}
	end_pos = skip_whitespace(rest_of_line);
	while (*end_pos && *end_pos != ' ' && *end_pos != '\t' && *end_pos != '|'
		&& *end_pos != '>' && *end_pos != '<')
		end_pos++;
	free(filename);
	return (end_pos);
}

/**
 * handle_redirections - Parse and apply all redirections in a command
 * @cmd: Command structure to populate
 * @cmd_str: Command string with potential redirections
 * Return: 0 on success, -1 on error
 */
int handle_redirections(t_cmd *cmd, char *cmd_str)
{
	const char		*current;
	t_token_type	redirect_type;
	int				fd_count;

	if (!cmd || !cmd_str)
		return (-1);
	cmd->infd = STDIN_FILENO;
	cmd->outfd = STDOUT_FILENO;
	fd_count = 0;
	current = (const char *)cmd_str;
	while (current && *current)
	{
		current = skip_whitespace(current);
		if (!*current)
			break;
		redirect_type = is_redirect_operator(current);
		if (redirect_type)
		{
			if (redirect_type == TOKEN_REDIRECT_IN || 
				redirect_type == TOKEN_REDIRECT_OUT ||
				redirect_type == TOKEN_REDIRECT_APPEND)
			{
				current += (redirect_type == TOKEN_REDIRECT_APPEND || 
					redirect_type == TOKEN_HEREDOC) ? 2 : 1;
				current = handle_single_redirect(cmd, redirect_type, current);
				if (!current)
					return (-1);
				fd_count++;
			}
			else if (redirect_type == TOKEN_HEREDOC)
			{
				current += 2;
				write(STDERR_FILENO, "Error: HEREDOC not yet implemented\n", 35);
				return (-1);
			}
		}
		else
		{
			while (*current && *current != ' ' && *current != '\t' 
				&& *current != '|' && *current != '>' && *current != '<')
				current++;
		}
	}
	return (0);
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
