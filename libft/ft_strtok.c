/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:31:10 by lcosson           #+#    #+#             */
/*   Updated: 2025/03/24 11:33:46 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*saveptr = NULL;
	char		*token;
	int			i;

	i = 0;
	if (!str)
		str = saveptr;
	if (!str)
		return (NULL);
	while (str[i] && ft_strchr(delim, str[i]))
		i++;
	if (!str[i])
		return (NULL);
	token = str + i;
	while (str[i] && !ft_strchr(delim, str[i]))
		i++;
	if (str[i])
	{
		str[i] = '\0';
		saveptr = str + i + 1;
	}
	else
		saveptr = NULL;
	return (token);
}
