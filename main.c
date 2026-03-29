/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:42:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/29 12:29:37 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell_state g_shell = {0, 0};

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

// Main shell loop: handles input, processing, and exit logic
void minishell_loop(t_env *my_env)
{
    int exit_code = 0;
    char *input;

    while (1)
    {
        input = read_input();
        if (!input)
        {
            printf("exit\n");
            exit(g_shell.last_status);
        }
        if (*input == '\0')
        {
            free(input);
            continue;
        }
        exit_code = process_input(input, &my_env);
        g_shell.last_status = exit_code;
        free(input);
        if (exit_code == -1)
            break;
    }
}

int main(int argc, char **argv, char **envp)
{
    t_env *my_env;

    (void)argc;
    (void)argv;
    g_shell.last_status = 0;
    my_env = init_env(envp); // create shell internal environment from system env
    if (isatty(STDIN_FILENO))
        setup_signal_handlers(); // setup signal handlers for interactive mode
    minishell_loop(my_env);
    return (0);
}
