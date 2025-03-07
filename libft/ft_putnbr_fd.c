/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:53:13 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/14 11:28:02 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int		sign;
	char	tmp;

	sign = 1;
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		sign = -1;
		write(fd, "-", 1);
	}
	if (n > 9 || n < -9)
		ft_putnbr_fd((n / 10) * sign, fd);
	tmp = ((n % 10) * sign) + '0';
	write(fd, &tmp, 1);
}
