/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 00:50:51 by pol               #+#    #+#             */
/*   Updated: 2025/05/19 13:47:23 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	open_input_redir(t_redir *redir, t_data *data)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->file);
		data->exit_status = 1;
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (fd);
}

static int	open_output_redir(t_redir *redir, t_data *data)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (redir->type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		data->exit_status = 1;
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (fd);
}

int	apply_redirs(t_list *redirs, t_data *data)
{
	t_redir	*redir;
	t_list	*node;
	int		fd;

	node = redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		fd = -1;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			fd = open_input_redir(redir, data);
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
			fd = open_output_redir(redir, data);
		if (fd == -1)
			return (1);
		node = node->next;
	}
	return (0);
}
