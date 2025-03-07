/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:40:05 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/12 09:52:17 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*dptr;
	char	*sptr;

	if (n && dest != src)
	{
		dptr = dest;
		sptr = (char *)src;
		while (n--)
			*dptr++ = *sptr++;
	}
	return (dest);
}
