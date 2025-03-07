/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:53:02 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/13 14:03:07 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*uc1;
	unsigned char	*uc2;

	if (n && s2 != s1)
	{
		uc1 = (unsigned char *) s1;
		uc2 = (unsigned char *) s2;
		while (n > 0)
		{
			if (*uc1 != *uc2)
				return (*uc1 - *uc2);
			uc1++;
			uc2++;
			n--;
		}
	}
	return (0);
}
