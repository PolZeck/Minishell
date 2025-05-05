/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:40:34 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/05 15:21:35 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	write_here_doc(char *limiter, t_pipex *pipex)
{
	char	*line;
	int		limiter_len;

	limiter_len = ft_strlen(limiter);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			if (errno == 0)
				break ;
			else
				(clean(pipex), my_perr(ERR_MALLOC, true));
		}
		if (ft_strncmp(line, limiter, limiter_len) == 0
			&& (line[limiter_len] == '\n' || line[limiter_len] == '\0'))
		{
			free(line);
			break ;
		}
		write(pipex->in_fd, line, ft_strlen(line));
		free(line);
	}
}

void	close_and_perr_fork(t_pipex *pipex)
{
	clean(pipex);
	close_fds(pipex);
	my_perr(ERR_FORK, true);
}

int	handle_signal_status(int status)
{
	int sig = WTERMSIG(status);

	if (sig == SIGINT)
	{
		return (130); // standard: 128 + SIGINT
	}
	else if (sig == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 20);
		return (131); // standard: 128 + SIGQUIT
	}
	return (128 + sig);
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

		if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGINT)
				received_sigint = 1; // ✅ on a vu au moins un SIGINT

			if (i == pipex->num_cmds - 1)
				final_status = 128 + sig;
		}
		else if (i == pipex->num_cmds - 1 && WIFEXITED(status))
			final_status = WEXITSTATUS(status);
	}
	if (received_sigint)
		write(1, "\n", 1); // ✅ une seule fois à la fin si au moins un SIGINT

	clean(pipex);
	return (final_status);
}

