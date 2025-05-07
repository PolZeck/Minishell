/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tmp_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:34:01 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/07 13:36:27 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		write(2, "Erreur malloc\n", 14);
		exit(1);
	}
	return (ptr);
}

static void	fill_random_bytes(unsigned char *buf, size_t len)
{
	int		fd;
	ssize_t	ret;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		write(2, "Erreur ouverture /dev/urandom\n", 31);
		exit(1);
	}
	ret = read(fd, buf, len);
	if (ret != (ssize_t)len)
	{
		write(2, "Erreur lecture /dev/urandom\n", 29);
		close(fd);
		exit(1);
	}
	close(fd);
}

void	generate_random_name(char *output, size_t len)
{
	const char		*charset;
	unsigned char	*buf;
	size_t			i;

	charset = "abcdefghijklmnopqrstuvwxyz0123456789";
	buf = safe_malloc(len);
	i = 0;
	fill_random_bytes(buf, len);
	while (i < len)
	{
		output[i] = charset[buf[i] % ft_strlen(charset)];
		i++;
	}
	output[len] = '\0';
	free(buf);
}
