/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:32:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 10:37:17 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	copy_env_vars_with_shlvl(char **envp, char **copy, int initial)
{
	int	i;
	int	j;
	int	has_shlvl;

	i = 0;
	j = 0;
	has_shlvl = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			has_shlvl = 1;
		if (initial && ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			copy[j++] = increment_shlvl(envp[i]);
		else
			copy[j++] = ft_strdup(envp[i]);
		i++;
	}
	return (has_shlvl);
}

static char	**copy_and_process_env(char **envp, int initial)
{
	int		count;
	char	**copy;
	int		has_shlvl;

	count = count_env(envp);
	copy = malloc(sizeof(char *) * (count + 2));
	if (!copy)
		return (NULL);
	has_shlvl = copy_env_vars_with_shlvl(envp, copy, initial);
	if (!has_shlvl)
		copy[count++] = ft_strdup("SHLVL=1");
	copy[count] = NULL;
	return (copy);
}

static char	**create_minimal_env(void)
{
	char	**copy;
	char	*cwd;

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

char	**dup_env(char **envp, int initial)
{
	if (!envp || !*envp)
		return (create_minimal_env());
	return (copy_and_process_env(envp, initial));
}
