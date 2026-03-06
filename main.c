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
    t_env *envp;
    char *input;
    char **args;
    char **pipeline;
    int exit_code;

    (void)argc;
    (void)argv;

    exit_code = 0;
    envp = init_env(envp);

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (*input == '\0')
        {
            free(input);
            continue;
        }
        add_history(input);
        
        // Check for pipes
        if (contains_pipe(input))
        {
            pipeline = parse_pipeline(input);
            if (pipeline)
            {
                execute_pipeline(pipeline, envp);
                free_args(pipeline);
            }
        }
        else
        {
            args = split_args(input);
            if (args)
            {
                exit_code = execute_command(args, &envp);
                if (exit_code == -1)
                {
                    free_args(args);
                    break;
                }
                free_args(args);
            }
        }
        free(input);
    }
    return (0);
}