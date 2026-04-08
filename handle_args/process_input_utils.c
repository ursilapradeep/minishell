/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 10:45:56 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/08 17:36:28 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_quote_state(char c, char *quote)
{
	if (!quote)
		return ;
	if (*quote == '\0' && is_quote(c))
		*quote = c;
	else if (*quote != '\0' && c == *quote)
		*quote = '\0';
}

int	is_invalid_pipe_position(char *input, int i) /* |ls leadin or trailing pipes return 0 */
{
	int	left;
	int	right;

	left = i - 1;
	while (left >= 0 && is_space(input[left]))
		left--;
	right = i + 1;
	while (input[right] && is_space(input[right]))
		right++;
	if (left < 0 || input[left] == '|'
		|| input[right] == '\0' || input[right] == '|')
		return (1);
	return (0);
}

int	has_unclosed_quotes(char *input)
{
	char	quote;
	int		i;

	quote = '\0';
	i = 0;
	while (input[i])
	{
		update_quote_state(input[i], &quote);
		i++;
	}
	return (quote != '\0');
}

int	find_logical_and(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	while (input[i])
	{
		update_quote_state(input[i], &quote);
		if (!quote && input[i] == '&' && input[i + 1] == '&')
			return (i);
		i++;
	}
	return (-1);
}

/*"echo ok && pwd" → echo succeeds (0),
so pwd runs and returns its status.
"false && pwd" → false fails (non-zero),
so pwd doesn't run, returns the failure status.*/
int	extract_and_trim_commands(char *input, int and_pos,
	char **left_trim, char **right_trim)
{
	char	*left;
	char	*right;

	left = ft_substr(input, 0, and_pos);
	right = ft_substr(input, and_pos + 2, ft_strlen(input) - and_pos - 2);
	if (!left || !right)
		return (free(left), free(right), 1);
	*left_trim = ft_strtrim(left, " \t\n");
	*right_trim = ft_strtrim(right, " \t\n");
	free(left);
	free(right);
	if (!*left_trim || !*right_trim)
		return (free(*left_trim), free(*right_trim), 1);
	return (0);
}

int	handle_logical_and(char *input, t_env **my_env, int and_pos)
{
	char	*left_trim;
	char	*right_trim;
	int		status;

	if (extract_and_trim_commands(input, and_pos, &left_trim, &right_trim) != 0)
		return (1);
	if (*left_trim == '\0' || *right_trim == '\0')
		return (free(left_trim), free(right_trim),
			ft_putstr_fd("minishell: syntax error `&&'\n", 2), 2);
	status = process_input(left_trim, my_env);
	if (status == 0)
		status = process_input(right_trim, my_env);
	free(left_trim);
	free(right_trim);
	return (status);
}
