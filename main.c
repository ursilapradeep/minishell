/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:42:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/30 15:52:06 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int	g_last_status = 0;
int	g_sigint_received = 0;

/*To prevent memory leaks, you must free both levels:

Loop through and free each string (free(args[i])) - frees individual strings like "ls", "-la", etc.
Free the array itself (free(args)) - frees the pointer array that held those strings */
// Free arguments array
void free_args(char **args)
{
    int i;

    if (!args)
        return ;
    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

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
	cmds = build_cmd_list(tokens);
	free_tokens(tokens);
	if (!cmds)
		return (2);
	status = execute_cmd_list(cmds, my_env);//TODO: create execute_cmd_list to run builtins/execve
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

int main(int argc, char **argv, char **envp)
{
    t_env *my_env; //store shell's internal environment list
    /*eg: PATH=/usr/bin:/bin*/
    char *input; //stores each line typed by user
    int exit_code; //store result of processing command.
    (void)argc;
    (void)argv;
    
    exit_code = 0;  //initialize status as zero for success
    g_last_status = 0;
    my_env = init_env(envp); //create shell internal environment from system env
    rl_catch_signals = 0;
    if (isatty(STDIN_FILENO))
        setup_signal_handlers(); //setup signal handlers for interactive mode
    while (1) 
    {
        input = read_input(); /* reads for eg: ls -l or echo hello */
        if (!input)
            break;
        if (*input == '\0')
        {
            free(input);
            continue;
        }
        exit_code = parse_input(input, &my_env); /* parse and execute command, update my_env if needed */
       // exit_code = process_input(input, &my_env);//todo: replace process_input with parse_input
        g_last_status = exit_code;
        free(input);
        if (exit_code == -1)
            break;
    }
    return (0);
}
