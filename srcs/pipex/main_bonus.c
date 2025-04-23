/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:34:28 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/23 12:56:10 by lcosson          ###   ########.fr       */
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

void	check_in_and_outfile_bonus(int argc, char **argv, t_pipex *pipex)
{
	if (ft_strlen(argv[1]) == 0)
	{
		ft_putstr_fd(ERR_MSG_NO_SUCH_FILE, STDERR_FILENO);
		close_fds(pipex);
		exit(1);
	}
	if (ft_strlen(argv[argc - 1]) == 0)
	{
		ft_putstr_fd(ERR_PERMISSION, STDERR_FILENO);
		close_fds(pipex);
		exit(1);
	}
}

/* int	main_bonus(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		status;

	if (argc < 5)
	{
		ft_putstr_fd(ERR_ARG, STDERR_FILENO);
		exit (1);
	}
	if (argc < 6 && (ft_strncmp(argv[1], "here_doc", 8) == 0))
	{
		ft_putstr_fd(ERR_ARG_HEREDOC, STDERR_FILENO);
		exit (1);
	}
	check_in_and_outfile_bonus(argc, argv, &pipex);
	init_struct(argc, argv, &pipex);
	init_pipes(&pipex);
	if (pipex.here_doc_flag == FALSE)
		init_files(argc, argv, &pipex);
	else
		init_files_heredoc(argc, argv, &pipex);
	status = my_pipex(argv, envp, &pipex);
	close_fds(&pipex);
	return (status);
} */
