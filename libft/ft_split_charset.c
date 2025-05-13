/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_charset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:28:41 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/13 12:32:00 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_charset(char p, const char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (p == charset[i])
			return (1);
		i++;
	}
	return (0);
}

int	count_strings_charset(char *str, const char *charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && is_charset(str[i], charset))
			i++;
		if (str[i])
			count++;
		while (str[i] && !is_charset(str[i], charset))
			i++;
	}
	return (count);
}

char	*ft_word_charset(char *str, const char *charset)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	len_word = 0;
	while (str[len_word] && !is_charset(str[len_word], charset))
		len_word++;
	word = malloc(sizeof(char) * (len_word + 1));
	if (!word)
		return (NULL);
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**free_function_charset(char **strings)
{
	int	i;

	i = 0;
	while (strings[i])
	{
		free(strings[i]);
		i++;
	}
	free(strings);
	return (NULL);
}

char	**ft_split_charset(char *str, const char *charset)
{
	char	**strings;
	int		i;

	i = 0;
	strings = malloc
		(sizeof(char *) * (count_strings_charset(str, charset) + 1));
	if (!strings)
		return (NULL);
	while (*str != '\0')
	{
		while (*str != '\0' && is_charset(*str, charset))
			str++;
		if (*str != '\0')
		{
			strings[i] = ft_word_charset(str, charset);
			if (!strings[i])
				return (free_function_charset(strings));
			i++;
		}
		while (*str && !is_charset(*str, charset))
			str++;
	}
	strings[i] = 0;
	return (strings);
}
