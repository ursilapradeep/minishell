/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections_utils_I.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 09:42:38 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 20:52:36 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <errno.h>

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
 * skip_whitespace - Skip whitespace characters
 * @str: String pointer
 * Return: Pointer to first non-whitespace char
 */
const char *skip_whitespace(const char *str)
{
	if (!str || !*str)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (*str)
		return str;
	else
		return NULL;
}

/**
 * handle_single_redirect - Handle one redirection operator
 * @cmd: Command structure to modify
 * @operator: Redirect operator (>, <, >>, <<)
 * @rest_of_line: Rest of command after operator
 * Return: Pointer to position after filename, NULL on error
 */
int open_redirect_file(t_cmd *cmd, t_token_type operator, const char *filename)
{
	int fd;

	if (operator == TOKEN_REDIRECT_IN)
	{
		if (cmd->infd > 2)
			close(cmd->infd);
		fd = open_input_file(filename);
		if (fd < 0)
			return (-1);
		cmd->infd = fd;
	}
	else if (operator == TOKEN_REDIRECT_OUT || operator == TOKEN_REDIRECT_APPEND)
	{
		if (cmd->outfd > 2)
			close(cmd->outfd);
		fd = open_output_file(filename, operator == TOKEN_REDIRECT_APPEND);
		if (fd < 0)
			return (-1);
		cmd->outfd = fd;
	}
	return (0);
}

/**
 * extract_redirect_filename - Extract filename after redirect operator
 * @input: Input string after redirect operator
 * Return: Allocated filename string, NULL on error
 */
char *extract_redirect_filename(const char *input)
{
    const char *start;
    const char *end;
    int len;
    char *filename;

    if (!input)
        return (NULL);
    start = skip_whitespace(input);
    if (!start || !*start) // Check if the string is empty after skipping whitespace
    {
        write(STDERR_FILENO, "Error: No filename after redirection operator\n", 45);
        return (NULL);
    }
    end = start;
    while (*end && *end != ' ' && *end != '\t' && *end != '|' && *end != '>' && *end != '<')
        end++;
    len = end - start;
    if (len == 0) // Check if no valid filename is found
    {
        write(STDERR_FILENO, "Error: Missing filename\n", 24);
        return (NULL);
    }
    filename = ft_calloc(len + 1, sizeof(char));
    if (!filename)
        return (NULL);
    ft_strlcpy(filename, start, len + 1);
    return (filename);
}

// const char *ret_heredoc(const char *rest_of_line)
// {
// 	(void)rest_of_line;
// 	return (NULL);
// }

const char *handle_single_redirect(t_cmd *cmd, t_token_type operator,
	const char *rest_of_line)
{
	char *filename;
	const char *end_pos;

	if (!cmd || !rest_of_line)
		return (NULL);
	filename = extract_redirect_filename(rest_of_line);
	if (!filename)
	{
		write(STDERR_FILENO, "Error: Missing or invalid filename\n", 35);
		return (NULL);
	}
	if (operator == TOKEN_HEREDOC)
	{
		free(filename);
		return (NULL);
	}
	if (open_redirect_file(cmd, operator, filename) < 0)
		return (free(filename), NULL);
	end_pos = skip_whitespace(rest_of_line);
	while (*end_pos && *end_pos != ' ' && *end_pos != '\t' && *end_pos != '|'
		&& *end_pos != '>' && *end_pos != '<')
		end_pos++;
	free(filename);
	return (end_pos);
}


