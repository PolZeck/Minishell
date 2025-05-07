/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:57:14 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/07 15:01:10 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_output(t_redir *redir, int *out_fd)
{
	int	fd;
	int	flags;

	if (!redir->file)
		exit(1);
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
	if (*out_fd != -1)
		close(*out_fd);
	*out_fd = fd;
}

void	handle_child_input(t_redir *redir, int *in_fd)
{
	int	fd;

	if (!redir->file)
		exit(1);
	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->file);
		exit(1);
	}
	if (*in_fd != -1)
		close(*in_fd);
	*in_fd = fd;
}

void	handle_child_heredoc(t_redir *redir, int *in_fd)
{
	if (redir->fd == -1)
		exit(1);
	if (*in_fd != -1)
		close(*in_fd);
	*in_fd = redir->fd;
}

void	handle_child_redir(t_redir *redir, int *in_fd, int *out_fd)
{
	if (redir->type == HEREDOC)
		handle_child_heredoc(redir, in_fd);
	else if (redir->type == REDIR_IN)
		handle_child_input(redir, in_fd);
	else if (redir->type == REDIR_OUT || redir->type == APPEND)
		handle_child_output(redir, out_fd);
}

void	apply_redirections_in_child(t_cmd *cmd)
{
	t_list	*node;
	int		in_fd;
	int		out_fd;

	node = cmd->redirs;
	in_fd = -1;
	out_fd = -1;
	while (node)
	{
		handle_child_redir(node->content, &in_fd, &out_fd);
		node = node->next;
	}
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}
