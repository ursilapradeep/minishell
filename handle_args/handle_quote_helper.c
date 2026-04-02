/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 14:48:42 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 15:58:06 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

//counting side $?
int	handle_exit_status_var(t_var_value_len_ctx *ctx)
{
	ctx->status_str = ft_itoa(g_shell.last_status);
	if (!ctx->status_str)
		return (-1);
	ctx->len = ft_strlen(ctx->status_str);
	free(ctx->status_str);
	*ctx->skip = 2;
	return (ctx->len);
}

/*ex: the key is USER (without the $).
i points to the $ character
input[i + 1] starts at U (first char after $)
loop counts: U, S, E, R → key_len = 4
ft_substr(input, i + 1, key_len) extracts 4 chars starting from i+1
Result: key = "USER"
*/
int	var_value_len(char *input, int i, t_env *env, int *skip)
{
	t_var_value_len_ctx	ctx;

	ctx.input = input;
	ctx.i = i;
	ctx.env = env;
	ctx.skip = skip;
	if (ctx.input[ctx.i + 1] == '?')
		return (handle_exit_status_var(&ctx));
	ctx.key_len = 0;
	while (is_var_char(ctx.input[ctx.i + 1 + ctx.key_len]))
		ctx.key_len++;
	ctx.key = ft_substr(ctx.input, ctx.i + 1, ctx.key_len);
	if (!ctx.key)
		return (-1);
	ctx.value = get_env_value(ctx.env, ctx.key);
	ctx.len = 0;
	if (ctx.value)
		ctx.len = ft_strlen(ctx.value);
	free(ctx.key);
	*ctx.skip = ctx.key_len + 1;
	return (ctx.len);
}
