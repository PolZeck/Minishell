/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:23:47 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/16 13:15:28 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_unused_fds(t_pipex *pipex)
{
	if (pipex == NULL)
		return ;
	if (pipex->pipe_fd[0] > 2)
		close(pipex->pipe_fd[0]);
	if (pipex->pipe_fd[1] > 2)
		close(pipex->pipe_fd[1]);
	if (pipex->in_fd > 2)
		close(pipex->in_fd);
	if (pipex->out_fd > 2)
		close(pipex->out_fd);
}

static void	apply_input_redirections(t_list *redirs, t_pipex *pipex)
{
	t_list	*node;
	t_redir	*redir;
	int		fd;

	node = redirs;
	while (node)
	{
		redir = node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				//perror(redir->file);
				close_unused_fds(pipex);
				//exit(1);
			}
			else
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		node = node->next;
	}
}

static void	apply_output_redirection(t_redir *redir, t_pipex *pipex)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (!redir)
		return ;
	if (redir->type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		close_unused_fds(pipex);
		(void)pipex;
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	apply_redirections(t_list *redirs, t_pipex *pipex)
{
	t_redir	*last_out;

	apply_input_redirections(redirs, pipex);
	open_and_close_all_outputs(redirs);
	last_out = find_last_output_redirection(redirs);
	apply_output_redirection(last_out, pipex);
}
