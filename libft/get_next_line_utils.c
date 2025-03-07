/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:47:21 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/10 13:19:10 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_strlen_gnl(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_check_line(char	*str)
{
	size_t	i;

	i = 0;
	if (ft_strlen_gnl(str + i) == 0)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static size_t	ft_strlcpy_gnl(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (!src || !dest)
		return (0);
	if (size == 0)
		return (ft_strlen_gnl(src));
	i = 0;
	while (src[i] && (i < (size - 1)))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen_gnl(src));
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*join;
	size_t	len_s1;
	size_t	len_s2;

	if (!s1 && !s2)
		return (NULL);
	len_s1 = ft_strlen_gnl(s1);
	len_s2 = 0;
	while (s2[len_s2] != '\n' && s2[len_s2] != 0)
		len_s2++;
	if (s2[len_s2] == '\n')
		len_s2++;
	join = malloc(len_s1 + len_s2 + 1);
	if (!join)
		return (free(s1), NULL);
	if (s1)
		ft_strlcpy_gnl(join, s1, len_s1 + 1);
	if (s2)
		ft_strlcpy_gnl(join + len_s1, s2, len_s2 + 1);
	free(s1);
	return (join);
}

char	*ft_strdup_gnl(const char *s)
{
	char	*dup;
	size_t	s_len;
	size_t	i;

	s_len = 0;
	while (s[s_len] != '\n' && s[s_len] != 0)
		s_len++;
	if (s[s_len] == '\n')
		s_len++;
	dup = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
