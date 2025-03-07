/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:53:42 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/14 14:36:24 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char const *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (is_set(set, s1[start]) && s1[start])
		start++;
	while (end > start && is_set(set, s1[end - 1]))
		end--;
	str = ft_substr(s1, start, end - start);
	if (!str)
		return (NULL);
	str[end - start] = '\0';
	return (str);
}

// int	main(void)
// {
// 	char *str;
// 	int	i;

// 	i = 0;

// 	str = malloc(sizeof(char) * 50);
// 	str = ft_strtrim("  fripouille         ", " ");
// 	while (str[i])
// 	{
// 		printf("%c",str[i]);
// 		i++;
// 	}
// }