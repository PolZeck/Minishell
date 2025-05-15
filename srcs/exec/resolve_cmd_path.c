/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_cmd_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:43:26 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 17:43:36 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	*check_absolute_path(t_cmd *cmd)
{
	struct stat	st;

	if (stat(cmd->args[0], &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			print_err("bash: ", cmd->args[0], ": Is a directory\n", 126);
			return (NULL);
		}
		if (access(cmd->args[0], X_OK) != 0)
		{
			print_err("bash: ", cmd->args[0], ": Permission denied\n", 126);
			return (NULL);
		}
		return (ft_strdup(cmd->args[0]));
	}
	print_err("bash: ", cmd->args[0], ": No such file or directory\n", 127);
	return (NULL);
}

char	*resolve_cmd_path(t_cmd *cmd, t_data *data)
{
	char	*path;
	char	*env_path;
	char	*fallback;

	if (ft_strchr(cmd->args[0], '/'))
		return (check_absolute_path(cmd));
	env_path = ft_getenv(data, "PATH");
	if (!env_path || env_path[0] == '\0')
	{
		fallback = ft_strjoin("./", cmd->args[0]);
		if (access(fallback, X_OK) == 0)
			return (fallback);
		free(fallback);
		print_err("bash: ", cmd->args[0], ": No such file or directory\n", 127);
		return (NULL);
	}
	path = find_command_path(cmd->args[0], data);
	if (!path)
		print_err("bash: ", cmd->args[0], ": command not found\n", 127);
	return (path);
}
