#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_env *my_env;
    char *input;
    char **args;
    (void)argc;
    (void)argv;

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
            execute_command(args, &my_env);
            free_args(args);
        }
        free(input);
    }
    return (0);
}