/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:53:20 by pledieu           #+#    #+#             */
/*   Updated: 2024/11/15 08:48:59 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_separator(char p, char c)
{
	int	i;

	i = 0;
	if (p == c)
		return (1);
	i++;
	return (0);
}

int	count_strings(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && check_separator(str[i], c))
			i++;
		if (str[i])
			count++;
		while (str[i] && !check_separator(str[i], c))
			i++;
	}
	return (count);
}

char	*ft_word(char *str, char c)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	len_word = 0;
	while (str[len_word] && !check_separator(str[len_word], c))
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

char	**free_function(char **strings)
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

char	**ft_split(char *str, char c)
{
	char	**strings;
	int		i;

	i = 0;
	strings = malloc(sizeof(char *) * (count_strings(str, c) + 1));
	if (!strings)
		return (NULL);
	while (*str != '\0')
	{
		while (*str != '\0' && check_separator(*str, c))
			str++;
		if (*str != '\0')
		{
			strings[i] = ft_word(str, c);
			if (!strings[i])
				return (free_function(strings));
			i++;
		}
		while (*str && !check_separator(*str, c))
			str++;
	}
	strings[i] = 0;
	return (strings);
}

// #include <stdio.h>
// int	main(int ac, char **av)
// {
// 	int	i;
// 	char	**split;
// 	(void) ac;
// 	split = ft_split(av[1], av[2][0]);
// 	i = 0;
// 	printf("{");
// 	while (split[i])
// 	{
// 		printf("{");
// 		printf("%s", split[i]);
// 		printf("}");
// 		i++;
// 	}
// 	printf("}");
// 	free_function(split);
// 	return (0);
// }