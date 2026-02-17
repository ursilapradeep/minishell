/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:13:21 by uvadakku          #+#    #+#             */
/*   Updated: 2025/07/23 15:11:47 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static	size_t	count_words(const char *s, char d)
{
	size_t	count;

	count = 0 ;
	while (*s)
	{
		while (*s && *s == d)
			s++;
		if (*s && *s != d)
		{
			count++;
			while (*s && *s != d)
				s++;
		}
	}
	return (count);
}

static	char	*get_next_word(const char *s, size_t len)
{
	size_t	i;
	char	*word;

	i = 0;
	word = (char *) malloc(len + 1);
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

void	free_all(char **result, int i)
{
	while (i > 0)
		free (result[--i]);
	free (result);
}

static	void	word_boundary(const char *s, const char c,
					size_t *start, size_t *end)
{
	while (s[*start] && s[*start] == c)
		(*start)++;
	*end = *start;
	while (s[*end] && s[*end] != c)
		(*end)++;
}

char	**ft_split(const char *s, char c)
{
	size_t	i;
	size_t	word_count;
	size_t	start;
	size_t	end;
	char	**result;

	if (!s)
		return (NULL);
	i = 0;
	start = 0;
	word_count = count_words(s, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (i < word_count)
	{
		word_boundary (s, c, &start, &end);
		result[i] = get_next_word(s + start, end - start);
		if (!result[i++])
			return ((free_all(result, i)), (NULL));
		start = end;
	}
	result[i] = NULL;
	return (result);
}
