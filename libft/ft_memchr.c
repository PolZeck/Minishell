/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:52:59 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/13 13:35:41 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;

	if (n != 0)
	{
		str = (unsigned char *)s;
		while (n > 0)
		{
			if (*str == (unsigned char)c)
				return (str);
			str++;
			n--;
		}
	}
	return (NULL);
}
