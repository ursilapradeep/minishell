/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:31:01 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/14 13:32:44 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	update_quote_state(char c, int *in_sq, int *in_dq)
{
	if (c == '\'' && !*in_dq)
		*in_sq = !*in_sq;
	else if (c == '"' && !*in_sq)
		*in_dq = !*in_dq;
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
	int		quote[2];

	line = ft_strdup("");
	if (!line)
		return (NULL);
	quote[0] = 0;
	quote[1] = 0;
	while (1)
	{
		bytes = read(STDIN_FILENO, &buf, 1);
		if (bytes <= 0 || (buf == '\n' && !quote[0] && !quote[1]))
			break ;
		update_quote_state(buf, &quote[0], &quote[1]);
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
		input = readline("minishell$ ");
	else
		input = read_non_interactive_line();
	if (!input)
		return (NULL);
	if (*input == '\0')
		return (input);
	add_history(input);
	return (input);
}
