/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 14:02:40 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * is_blank_char - Check if character is whitespace
 * @c: Character to check
 * Return: 1 if whitespace, 0 otherwise
 */
int is_blank_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/**
 * is_quote_char - Check if character is a quote
 * @c: Character to check
 * Return: 1 if quote, 0 otherwise
 */
int is_quote_char(char c)
{
	return (c == '"' || c == '\'');
}

/**
 * update_current_quote - Toggle quote state
 * @curr_quote: Pointer to current quote state
 * @quote_char: Quote character (" or ')
 */
void update_current_quote(char *curr_quote, char quote_char)
{
	if (*curr_quote == 0)
		*curr_quote = quote_char;
	else if (*curr_quote == quote_char)
		*curr_quote = 0;
}

/**
 * is_part_of_curr_arg - Check if character is part of current argument
 * @c: Character to check
 * @curr_quote: Current quote context
 * Return: 1 if part of argument, 0 if argument separator
 */
int is_part_of_curr_arg(char c, char curr_quote)
{
	if (curr_quote != 0)
		return (1);
	return (!is_blank_char(c) && c != '|' && c != '>' && c != '<');
}

/**
 * handle_backslash - Handle escape sequences in quotes
 * Adapted from pipex for minishell
 * @str: String being processed
 * @src: Pointer to source position
 * @dst: Pointer to destination position
 */
void handle_backslash(char *str, int *src, int *dst)
{
	if (str[*src + 1] != '\0')
	{
		*src = *src + 1;
		str[*dst] = str[*src];
		*dst = *dst + 1;
		*src = *src + 1;
	}
	else
	{
		str[*dst] = str[*src];
		*dst = *dst + 1;
		*src = *src + 1;
	}
}

/**
 * handle_quote_open - Handle opening quote
 * @c: Quote character
 * @quote: Pointer to quote state
 * @src: Pointer to source position
 */
void handle_quote_open(char c, char *quote, int *src)
{
	*quote = c;
	*src = *src + 1;
}

/**
 * handle_quote_close - Handle closing quote
 * @quote: Pointer to quote state
 * @src: Pointer to source position
 */
void handle_quote_close(char *quote, int *src)
{
	*quote = 0;
	*src = *src + 1;
}

/**
 * handle_normal_char - Handle regular character
 * @str: String being processed
 * @src: Pointer to source position
 * @dst: Pointer to destination position
 */
void handle_normal_char(char *str, int *src, int *dst)
{
	str[*dst] = str[*src];
	*dst = *dst + 1;
	*src = *src + 1;
}

/**
 * remove_quotes - Remove quotes from string while preserving content
 * Adapted from pipex for minishell
 * @str: String to process
 */
void remove_quotes(char *str)
{
	int		i;
	int		dst;
	char	quote;
	char	c;

	if (!str)
		return ;
	i = 0;
	dst = 0;
	quote = 0;
	while (str[i] != '\0')
	{
		c = str[i];
		if (c == '\\' && str[i + 1] != '\0' && quote != 0)
			handle_backslash(str, &i, &dst);
		else if (is_quote_char(c) && quote == 0)
			handle_quote_open(c, &quote, &i);
		else if (quote != 0 && c == quote)
			handle_quote_close(&quote, &i);
		else
			handle_normal_char(str, &i, &dst);
	}
	str[dst] = '\0';
}

/**
 * count_args - Count number of arguments in command string
 * Adapted from pipex for minishell
 * @cmd: Command string
 * Return: Number of arguments
 */
static int count_args(const char *cmd)
{
	int		count;
	char	curr_quote;

	if (!cmd || !*cmd)
		return (0);
	count = 0;
	curr_quote = 0;
	while (*cmd)
	{
		while (*cmd && is_blank_char(*cmd))
			cmd++;
		if (*cmd)
		{
			count++;
			while (*cmd && is_part_of_curr_arg(*cmd, curr_quote))
			{
				if (*cmd == '\\' && *(cmd + 1) != '\0' && curr_quote != 0)
					cmd += 2;
				else if ((*cmd == '"' || *cmd == '\'')
					&& (curr_quote == 0 || curr_quote == *cmd))
					update_current_quote(&curr_quote, *cmd++);
				else
					cmd++;
			}
		}
	}
	return (count);
}

/**
 * get_next_arg - Extract next argument from command string
 * Adapted from pipex for minishell
 * @cmd: Pointer to command string
 * Return: Newly allocated argument string
 */
static char *get_next_arg(char **cmd)
{
	char	*start_pos;
	char	*arg;
	int		length;
	char	curr_quote;
	const char *temp;

	while (**cmd && is_blank_char(**cmd))
		(*cmd)++;
	start_pos = *cmd;
	curr_quote = 0;
	length = 0;
	temp = *cmd;
	while (*temp && is_part_of_curr_arg(*temp, curr_quote))
	{
		if (*temp == '\\' && *(temp + 1) != '\0' && curr_quote != 0)
		{
			temp++;
			length++;
		}
		else if ((*temp == '"' || *temp == '\'')
			&& (curr_quote == 0 || curr_quote == *temp))
			update_current_quote(&curr_quote, *temp);
		temp++;
		length++;
	}
	*cmd = (char *)temp;
	arg = ft_calloc(length + 1, sizeof(char));
	if (!arg)
		return (NULL);
	ft_strlcpy(arg, start_pos, length + 1);
	return (arg);
}

/**
 * parse_command_args - Parse command string into argument array
 * Combines pipex's robust parsing with minishell structures
 * @cmd_str: Command string to parse
 * Return: Null-terminated array of arguments
 */
char **parse_command_args(char *cmd_str)
{
	char	**args;
	int		arg_count;
	int		i;
	char	*cmd_copy;

	if (!cmd_str || !*cmd_str)
		return (NULL);
	arg_count = count_args(cmd_str);
	if (arg_count == 0)
		return (NULL);
	args = ft_calloc(arg_count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	cmd_copy = ft_strdup(cmd_str);
	if (!cmd_copy)
	{
		free(args);
		return (NULL);
	}
	i = 0;
	while (i < arg_count)
	{
		args[i] = get_next_arg(&cmd_copy);
		if (!args[i])
		{
			free(cmd_copy);
			free_args(args);
			return (NULL);
		}
		remove_quotes(args[i]);
		i++;
	}
	args[i] = NULL;
	free(cmd_copy);
	return (args);
}
