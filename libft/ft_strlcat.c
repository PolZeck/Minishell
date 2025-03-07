/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:51:38 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/07 16:05:17 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_strlcat(char *dest, char *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	len_d;
	unsigned int	len_s;

	i = 0;
	j = 0;
	while (dest[j] != '\0')
	{
		j++;
	}
	len_d = j;
	len_s = ft_strlen(src);
	if (size == 0 || size <= len_d)
		return (len_s + size);
	while (src[i] != '\0' && i < size - len_d - 1)
	{
		dest[j] = src [i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (len_d + len_s);
}
