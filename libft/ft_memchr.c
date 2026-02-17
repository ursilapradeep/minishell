/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 11:38:39 by uvadakku          #+#    #+#             */
/*   Updated: 2025/07/17 15:24:36 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t		i;
	const char	*ptr;

	ptr = (const char *)s;
	i = 0;
	while (i < n)
	{
		if (((unsigned char)ptr[i]) == (unsigned char)c)
		{
			return ((void *)(&(ptr)[i]));
		}
		i++;
	}
	return (NULL);
}
