#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_env *my_env;
    char *input;
    char **args;
    int exit_code;

    (void)argc;
    (void)argv;

    exit_code = 0;
    my_env = init_env(envp);

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
            exit_code = execute_command(args, &my_env);
            if (exit_code == -1)
                break;
            free_args(args);
        }
        free(input);
    }
    return (0);
}