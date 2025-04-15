/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:41:07 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/09 18:26:06 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_struct(int argc, char **argv, t_pipex *pipex)
{
	ft_memset(pipex, 0, sizeof(t_pipex));
	pipex->in_fd = -1;
	pipex->out_fd = -1;
	pipex->pipe_fd[0] = -1;
	pipex->pipe_fd[1] = -1;
	pipex->prev_pipe_fd = -1;
	pipex->here_doc_flag = FALSE;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		pipex->here_doc_flag = TRUE;
	if (pipex->here_doc_flag)
		pipex->num_cmds = argc - 4;
	else
		pipex->num_cmds = argc - 3;
	pipex->pid = NULL;
}

void	close_and_perr_dup2(t_pipex *pipex)
{
	clean (pipex);
	close_fds(pipex);
	my_perr(ERR_DUP2, true);
}

void	init_files(int argc, char **argv, t_pipex *pipex)
{
	pipex->in_fd = open(argv[1], O_RDONLY);
	if (pipex->in_fd == -1)
	{
		perror(argv[1]);
	}
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (pipex->out_fd == -1)
	{
		perror(argv[argc - 1]);
		close_fds(pipex);
		exit(EXIT_FAILURE);
	}
}

void	init_files_heredoc(int argc, char **argv, t_pipex *pipex)
{
	pipex->in_fd = open(HERE_DOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (pipex->in_fd == -1)
		my_perr(HERE_DOC_FILE, true);
	write_here_doc(argv[2], pipex);
	close(pipex->in_fd);
	pipex->in_fd = open(HERE_DOC_FILE, O_RDONLY);
	if (pipex->in_fd == -1)
	{
		unlink(HERE_DOC_FILE);
		my_perr(HERE_DOC_FILE, true);
	}
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (pipex->out_fd == -1)
	{
		if (access(argv[argc - 1], F_OK) == -1)
			ft_putstr_fd(ERR_MSG_NO_SUCH_FILE, STDERR_FILENO);
		else if (access(argv[argc - 1], W_OK) == -1)
			ft_putstr_fd(ERR_PERMISSION, STDERR_FILENO);
		else
			my_perr(ERR_FD, false);
		close(pipex->in_fd);
		unlink(HERE_DOC_FILE);
		exit(EXIT_FAILURE);
	}
}

void	init_pipes(t_pipex *pipex)
{
	if (pipe(pipex->pipe_fd) == -1)
	{
		clean(pipex);
		my_perr(ERR_PIPE, true);
	}
}
