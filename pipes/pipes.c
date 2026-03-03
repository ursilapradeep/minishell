#include "../minishell.h"

static void	close_all_pipes(int pipes[][2], int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static void	execute_child(char **pipeline, char **envp, int pipes[][2],
		int cmd_count, int i)
{
	char	**args;
	char	*cmd_path;

	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipes, cmd_count);
	args = split_args(pipeline[i]);
	if (args)
	{
		cmd_path = find_command(args[0], envp);
		if (cmd_path)
		{
			execve(cmd_path, args, envp);
			perror("execve");
			free(cmd_path);
		}
		else
			printf("minishell: command not found: %s\n", args[0]);
		free_args(args);
	}
	exit(127);
}

static void	fork_and_execute_all(char **pipeline, char **envp, int pipes[][2],
		int cmd_count)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < cmd_count)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		if (pid == 0)
			execute_child(pipeline, envp, pipes, cmd_count, i);
		i++;
	}
	close_all_pipes(pipes, cmd_count);
	i = 0;
	while (i < cmd_count)
	{
		wait(NULL);
		i++;
	}
}

// Execute pipeline
void	execute_pipeline(char **pipeline, char **envp)
{
	int		cmd_count;
	int		pipes[1024][2];
	int		i;

	i = 0;
	cmd_count = 0;
	while (pipeline[cmd_count])
		cmd_count++;
	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return ;
		}
		i++;
	}
	fork_and_execute_all(pipeline, envp, pipes, cmd_count);
}
