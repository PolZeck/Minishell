/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:39:55 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/05 15:45:15 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

int	ft_strtoll_body(const char *str, int sign, long long *result)
{
	long long	value;
	long long	digit;

	value = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		digit = *str - '0';
		if (sign == 1 && value > (LLONG_MAX - digit) / 10)
			return (0);
		if (sign == -1 && value > (-(LLONG_MIN + digit) / 10))
			return (0);
		value = value * 10 + digit;
		str++;
	}
	*result = value * sign;
	return (1);
}

static int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

long long	ft_strtoll(const char *str, long long *result)
{
	int			sign;
	long long	value;

	sign = 1;
	value = 0;
	*result = 0;
	while (is_space(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	return (ft_strtoll_body(str, sign, result));
}
