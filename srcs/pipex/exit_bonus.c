/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:41:37 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/09 18:32:09 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_exec_error(char *path_cmd, char **tokenized_cmd)
{
	my_free(path_cmd);
	my_free2((void **)tokenized_cmd);
	if (errno == EACCES)
		exit(126);
	else
		exit(127);
}

void	my_perr(const char *msg, bool should_exit)
{
	perror(msg);
	if (should_exit)
		exit(EXIT_FAILURE);
}

void	my_free2(void **mallocated)
{
	int	i;

	if (!mallocated)
		return ;
	i = 0;
	while (mallocated[i] != NULL)
	{
		if (mallocated[i] != NULL)
		{
			free(mallocated[i]);
			mallocated[i] = NULL;
		}
		i++;
	}
	free(mallocated);
}

void	my_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	clean(t_pipex *pipex)
{
	close_fds(pipex);
	if (pipex->pid)
	{
		free(pipex->pid);
		pipex->pid = NULL;
	}
	if (pipex->here_doc_flag)
		unlink(HERE_DOC_FILE);
}
