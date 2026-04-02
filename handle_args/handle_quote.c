/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:09:09 by uvadakku          #+#    #+#             */
/*   Updated: 2026/04/02 17:57:44 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Example:
Input: echo "$USER" '$HOME'
env: USER=alice, HOME=/Users/alice

Scanning:
ctx->input → the full string being parsed
ctx->i → current position in that string
ctx->quote → whether you are inside ' or "
ctx->env → environment list used to expand $USER, $HOME, etc.
ctx->out_len → total output length being built

Then allocate 17 bytes (16 + null terminator) and fill in second pass.*/
int	handle_token_len_special_cases(t_token_len_ctx *ctx,
	int *skip, int *vlen)
{
	if (!*ctx->quote && (ctx->input[*ctx->i] == '\''
			|| ctx->input[*ctx->i] == '"'))
	{
		*ctx->quote = ctx->input[(*ctx->i)++];
		return (1);
	}
	if (*ctx->quote && ctx->input[*ctx->i] == *ctx->quote)
	{
		*ctx->quote = '\0';
		(*ctx->i)++;
		return (1);
	}
	if (ctx->input[*ctx->i] == '$' && *ctx->quote != '\''
		&& (ft_isalpha(ctx->input[*ctx->i + 1])
			|| ctx->input[*ctx->i + 1] == '_'
			|| ctx->input[*ctx->i + 1] == '?'))
	{
		*vlen = var_value_len(ctx->input, *ctx->i, ctx->env, skip);
		if (*vlen < 0)
			return (0);
		*ctx->out_len += *vlen;
		*ctx->i += *skip;
		return (1);
	}
	return (0);
}

int	count_token_len(t_token_len_ctx *ctx)

	{
	int	skip;
	int	vlen;

	if (handle_token_len_special_cases(ctx, &skip, &vlen))
		return (1);
	else
	{
		(*ctx->out_len)++;
		(*ctx->i)++;
	}
	return (1);
}

int	expanded_len(char *input, t_env *env)
{
	int				i;
	int				len;
	char			quote;
	t_token_len_ctx	ctx;

	i = 0;
	len = 0;
	quote = '\0';
	ctx.input = input;
	ctx.i = &i;
	ctx.quote = &quote;
	ctx.env = env;
	ctx.out_len = &len;
	while (input[i])
	{
		if (!count_token_len(&ctx))
			return (-1);
	}
	return (len);
}

/*input = "echo \"$USER\" '$HOME' $PATH"
env contains: USER=alice HOME = /Users/alice PATH =/usr/bin:/bin
$USER inside double quotes expands
$HOME inside single quotes does not expand
$PATH outside quotes expands*/
void	init_process_char_ctx(t_process_char_ctx *char_ctx,
		t_expand_ctx *ctx, char *input, t_env *env)
{
	char_ctx->expanded = ctx->expanded;
	char_ctx->j = &ctx->j;
	char_ctx->input = input;
	char_ctx->i = &ctx->i;
	char_ctx->quote = &ctx->quote;
	char_ctx->env = env;
}

char	*expand_variables(char *input, t_env *env)
{
	t_expand_ctx		ctx;
	t_process_char_ctx	char_ctx;
	int					len;

	len = expanded_len(input, env);
	if (len < 0)
		return (NULL);
	ctx.expanded = malloc(len + 1);
	if (!ctx.expanded)
		return (NULL);
	ctx.i = 0;
	ctx.j = 0;
	ctx.quote = '\0';
	while (input[ctx.i])
	{
		init_process_char_ctx(&char_ctx, &ctx, input, env);
		if (!process_next_char(&char_ctx))
			return (free(ctx.expanded), NULL);
	}
	ctx.expanded[ctx.j] = '\0';
	return (ctx.expanded);
}
