/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:42:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/23 11:02:52 by uvadakku         ###   ########.fr       */
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
        exit_code = process_input(input, &my_env);
        g_last_status = exit_code;
        free(input);
        if (exit_code == -1)
            break;
    }
    return (0);
}
