/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:53:40 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/13 11:36:28 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;

	c = c % 256;
	str = (char *)s + ft_strlen(s);
	while (str > s)
	{
		if (*str == c)
			return (str);
		str--;
	}
	if (*str == c)
		return (str);
	return (NULL);
}
