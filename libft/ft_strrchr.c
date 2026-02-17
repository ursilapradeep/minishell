/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:35:39 by uvadakku          #+#    #+#             */
/*   Updated: 2025/07/09 16:46:57 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	last_ch;

	i = 0;
	last_ch = -1;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			last_ch = i;
		}
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	if (last_ch != -1)
		return ((char *)&s[last_ch]);
	return (NULL);
}
