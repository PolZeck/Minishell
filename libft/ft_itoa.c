/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:52:51 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/12 14:25:20 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	intlen(int n)
{
	long	nb;
	int		i;

	nb = n;
	i = 0;
	if (nb < 0)
	{
		i++;
		nb = -nb;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	i++;
	return (i);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;
	int		i;
	long	l;

	l = (long)n;
	i = 0;
	len = intlen(l);
	res = malloc((sizeof(char) * len) + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (n < 0)
	{
		res[0] = '-';
		l = -l;
		i++;
	}
	while (len-- > i)
	{
		res[len] = (l % 10) + '0';
		l = l / 10;
	}
	return (res);
}

// int main()
// {
// 	int numbers[] = {0, -123, 456, -2147483648, 2147483647};
// 	int i;
// 	char *str;

// 	for (i = 0; i < 5; i++)
// 	{
// 		str = ft_itoa(numbers[i]);
// 		printf("ft_itoa(%d) = %s\n", numbers[i], str);
// 		free(str);
// 	}
// 	return 0;
// }