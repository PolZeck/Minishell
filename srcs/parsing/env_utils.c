/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:44:47 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/24 10:51:45 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <string.h>

char	*ft_getenv(t_data *data, const char *name)
{
	int		i;
	size_t	len;

	if (!data || !data->env || !name)
		return (NULL);
	len = strlen(name);
	i = 0;
	while (data->env[i])
	{
		if (!strncmp(data->env[i], name, len) && data->env[i][len] == '=')
			return (data->env[i] + len + 1);
		i++;
	}
	return (NULL);
}
