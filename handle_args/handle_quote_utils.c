/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 16:10:08 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 15:57:26 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Input: echo $?
g_shell.last_status = 127
Function converts 127 to "127"
Writes 1, 2, 7 into the output
Advances past $?
Result: echo 127 ..only handles the special case of $?.*/
static int	append_last_status(t_process_char_ctx *ctx)
{
	char	*status_str;
	char	*tmp;

	status_str = ft_itoa(g_shell.last_status);
	if (!status_str)
		return (0);
	tmp = status_str;
	while (*tmp)
		ctx->expanded[(*ctx->j)++] = *tmp++;
	free(status_str);
	*ctx->i += 2;
	return (1);
}

/*Input: echo $MY_VAR1*/
int	append_var_value(t_process_char_ctx *ctx)
{
	int		key_len;
	char	*key;
	char	*value;

	if (ctx->input[*ctx->i + 1] == '?')
		return (append_last_status(ctx));
	key_len = 0;
	while (is_var_char(ctx->input[*ctx->i + 1 + key_len]))
		key_len++;
	key = ft_substr(ctx->input, *ctx->i + 1, key_len);
	if (!key)
		return (0);
	value = get_env_value(ctx->env, key);
	if (value)
		while (*value)
			ctx->expanded[(*ctx->j)++] = *value++;
	free(key);
	*ctx->i += key_len + 1;
	return (1);
}

/*Input: "echo $USER"
Initial: expanded = "", i = 0, j = 0, quote = '\0'

Call 1: input[0] = '"'   → *quote = '"', i = 1
Call 2: input[1] = 'e'   → expanded[0] = 'e', i = 2, j = 1
Call 3: input[2] = 'c'   → expanded[1] = 'c', i = 3, j = 2
...
Call 7: input[6] = '$'   → calls append_var_value() → expands "USER" to "alice"
 expanded = "echo alice", i = 11, j = 10
Call 8: input[11] = '"'  → *quote = '\0', closes quote 
output: echo alice (variable expanded double quote)*/
int	process_next_char(t_process_char_ctx *ctx)
{
	if (!*ctx->quote && (ctx->input[*ctx->i] == '\''
			|| ctx->input[*ctx->i] == '"'))
		*ctx->quote = ctx->input[(*ctx->i)++];
	else if (*ctx->quote && ctx->input[*ctx->i] == *ctx->quote)
	{
		*ctx->quote = '\0';
		(*ctx->i)++;
	}
	else if (ctx->input[*ctx->i] == '$' && *ctx->quote != '\''
		&& (ft_isalpha(ctx->input[*ctx->i + 1])
			|| ctx->input[*ctx->i + 1] == '_'
			|| ctx->input[*ctx->i + 1] == '?'))
	{
		if (!append_var_value(ctx))
			return (0);
	}
	else
		ctx->expanded[(*ctx->j)++] = ctx->input[(*ctx->i)++];
	return (1);
}
