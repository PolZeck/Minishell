/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:40:14 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/09 15:28:52 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	check_open_files_bonus(int argc, char **argv, t_pipex *pipex)
{
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (pipex->out_fd == -1)
	{
		ft_putstr_fd(argv[argc - 1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		close_fds(pipex);
		pipex->out_fd = -1;
	}
	pipex->in_fd = open(argv[1], O_RDONLY);
	if (pipex->in_fd == -1)
	{
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		pipex->in_fd = -1;
	}
}

static char	*handle_slash_case_bonus(char *cmd, t_pipex *pipex)
{
	if (access(cmd, F_OK) == -1)
	{
		ft_putstr_fd(ERR_COMMAND, STDERR_FILENO);
		close_fds(pipex);
		exit(127);
	}
	else if (access(cmd, X_OK) == -1)
	{
		ft_putstr_fd(ERR_PERMISSION, STDERR_FILENO);
		close_fds(pipex);
		exit(126);
	}
	return (ft_strdup(cmd));
}

char	*check_addpath_cmd_bonus(char *cmd, char *envp[], t_pipex *pipex)
{
	char	*path;
	char	*path_cmd;

	if (!cmd || ft_strlen(cmd) == 0)
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL)
		return (handle_slash_case_bonus(cmd, pipex));
	path = my_getpath(envp);
	if (!path)
		return (NULL);
	path_cmd = my_addpath_cmd(cmd, path, pipex);
	my_free(path);
	if (!path_cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(ERR_COMMAND, STDERR_FILENO);
	}
	return (path_cmd);
}

char	*my_addpath_cmd(char *cmd, char *path, t_pipex *pipex)
{
	char	*path_dir;
	char	*path_cmd;

	if (!cmd || !path)
		return (NULL);
	path_dir = ft_strtok(path, ":");
	path_cmd = NULL;
	while (path_dir != NULL)
	{
		path_cmd = ft_strjoin_char(path_dir, cmd, '/');
		if (!path_cmd)
		{
			close_fds(pipex);
			my_perr(ERR_MALLOC, false);
			return (NULL);
		}
		if (access(path_cmd, F_OK | X_OK) == 0)
			break ;
		my_free(path_cmd);
		path_dir = ft_strtok(NULL, ":");
	}
	if (path_dir == NULL)
		return (NULL);
	return (path_cmd);
}

char	*my_getpath(char **envp)
{
	char	*path;
	int		i;

	path = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (ft_strdup(path));
}
