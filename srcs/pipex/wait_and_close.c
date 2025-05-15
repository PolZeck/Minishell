/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_and_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:34:28 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/15 14:41:30 by pledieu          ###   ########lyon.fr   */
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

void	update_status(int status, int *final_status,
		bool is_last, int *received_sigint)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			*received_sigint = 1;
		if (is_last)
		{
			if (sig == SIGQUIT)
				write(2, "Quit (core dumped)\n", 20);
			*final_status = 128 + sig;
		}
	}
	else if (WIFEXITED(status) && is_last)
		*final_status = WEXITSTATUS(status);
}

int	wait_for_processes(t_pipex *pipex)
{
	int	i;
	int	status;
	int	final_status;
	int	received_sigint;

	final_status = 0;
	received_sigint = 0;
	i = -1;
	while (++i < pipex->num_cmds)
	{
		waitpid(pipex->pid[i], &status, 0);
		update_status(status,
			&final_status, i == pipex->num_cmds - 1, &received_sigint);
	}
	if (received_sigint)
		write(1, "\n", 1);
	clean(pipex);
	return (final_status);
}
