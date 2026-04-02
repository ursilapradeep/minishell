/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:19:38 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/01 19:09:52 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*For actual shell execution with echo hi > out.txt:
	command executed: echo hi
	content written to out.txt: hi (plus newline), not "echo hi".*/
char	*remove_substring(const char *input, int start, int end)
{
	char	*result;
	int		len;
	int		new_len;

	len = ft_strlen(input);
	new_len = len - (end - start);
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, input, start);
	ft_memcpy(result + start, input + end, len - end);
	result[new_len] = '\0';
	return (result);
}

/*Short example:
Command: cat < input.txt
This function makes cat read from input.txt instead of keyboard input.*/
int	handle_input_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: input redirect");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
