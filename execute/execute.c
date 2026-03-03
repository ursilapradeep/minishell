/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 11:51:52 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/03 14:29:57 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char **environ;

// Execute command
int execute_command(char **args, t_env **my_env)
{
    char *cmd_path;
    pid_t pid;
    int status;

    if (!args || !args[0])
        return (0);
    
    if (is_builtin(args[0]))
    {
        return (execute_builtin(args, my_env));
    }
    
    cmd_path = find_command(args[0]);
    if (!cmd_path)
    {
        printf("minishell: command not found: %s\n", args[0]);
        return (127);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        return (1);
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
    return (0);
}
