#include "minishell.h"

int	handle_pipeline(char *input, t_env **my_env)
{
	char	**pipeline;

	pipeline = parse_pipeline(input);
	if (pipeline)
	{
		execute_pipeline(pipeline, my_env);
		free_args(pipeline);
	}
	return (0);
}
int	handle_single_command(char *input, t_env **my_env)
{
	char	**args;
	int		exit_code;

	args = split_args(input);
	if (args)
	{
		exit_code = execute_command(args, my_env);
		if (exit_code == -1)
		{
			free_args(args);
			return (-1);
		}
		free_args(args);
	}
	return (0);
}

int	process_input(char *input, t_env **my_env)
{
	if (contains_pipe(input))
		return (handle_pipeline(input, my_env));
	else
		return (handle_single_command(input, my_env));
}

char *read_input(void)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
	{
		printf("exit\n");
		return (NULL);
	}
	if (*input == '\0')
	{
		free(input);
		return (read_input());
	}
	add_history(input);
	return (input);
}
