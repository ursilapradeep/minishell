/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 18:13:54 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/23 18:14:26 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	word_end(char *input, int i)
{
	char	quote;

	quote = '\0';
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = '\0';
		else if (!quote && is_space(input[i]))
			break ;
		i++;
	}
	return (i);
}

int	word_len_without_quotes(char *input, int start, int end)
{
	int		len;
	int		i;
	char	quote;

	len = 0;
	i = start;
	quote = '\0';
	while (i < end)
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = '\0';
		else
			len++;
		i++;
	}
	return (len);
}

void	copy_without_quotes(char *dst, char *input, int start, int end)
{
	int		j;
	int		i;
	char	quote;

	j = 0;
	i = start;
	quote = '\0';
	while (i < end)
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = '\0';
		else
			dst[j++] = input[i];
		i++;
	}
	dst[j] = '\0';
}
