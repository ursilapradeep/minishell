#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char **environ;

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

// Split input into arguments (without strtok)
char **split_args(char *input)
{
    char **args;
    char *copy;
    int count;
    int i;
    int j;
    int start;

    copy = strdup(input);
    if (!copy)
        return (NULL);
    
    // Count arguments
    count = 0;
    i = 0;
    while (copy[i])
    {
        while (copy[i] && (copy[i] == ' ' || copy[i] == '\t' || copy[i] == '\n'))
            i++;
        if (copy[i])
        {
            count++;
            while (copy[i] && copy[i] != ' ' && copy[i] != '\t' && copy[i] != '\n')
                i++;
        }
    }

    // Allocate args array
    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
    {
        free(copy);
        return (NULL);
    }

    // Fill args array
    i = 0;
    j = 0;
    while (copy[i] && j < count)
    {
        while (copy[i] && (copy[i] == ' ' || copy[i] == '\t' || copy[i] == '\n'))
            i++;
        if (copy[i])
        {
            start = i;
            while (copy[i] && copy[i] != ' ' && copy[i] != '\t' && copy[i] != '\n')
                i++;
            args[j] = malloc(i - start + 1);
            if (!args[j])
            {
                free(copy);
                free_args(args);
                return (NULL);
            }
            strncpy(args[j], &copy[start], i - start);
            args[j][i - start] = '\0';
            j++;
        }
    }
    args[j] = NULL;
    free(copy);
    return (args);
}

// Find command in PATH
char *find_command(char *cmd)
{
    char *path_env;
    char *path_copy;
    char *dir_start;
    char full_path[1024];
    int i;
    int len;

    if (access(cmd, X_OK) == 0)
        return (strdup(cmd));
    
    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);
    
    path_copy = strdup(path_env);
    if (!path_copy)
        return (NULL);
    
    dir_start = path_copy;
    i = 0;
    while (path_copy[i])
    {
        if (path_copy[i] == ':')
        {
            len = i;
            snprintf(full_path, sizeof(full_path), "%.*s/%s", len, dir_start, cmd);
            if (access(full_path, X_OK) == 0)
            {
                free(path_copy);
                return (strdup(full_path));
            }
            dir_start = &path_copy[i + 1];
        }
        i++;
    }
    // Check last directory
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_start, cmd);
    if (access(full_path, X_OK) == 0)
    {
        free(path_copy);
        return (strdup(full_path));
    }
    free(path_copy);
    return (NULL);
}

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
        
        args = split_args(input); // create the  argv array needed to pass to execve()
        //Does this in 2 passes because we need to count the number of arguments first to allocate the array, then fill it in the second pass. ie. allocate memory and populates the aray.
        if (args)
        {
            execute_command(args);
            free_args(args);
        }
        free(input);
    }
    return (0);
}