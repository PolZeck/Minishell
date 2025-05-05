/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:44:47 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/05 12:36:29 by pledieu          ###   ########lyon.fr   */
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

static char	*increment_shlvl(const char *shlvl_entry)
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

char	**dup_env(char **envp)
{
	int		i, j;
	int		has_shlvl = 0;
	char	**copy;
	char	*cwd;

	if (!envp || !*envp)
	{
		copy = malloc(sizeof(char *) * 5);
		if (!copy)
			return (NULL);
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			copy[0] = ft_strjoin("PWD=", cwd);
			free(cwd);
		}
		else
			copy[0] = ft_strdup("PWD=");
		copy[1] = ft_strdup("SHLVL=1");
		copy[2] = ft_strdup("_=/usr/bin/env");
		copy[3] = ft_strdup("OLDPWD");
		copy[4] = NULL;
		return (copy);
	}

	// Compte les entrées
	for (i = 0; envp[i]; i++)
		;
	copy = malloc(sizeof(char *) * (i + 2)); // +1 pour SHLVL si absent
	if (!copy)
		return (NULL);

	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			copy[j++] = increment_shlvl(envp[i]);
			has_shlvl = 1;
		}
		else
			copy[j++] = ft_strdup(envp[i]);
		i++;
	}
	if (!has_shlvl)
		copy[j++] = ft_strdup("SHLVL=1");
	copy[j] = NULL;
	return (copy);
}


