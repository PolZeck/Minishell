/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:40:34 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/07 16:24:55 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	handle_signal_status(int status)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
	{
		return (130);
	}
	else if (sig == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 20);
		return (131);
	}
	return (128 + sig);
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
