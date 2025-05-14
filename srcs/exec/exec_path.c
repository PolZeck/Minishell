/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:49:57 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/15 00:43:50 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	**get_path_split(char *cmd, t_data *data)
{
	char	*path;
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	path = ft_getenv(data, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*prefix;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		prefix = ft_strjoin(paths[i], "/");
		if (!prefix)
			return (NULL);
		full_path = ft_strjoin(prefix, cmd);
		free(prefix);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_data *data)
{
	char	**paths;
	char	*result;

	paths = get_path_split(cmd, data);
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, cmd);
	free_split(paths);
	return (result);
}
