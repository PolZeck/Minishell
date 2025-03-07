/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:52:47 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/13 10:55:56 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	uc1;
	unsigned char	uc2;
	size_t			i;

	i = 0;
	while ((i < n) && ((s1[i]) || (s2[i])))
	{
		uc1 = (unsigned char) s1[i];
		uc2 = (unsigned char) s2[i];
		if (uc1 > uc2)
			return (1);
		if (uc1 < uc2)
			return (-1);
		i++;
	}
	return (0);
}
