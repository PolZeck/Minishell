/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:49:30 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/07 15:15:59 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_strlcpy(char *dest, char *src, unsigned int size)
{
	unsigned int	i;

	i = 0;
	if (size > 0)
	{
		size = size - 1;
		while (src[i] && i < size)
		{
			if (i < size)
				*(dest + i) = *(src + i);
			i++;
		}
		dest[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}
