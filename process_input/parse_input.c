/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:31:01 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/09 12:36:31 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_input(char *input, t_env **my_env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	tokens = tokenize(input);
	if (!tokens)
		return (2);
	if (expand_token_list(tokens, *my_env) < 0)
	{
		free_tokens(tokens);
		return (2);
	}
	cmds = build_commands(tokens);
	free_tokens(tokens);
	if (!cmds)
		return (2);
	if (process_heredocs(cmds) < 0)
	{
		free_cmd_list(cmds);
		return (2);
	}
	status = execute_commands(cmds, my_env);
	free_cmd_list(cmds);
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
