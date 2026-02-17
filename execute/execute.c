#include "../minishell.h"

extern char **environ;

// Execute command
void execute_command(char **args)
{
    char *cmd_path;
    pid_t pid;
    int status;

    cmd_path = find_command(args[0]);
    if (!cmd_path)
    {
        printf("minishell: command not found: %s\n", args[0]);
        return ;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        return ;
    }
    
    if (pid == 0)
    {
        // Child process
        execve(cmd_path, args, environ);
        perror("execve");
        exit(127);
    }
    else
    {
        // Parent process
        waitpid(pid, &status, 0);
    }
    free(cmd_path);
}
