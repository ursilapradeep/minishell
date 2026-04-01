/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:31:01 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/01 12:31:41 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int parse_input(char *input, t_env **my_env)
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
	status = execute_commands(cmds, my_env);//TODO: create execute_cmd_list to run builtins/execve
	free_cmd_list(cmds);
	return (status);
}

static char	*read_non_interactive_line(void)
{
	char	buf;
	char	*line;
	char	*ch;
	char	*joined;
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
		ch = ft_substr(&buf, 0, 1);
		if (!ch)
			return (free(line), NULL);
		joined = ft_strjoin(line, ch);
		free(ch);
		free(line);
		line = joined;
		if (!line)
			return (NULL);
	}
	if (bytes <= 0 && line[0] == '\0')
		return (free(line), NULL);
	return (line);
}

char *read_input(void)
{
	/* Buffer that will store the line read from the prompt. */
	char	*input;

	if (isatty(STDIN_FILENO))
	{
		g_sigint_received = 0;
		rl_done = 0;
		input = readline("minishell$ ");	/* Display the minishell prompt and read one line of user input. */
	}
	else
		input = read_non_interactive_line();
	if (!input)
	{
		if (g_sigint_received)
			return (ft_strdup(""));
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5); 	/* Interactive EOF prints exit (bash-like). */
		return (NULL); /* Signal the caller that no more input is available. */
	}
	if (*input == '\0') 	/* Skip empty lines so they are not added to command history. */
		return (input);
	add_history(input); /* Save the non-empty command in readline history. */
	return (input); 	/* Return the input line to the caller for further processing. */
}