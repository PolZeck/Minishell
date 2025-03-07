/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:44:08 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/15 08:45:06 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (size > __SIZE_MAX__ / nmemb || nmemb > __SIZE_MAX__ / size)
		return (NULL);
	res = malloc(size * nmemb);
	if (!res)
		return (NULL);
	res = ft_memset(res, 0, nmemb * size);
	return (res);
}

// int	main(void)
// {
// 	printf("%lu", __SIZE_MAX__);
// }