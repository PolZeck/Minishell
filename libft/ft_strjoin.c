/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:53:28 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/08 14:53:15 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;

	str = NULL;
	i = 0;
	str = malloc((sizeof(char) * (ft_strlen(s1) + ft_strlen(s2))) + 1);
	if (!str)
		return (NULL);
	while (*s1)
		str[i++] = *s1++;
	while (*s2)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
}

static size_t	ft_strlcpy_gnl(char *dst, const char *src, size_t size)
{
	size_t	i;

	if (!src || !dst)
		return (0);
	if (size == 0)
		return (ft_strlen_gnl(src));
	i = 0;
	while (src[i] && (i < (size - 1)))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen_gnl(src));
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*concat;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen_gnl(s1);
	s2_len = 0;
	while (s2[s2_len] && s2[s2_len] != '\n')
		s2_len++;
	if (s2[s2_len] == '\n')
		s2_len++;
	concat = malloc(s1_len + s2_len + 1);
	if (!concat)
		return (free(s1), NULL);
	if (s1)
		ft_strlcpy_gnl(concat, s1, s1_len + 1);
	ft_strlcpy_gnl(concat + s1_len, s2, s2_len + 1);
	free(s1);
	return (concat);
}
