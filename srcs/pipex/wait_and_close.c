/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:34:28 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/07 16:33:20 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_fds(t_pipex *pipex)
{
	if (pipex->in_fd > 2)
		close(pipex->in_fd);
	if (pipex->out_fd > 2)
		close(pipex->out_fd);
	if (pipex->pipe_fd[0] > 2)
		close(pipex->pipe_fd[0]);
	if (pipex->pipe_fd[1] > 2)
		close(pipex->pipe_fd[1]);
	if (pipex->prev_pipe_fd > 2)
		close(pipex->prev_pipe_fd);
	pipex->in_fd = -1;
	pipex->out_fd = -1;
	pipex->pipe_fd[0] = -1;
	pipex->pipe_fd[1] = -1;
	pipex->prev_pipe_fd = -1;
}

int	wait_for_processes(t_pipex *pipex)
{
	int	i;
	int	status;
	int	final_status;
	int	received_sigint;
	int	sig;

	final_status = 0;
	received_sigint = 0;
	i = -1;
	while (++i < pipex->num_cmds)
	{
		waitpid(pipex->pid[i], &status, 0);
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
				received_sigint = 1;
			if (i == pipex->num_cmds - 1)
				final_status = 128 + sig;
		}
		else if (i == pipex->num_cmds - 1 && WIFEXITED(status))
			final_status = WEXITSTATUS(status);
	}
	if (received_sigint)
		write(1, "\n", 1);
	clean(pipex);
	return (final_status);
}
