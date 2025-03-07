/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:47:17 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/10 13:19:12 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	clean_buffer(char *buf)
{
	size_t	i;

	i = 0;
	while (buf[i])
		buf[i++] = '\0';
}

static void	shift_buffer(char *buf)
{
	size_t	j;
	size_t	i;

	i = 0;
	j = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (buf[i] == '\n')
		i++;
	while (buf[i + j])
	{
		buf[j] = buf[i + j];
		j++;
	}
	buf[j] = '\0';
}

char	*get_next_line(int fd)
{
	static char	stock[BUFFER_SIZE + 1] = "\0";
	char		*res;
	int			bytes_read;

	bytes_read = 1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	res = ft_strdup_gnl(stock);
	if (!res)
		return (NULL);
	while (bytes_read && ft_check_line(res) == 0)
	{
		bytes_read = read(fd, stock, BUFFER_SIZE);
		if (bytes_read < 0)
			return (clean_buffer(stock), free(res), NULL);
		stock[bytes_read] = '\0';
		res = ft_strjoin_gnl(res, stock);
		if (!res)
			return (NULL);
	}
	shift_buffer(stock);
	if (res[0] == 0)
		return (free(res), NULL);
	return (res);
}
