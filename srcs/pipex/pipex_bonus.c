/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:40:34 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/09 18:29:56 by lcosson          ###   ########.fr       */
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
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

int	wait_for_processes(t_pipex *pipex)
{
	int	i;
	int	status;
	int	final_status;

	final_status = 0;
	i = -1;
	while (++i < pipex->num_cmds)
	{
		waitpid(pipex->pid[i], &status, 0);
		if (i == pipex->num_cmds - 1)
		{
			if (WIFEXITED(status))
				final_status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				final_status = handle_signal_status(status);
		}
	}
	clean(pipex);
	return (final_status);
}

int	my_pipex(char **argv, char **envp, t_pipex *pipex)
{
	int	i;
	int	status;

	pipex->pid = ft_calloc(pipex->num_cmds, sizeof(pid_t));
	if (!pipex->pid)
	{
		close_fds(pipex);
		clean(pipex);
		my_perr(ERR_MALLOC, true);
	}
	i = -1;
	while (++i < pipex->num_cmds)
	{
		if (i == 0)
			pipex->pid[i] = first_execution_bonus(i, argv, envp, pipex);
		else if (i == pipex->num_cmds - 1)
			pipex->pid[i] = last_execution_bonus(i, argv, envp, pipex);
		else
			pipex->pid[i] = middle_execution_bonus(i, argv, envp, pipex);
	}
	close_fds(pipex);
	status = wait_for_processes(pipex);
	clean(pipex);
	return (status);
}
