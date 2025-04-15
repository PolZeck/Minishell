/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:24:29 by lcosson           #+#    #+#             */
/*   Updated: 2025/03/27 13:59:06 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t		count;
	const char	*r;

	count = 0;
	while (*s)
	{
		r = reject;
		while (*r)
		{
			if (*s == *r)
				return (count);
			r++;
		}
		count++;
		s++;
	}
	return (count);
}
