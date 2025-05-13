/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:21:27 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/13 12:31:28 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	is_valid_identifier_export(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '=' && i == 0)
		return (0);
	return (1);
}

int	var_exists(char **env, char *name)
{
	int	len;
	int	i;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char	**replace_env(char **env, char *new_entry, int idx)
{
	free(env[idx]);
	env[idx] = ft_strdup(new_entry);
	return (env);
}

char	**append_env(char **env, char *new_entry)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i++] = ft_strdup(new_entry);
	new_env[i] = NULL;
	free_split(env);
	return (new_env);
}

char	**replace_or_append_env(char **env, char *entry)
{
	int		len;
	int		i;

	if (!entry)
		return (env);
	len = 0;
	while (entry[len] && entry[len] != '=')
		len++;
	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], entry, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(entry);
			return (env);
		}
		i++;
	}
	return (append_env(env, entry));
}
