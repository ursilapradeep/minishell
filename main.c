/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 10:42:04 by spaipur-          #+#    #+#             */
/*   Updated: 2026/04/15 05:50:19 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

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

static int	handle_input_read(char *input)
{
	if (!input)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		exit(get_last_status());
	}
	if (*input == '\0')
	{
		free(input);
		return (1);
	}
	return (0);
}

void	minishell_loop(t_env *my_env)
{
	int		exit_code;
	char	*input;

	exit_code = 0;
	while (1)
	{
		input = read_input();
		if (g_signal == SIGINT)
		{
			set_last_status(130);
			g_signal = 0;
		}
		if (handle_input_read(input))
			continue ;
		exit_code = parse_input(input, &my_env);
		set_last_status(exit_code);
		free(input);
		if (!isatty(STDIN_FILENO) && exit_code == 2)
			exit(2);
		if (exit_code == -1)
			break ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*my_env;

	(void)argc;
	(void)argv;
	set_last_status(0);
	my_env = init_env(envp);
	if (isatty(STDIN_FILENO))
		setup_signal_handlers();
	minishell_loop(my_env);
	return (0);
}
