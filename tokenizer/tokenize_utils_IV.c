/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_IV.c                               :::      :::    :::   */
/*                                                    :::      :::    :::   */
/*   By: spaipur- <spaipur-@student.42.fr>          :::      :::    :::   */
/*                                                :::      :::    :::   */
/*   Created: 2026/04/11 22:53:00 by spaipur-          :::      :::    :::   */
/*   Updated: 2026/04/11 22:53:00 by spaipur-         :::      :::    :::   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*determine_token_value(const char **current,
			t_token_type *token_type, int *consumed);
