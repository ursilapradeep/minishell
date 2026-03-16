/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:15:42 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/16 15:00:57 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	find_logical_and(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = '\0';
		else if (!quote && input[i] == '&' && input[i + 1] == '&')
			return (i);
		i++;
	}
	return (-1);
}

static int	handle_logical_and(char *input, t_env **my_env, int and_pos)
{
	char	*left;
	char	*right;
	char	*left_trim;
	char	*right_trim;
	int		status;

	left = ft_substr(input, 0, and_pos);
	right = ft_substr(input, and_pos + 2, ft_strlen(input) - and_pos - 2);
	if (!left || !right)
		return (free(left), free(right), 1);
	left_trim = ft_strtrim(left, " \t\n");
	right_trim = ft_strtrim(right, " \t\n");
	free(left);
	free(right);
	if (!left_trim || !right_trim)
		return (free(left_trim), free(right_trim), 1);
	if (*left_trim == '\0' || *right_trim == '\0')
		return (free(left_trim), free(right_trim),
			ft_putstr_fd("minishell: syntax error near unexpected token `&&'\n", 2), 2);
	status = process_input(left_trim, my_env);
	if (status == 0)
		status = process_input(right_trim, my_env);
	free(left_trim);
	free(right_trim);
	return (status);
}

int	process_input(char *input, t_env **my_env)
{
	int	and_pos;

	and_pos = find_logical_and(input);
	if (and_pos != -1)
		return (handle_logical_and(input, my_env, and_pos));
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
