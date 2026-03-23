/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 12:35:18 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/20 15:20:30 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
#include <fcntl.h>
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

/**
 * open_input_file - Open file for input redirection (<)
 * @filename: File to open
 * Return: File descriptor on success, -1 on error
 */
int open_input_file(const char *filename)
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
int open_output_file(const char *filename, int append)
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

