#include "minishell.h"

int main(void)
{
    char *input;
    char **args;

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
        
        args = split_args(input);
        if (args)
        {
            execute_command(args);
            free_args(args);
        }
        free(input);
    }
    return (0);
}