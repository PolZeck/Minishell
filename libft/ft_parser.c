/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:11:48 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/10 13:19:00 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_parse_format(char format, va_list args)
{
	if (format == 'c')
		return (ft_handle_char(va_arg(args, int)));
	else if (format == 's')
		return (ft_handle_string(va_arg(args, char *)));
	else if (format == 'd' || format == 'i')
		return (ft_handle_int(va_arg(args, int)));
	else if (format == 'u')
		return (ft_handle_unsigned(va_arg(args, unsigned int)));
	else if (format == 'x')
		return (ft_handle_hex(va_arg(args, unsigned int), 0));
	else if (format == 'X')
		return (ft_handle_hex(va_arg(args, unsigned int), 1));
	else if (format == 'p')
		return (ft_handle_pointer(va_arg(args, void *)));
	else
		return (ft_putchar('%'));
}
