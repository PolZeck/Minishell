/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:42:11 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/09 09:20:56 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

bool	has_input_redir(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;

	if (!cmd)
		return (false);
	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			return (true);
		node = node->next;
	}
	return (false);
}

bool	has_output_redir(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;

	if (!cmd)
		return (false);
	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			return (true);
		node = node->next;
	}
	return (false);
}

void	apply_redirections(t_list *redirs)
{
	t_list	*node;
	t_redir	*redir;
	t_redir	*last_out;
	int		fd;
	int		flags;

	node = redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		node = node->next;
	}
	node = redirs;
	last_out = NULL;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			flags = O_WRONLY | O_CREAT;
			if (redir->type == APPEND)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(redir->file, flags, 0644);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			close(fd);
			last_out = redir;
		}
		node = node->next;
	}
	if (last_out)
	{
		flags = O_WRONLY | O_CREAT;
		if (last_out->type == APPEND)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(last_out->file, flags, 0644);
		if (fd == -1)
		{
			perror(last_out->file);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	close_and_clean_in_fd(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	close_fds(pipex);
	clean (pipex);
	exit(1);
}
