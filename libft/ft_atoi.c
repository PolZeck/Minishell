/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:54:41 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/15 11:14:03 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int					i;
	int					sign;
	unsigned long long	ans;

	i = 0;
	sign = 1;
	ans = 0;
	while (((nptr[i] >= 9 && nptr[i] <= 13)) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		ans = ans * 10 + (nptr[i] - '0');
		if (ans >= __LONG_LONG_MAX__ && sign < 0)
			return (0);
		if (ans >= __LONG_LONG_MAX__)
			return (-1);
		i++;
	}
	return (((int) ans) * sign);
}

// int	main(int ac, char **av)
// {
// 	(void) ac;
// 	printf("mon atoi %d\n", ft_atoi(av[1]));
// 	printf("vrai atoi %d", atoi(av[1]));
// }