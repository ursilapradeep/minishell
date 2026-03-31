/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections_utils_II.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 12:35:18 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 20:54:51 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

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

