/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:42:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/07 11:06:24 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell_state	g_shell = {0, 0};

void	free_args(char **args)
{
	int	i;

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

void	minishell_loop(t_env *my_env)
{
	int		exit_code;
	char	*input;

	exit_code = 0;
	while (1)
	{
		input = read_input();
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				printf("exit");
			exit(g_shell.last_status);
		}
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		exit_code = process_input(input, &my_env);
		g_shell.last_status = exit_code;
		free(input);
		if (exit_code == -1)
			break ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*my_env;

	(void)argc;
	(void)argv;
	g_shell.last_status = 0;
	my_env = init_env(envp);
	if (isatty(STDIN_FILENO))
		setup_signal_handlers();
	minishell_loop(my_env);
	return (0);
}
