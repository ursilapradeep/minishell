#include "../minishell.h"

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
