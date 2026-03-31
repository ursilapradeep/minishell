/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:42 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/30 11:13:57 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	has_unclosed_quotes(char *input)
{
	char	quote;
	int		i;

	quote = '\0';
	i = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = '\0';
		i++;
	}
	return (quote != '\0');
}

int	find_logical_and(char *input)
{
	int		i; 	/* Index used to scan the input string character by character. */
	char	quote;  /* Tracks the currently open quote, or '\0' when outside quotes. */

	i = 0; /* Start scanning from the beginning of the input. */
	quote = '\0'; 	/* Start with no active quote context. */
	while (input[i]) 	/* Walk through the full input until the null terminator is reached. */
	{
		if (!quote && (input[i] == '\'' || input[i] == '"')) /* Enter quote-tracking mode when an opening quote is found. */
			quote = input[i];
		else if (quote && input[i] == quote) 
			quote = '\0';
		else if (!quote && input[i] == '&' && input[i + 1] == '&') 		/* Move to the next character in the input string. */
			return (i);
		i++;
	}
	/* Report that no valid logical AND operator was found. */
	return (-1);
}

int	handle_logical_and(char *input, t_env **my_env, int and_pos)
{
	char	*left; /* Stores the command text that appears before &&. */
	char	*right; 	/* Stores the command text that appears after &&. */
	char	*left_trim; 	/* Holds the trimmed left-side and righ-side  command without surrounding spaces. */
	char	*right_trim; 	
	int		status; 	/* Stores the exit status returned by command execution. */

	left = ft_substr(input, 0, and_pos);  	/* Extract the left-hand, right-hand command from the full input. */
	right = ft_substr(input, and_pos + 2, ft_strlen(input) - and_pos - 2);
	if (!left || !right)
		return (free(left), free(right), 1); 	/* Stop if either substring allocation failed. */
	left_trim = ft_strtrim(left, " \t\n"); /* Remove leading and trailing whitespace from the left and right command. */
	right_trim = ft_strtrim(right, " \t\n");
	free(left); /* Free the untrimmed left, right substring after trimming. */
	free(right);
	if (!left_trim || !right_trim)
		return (free(left_trim), free(right_trim), 1);
	if (*left_trim == '\0' || *right_trim == '\0') /* Reject syntax where either side of && is empty after trimming. */
		return (free(left_trim), free(right_trim),
			ft_putstr_fd("minishell: syntax error near unexpected token `&&'\n", 2), 2); /* Execute the left command first and keep its exit status. */
	status = process_input(left_trim, my_env); /* Run the right command only if the left command succeeded. */
	if (status == 0)
		status = process_input(right_trim, my_env); /* Release the trimmed left command before returning. */
	free(left_trim); /* Release the trimmed right command before returning. */
	free(right_trim);/* Return the final status of the logical AND evaluation. */
	return (status);
}

// int	process_input(char *input, t_env **my_env)
// {
// 	/* Position of the first valid && operator, if one exists. */
// 	int	and_pos;
// 	int	status;

// 	if (has_unclosed_quotes(input))
// 	{
// 		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
// 		g_last_status = 2;
// 		return (2);
// 	}
// 	and_pos = find_logical_and(input); 	/* Look for a logical AND operator outside quoted sections. */
// 	/* Delegate to the && handler when the operator is present. */
// 	if (and_pos != -1)
// 		status = handle_logical_and(input, my_env, and_pos);
// 	else if (contains_pipe(input))
// 		status = handle_pipeline(input, my_env);
// 	else
// 		status = handle_single_command(input, my_env);
// 	g_last_status = status;
// 	return (status);  	/* Route piped input to the pipeline execution path. */
// }

// static char	*read_non_interactive_line(void)
// {
// 	char	buf;
// 	char	*line;
// 	char	*ch;
// 	char	*joined;
// 	int		bytes;

// 	line = ft_strdup("");
// 	if (!line)
// 		return (NULL);
// 	while (1)
// 	{
// 		bytes = read(STDIN_FILENO, &buf, 1);
// 		if (bytes <= 0)
// 			break ;
// 		if (buf == '\n')
// 			break ;
// 		ch = ft_substr(&buf, 0, 1);
// 		if (!ch)
// 			return (free(line), NULL);
// 		joined = ft_strjoin(line, ch);
// 		free(ch);
// 		free(line);
// 		line = joined;
// 		if (!line)
// 			return (NULL);
// 	}
// 	if (bytes <= 0 && line[0] == '\0')
// 		return (free(line), NULL);
// 	return (line);
// }

// char *read_input(void)
// {
// 	/* Buffer that will store the line read from the prompt. */
// 	char	*input;

// 	if (isatty(STDIN_FILENO))
// 	{
// 		g_sigint_received = 0;
// 		rl_done = 0;
// 		input = readline("minishell$ ");	/* Display the minishell prompt and read one line of user input. */
// 	}
// 	else
// 		input = read_non_interactive_line();
// 	if (!input)
// 	{
// 		if (g_sigint_received)
// 			return (ft_strdup(""));
// 		if (isatty(STDIN_FILENO))
// 			write(STDERR_FILENO, "exit\n", 5); 	/* Interactive EOF prints exit (bash-like). */
// 		return (NULL); /* Signal the caller that no more input is available. */
// 	}
// 	if (*input == '\0') 	/* Skip empty lines so they are not added to command history. */
// 		return (input);
// 	add_history(input); /* Save the non-empty command in readline history. */
// 	return (input); 	/* Return the input line to the caller for further processing. */
// }
