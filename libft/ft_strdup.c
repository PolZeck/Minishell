/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:47:59 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/12 16:55:05 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	char	*dest;

	i = 0;
	dest = (char *)malloc(ft_strlen(s) * sizeof(char) + 1);
	if (!dest)
		return (0);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

// int main (void)
// {
//     char    *str;
//     char    *allocated;

//     str = "malloc caf";
//     printf("original  : $%s$ base  : @ %p\n", str, str);
//     allocated = strdup(str);
//     printf("strdup    : $%s$ alloc : @ %p\n", allocated, allocated);
//     allocated = ft_strdup(str);
//     printf("ft_strdup : $%s$ alloc : @ %p\n", allocated, allocated);
// }
