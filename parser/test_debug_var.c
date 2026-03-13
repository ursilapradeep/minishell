/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_debug_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <spaipur-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:15:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/13 13:56:20 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	const char *input = "${HOME}";
	const char *var_name = input + 1;  /* Points to {HOME} */
	const char *start;
	const char *end;
	int len;
	
	printf("Input: %s\n", input);
	printf("var_name (input+1): %s\n", var_name);
	
	if (*var_name == '{')
	{
		start = var_name + 1;  /* Points to H */
		end = start;
		while (*end && *end != '}')
			end++;
		len = end - start;
		printf("Start pointer: %s\n", start);
		printf("End pointer: %s\n", end);
		printf("Extracted length: %d\n", len);
		printf("Extracted name: ");
		for (int i = 0; i < len; i++)
			printf("%c", start[i]);
		printf("\n");
	}
	
	return (0);
}
