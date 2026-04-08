/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:42 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/08 17:24:40 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_invalid_pipe_syntax(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	while (input[i])
	{
		update_quote_state(input[i], &quote);
		if (!quote && input[i] == '|')
		{
			if (is_invalid_pipe_position(input, i))
				return (1);
		}
		i++;
	}
	return (0);
}

/* Position of the first valid && operator, if one exists. */
/* Look for a logical AND operator outside quoted sections. */
	/* Route piped input to the pipeline execution path. */
int	process_input(char *input, t_env **my_env)
{
	int	and_pos;
	int	status;

	if (has_unclosed_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		g_shell.last_status = 2;
		return (2);
	}
	if (has_invalid_pipe_syntax(input))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_shell.last_status = 2;
		return (2);
	}
	and_pos = find_logical_and(input);
	if (and_pos != -1)
		status = handle_logical_and(input, my_env, and_pos);
	else if (contains_pipe(input))
		status = handle_pipeline(input, my_env);
	else
		status = handle_single_command(input, my_env);
	g_shell.last_status = status;
	return (status);
}

/*Input: line = "hel", buf = 'l'
Output: "hell"

Memory flow:
- ch = "l" (allocated)
- joined = "hell" (allocated)
- free "hel" and free "l"
- return "hell"*/
char	*append_char_to_line(char *line, char buf)
{
	char	*ch;
	char	*joined;

	ch = ft_substr(&buf, 0, 1);
	if (!ch)
		return (free(line), NULL);
	joined = ft_strjoin(line, ch);
	free(ch);
	free(line);
	return (joined);
}

/*line = ft_strdup("") → line = "" (empty string)

First iteration:

read(STDIN_FILENO, &buf, 1) → reads 'h', bytes = 1
buf != '\n' → continue
append_char_to_line(line, 'h') → line = "h"
Second iteration:

Read 'e' → line = "he"
Continue through 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd':

line = "hello world"
Next iteration:

read() reads '\n'
buf == '\n' → break out of loop
After loop:

bytes > 0 and line[0] != '\0' → condition fails
return (line) → returns "hello world"*/
char	*read_non_interactive_line(void)
{
	char	buf;
	char	*line;
	int		bytes;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	while (1)
	{
		bytes = read(STDIN_FILENO, &buf, 1);
		if (bytes <= 0)
			break ;
		if (buf == '\n')
			break ;
		line = append_char_to_line(line, buf);
		if (!line)
			return (NULL);
	}
	if (bytes <= 0 && line[0] == '\0')
		return (free(line), NULL);
	return (line);
}

char	*read_input(void)
{
	char	*input;

	if (isatty(STDIN_FILENO))
	{
		g_shell.sigint_received = 0;
		input = readline("minishell$ ");
	}
	else
		input = read_non_interactive_line();
	if (!input)
	{
		if (g_shell.sigint_received)
			return (ft_strdup(""));
		return (NULL);
	}
	if (*input == '\0')
		return (input);
	add_history(input);
	return (input);
}
