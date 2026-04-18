/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirections_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:30:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/15 04:52:34 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**copy_heredoc_delimiters(t_cmd *cmd, char *delimiter)
{
	char	**new_delimiters;
	int		i;

	new_delimiters = ft_calloc(cmd->heredoc_count + 2, sizeof(char *));
	if (!new_delimiters)
		return (NULL);
	i = 0;
	while (i < cmd->heredoc_count)
	{
		new_delimiters[i] = cmd->heredoc_delimiters[i];
		i++;
	}
	new_delimiters[cmd->heredoc_count] = ft_strdup(delimiter);
	if (!new_delimiters[cmd->heredoc_count])
	{
		free(new_delimiters);
		return (NULL);
	}
	new_delimiters[cmd->heredoc_count + 1] = NULL;
	return (new_delimiters);
}

static void	print_syntax_error_token(t_token *current)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '",
		STDERR_FILENO);
	if (!current->next)
		ft_putstr_fd("newline", STDERR_FILENO);
	else
		ft_putstr_fd(current->next->value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

static int	handle_heredoc_token(t_cmd *cmd, t_token *current)
{
	char	*delimiter;
	char	**new_delims;

	if (!current->next || current->next->type != TOKEN_WORD)
	{
		print_syntax_error_token(current);
		return (-1);
	}
	if (current->next->quoted)
		delimiter = ft_strjoin("\1", current->next->value);
	else
		delimiter = ft_strdup(current->next->value);
	if (!delimiter)
		return (-1);
	new_delims = copy_heredoc_delimiters(cmd, delimiter);
	free(delimiter);
	if (!new_delims)
		return (-1);
	free(cmd->heredoc_delimiters);
	cmd->heredoc_delimiters = new_delims;
	cmd->heredoc_count++;
	cmd->heredoc_delimiter = cmd->heredoc_delimiters[cmd->heredoc_count - 1];
	return (1);
}

int	handle_redirection(t_cmd *cmd, t_token *current, int type)
{
	char	*filename;
	int		target_fd;

	if (type == TOKEN_HEREDOC)
		return (handle_heredoc_token(cmd, current));
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		print_syntax_error_token(current);
		return (-1);
	}
	filename = current->next->value;
	target_fd = STDOUT_FILENO;
	if (current->prev && current->prev->type == TOKEN_WORD
		&& current->prev != current
		&& ft_strncmp(current->prev->value, "2", 2) == 0)
		target_fd = STDERR_FILENO;
	if ((type == TOKEN_REDIRECT_IN && cmd->infd == -2)
		|| (type != TOKEN_REDIRECT_IN && target_fd == STDERR_FILENO
			&& cmd->errfd == -2)
		|| (type != TOKEN_REDIRECT_IN && target_fd != STDERR_FILENO
			&& cmd->outfd == -2))
		return (1);
	return (process_file_fd(cmd, open_redirection_file(cmd, filename, type,
				target_fd), type, target_fd));
}
