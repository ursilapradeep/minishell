/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:42:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 10:42:05 by spaipur-         ###   ########.fr       */
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
    t_env *my_env;
    char *input;
    int exit_code;

    (void)argc;
    (void)argv;
    exit_code = 0;
    my_env = init_env(envp);
    while (1)
    {
        input = read_input();
        if (!input)
            break;
        exit_code = process_input(input, &my_env);
        free(input);
        if (exit_code == -1)
            break;
    }
    return (0);
}
