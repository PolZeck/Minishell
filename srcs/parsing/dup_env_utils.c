/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:44:47 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/12 14:05:42 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*ft_getenv(t_data *data, const char *name)
{
	int		i;
	size_t	len;

	if (!data || !data->env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], name, len) && data->env[i][len] == '=')
			return (data->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*increment_shlvl(const char *shlvl_entry)
{
	int		level;
	char	*new_entry;
	char	*tmp;

	if (!shlvl_entry || ft_strncmp(shlvl_entry, "SHLVL=", 6) != 0)
		return (ft_strdup("SHLVL=1"));
	level = ft_atoi(shlvl_entry + 6);
	if (level < 0)
		level = 0;
	level++;
	tmp = ft_itoa(level);
	new_entry = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	return (new_entry);
}

int	count_env(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}
